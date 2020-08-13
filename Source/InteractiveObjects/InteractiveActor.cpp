#include "InteractiveActor.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Camera/CameraComponent.h"

// To help us see what the PlayerCharacter is looking at.
#include "DrawDebugHelpers.h"

/**
 * Sets default values.
 */
AInteractiveActor::AInteractiveActor()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// Load the meshes used for the Desk and Drawer.
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>DeskMeshAsset(TEXT("StaticMesh'/Game/Models/Furniture/Desk/desk_desk.desk_desk'"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>DeskDrawerMeshAsset(TEXT("StaticMesh'/Game/Models/Furniture/Desk/desk_drawer.desk_drawer'"));

	// Create the Root and set it as the RootComponent.
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Create the BoxComponent and set it to be a collision trigger and then
	// attach it to the RootComponent.
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->SetCollisionProfileName(TEXT("Trigger"));
	BoxCollider->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	BoxCollider->SetBoxExtent(FVector(150.f, 100.f, 80.f));
	BoxCollider->SetupAttachment(Root);

	//// Create the component for the Desk, set its mesh and position and then
	//// attach it to the Root.
	//Desk = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Desk"));
	//Desk->SetRelativeLocation(FVector(0.f, 110.f, -40.f));
	//Desk->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	//Desk->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	//Desk->SetStaticMesh(DeskMeshAsset.Object);
	//Desk->SetupAttachment(Root);

	//// Create the component for the Drawer, set its mesh and position and
	//// attach it to the Desk.
	//DeskDrawer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeskDrawer"));
	//DeskDrawer->SetStaticMesh(DeskDrawerMeshAsset.Object);
	//DeskDrawer->SetupAttachment(Desk);

	// Create the TextRenderComponent and add a generic "Press E to Interact"
	// message and set it to be invisible so the PlayerCharacter can't see it
	// and lastly attach it to the RootComponent.
	NoticeText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NoticeText"));
	NoticeText->SetRelativeLocation(FVector(0.f, 90.f, 80.f));
	NoticeText->SetText(FText::FromString("Press E to Interact"));
	NoticeText->SetTextRenderColor(FColor::Blue);
	NoticeText->SetVisibility(false);
	NoticeText->SetupAttachment(Root);
}

/**
 * Called when the game starts or when spawned.
 */
void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the methods that are used to respond to when the player enters or
	// leaves the BoxCollider's trigger area.
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveActor::OnComponentEnterInteractionArea);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &AInteractiveActor::OnComponentLeaveInteractionArea);

	// Add the DeskDrawer to the `InteractiveObjects` array.
	//InteractiveObjects.Add(DeskDrawer);
}

/**
 * Called every frame.
 *
 * @param DeltaTime The difference in time between the last frame and this one.
 */
void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * When a component enters this Actor's BoxCollider trigger area we check to
 * see if that component belonged to the PlayerCharacter and if so we start
 * the timer that runs the method that checks to see what the PlayerCharacter
 * is looking at.
 *
 * @param OverlappedComp
 * @param OtherActor
 * @param OtherComp
 * @param OtherBodyIndex
 * @param bFromSweep
 * @param SweepResult
 */
void AInteractiveActor::OnComponentEnterInteractionArea(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Return early if anything is null so we can avoid a potential crash.
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;

	// Try to cast to `PlayerCharacter` to see if it was the PlayerCharacter
	// that entered the interaction area. If not we can return early.
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter == nullptr) return;

	// Set this InteractiveActor to be the value of the PlayerCharacter's
	// `InteractionArea` so if they press the interact button we know they want
	// to interact with an interactive object in this InteractiveActor.
	PlayerCharacter->InteractionArea = this;

	// Create a FTimerDelegate so that we can pass the PlayerCharacter object to
	// our `LookForInteractions` method.
	FTimerDelegate PlayerLookTimerDel;
	PlayerLookTimerDel.BindUFunction(this, FName("LookForInteractions"), PlayerCharacter);

	// Set a timer that runs the `LookForInteractions` method every 0.1s which
	// checks to see if a Player is looking at an object that can be interacted with.
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(PlayerLookTimer, PlayerLookTimerDel, 0.1f, true);
	}
}

/**
 * When a component leaves this Actor's BoxCollider trigger area we check to see
 * if it was the PlayerCharacter and if so we stop the timer and hide the notice text.
 *
 * @param OverlappedComp
 * @param OtherActor
 * @param OtherComp
 * @param OtherBodyIndex
 */
void AInteractiveActor::OnComponentLeaveInteractionArea(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Return early if anything is null so we can avoid a potential crash.
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;

	// Try to cast to `PlayerCharacter` to see if it was the PlayerCharacter
	// that left the interaction area. If not we can return early.
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter == nullptr) return;

	// Set the InteractionArea of the PlayerCharacter to be a nullptr so that
	// they can't interact with this InteractiveActor when not in range.
	PlayerCharacter->InteractionArea = nullptr;

	// Stop the timer that checks for the player interacting with objects as
	// the player is no longer in range to do so.
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayerLookTimer);
	}
}

/**
 * Run on a timer when the PlayerCharacter enters the BoxCollider's trigger area
* and it checks to see if the PlayerCharacter is looking at any objects that can
* be interacted with by sending a trace from the PlayerCharacter's camera and
* checking for blocking collisions.
* 
* @param PlayerCharacter The PlayerCharacter that entered the BoxCollider's bounds.
 */
void AInteractiveActor::LookForInteractions(APlayerCharacter* PlayerCharacter)
{
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	// Set the start of the trace to be the player's camera.
	FVector Start = PlayerCharacter->PlayerCamera->GetComponentLocation();
	FVector ForwardVector = PlayerCharacter->PlayerCamera->GetForwardVector();

	// Set the end of the trace to be 1000.f units from the start in the direction
	// the player is facing.
	FVector End = ((ForwardVector * 1000.f) + Start);

	// DEBUG
	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	// If there's no collision or the collision wasn't a blocking collision then
	// we can return early.
	if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) return;
	if (!OutHit.bBlockingHit) return;

	// If there's no component then we also return early because we only deal
	// with the components of the InteractiveObjects.
	if (OutHit.GetComponent() == nullptr) return;

	// Attempt to cast the component to a UStaticMeshComponent and return early
	// if we can't.
	UStaticMeshComponent* InteractiveComponentHit = Cast<UStaticMeshComponent>(OutHit.GetComponent());
	if (InteractiveComponentHit == nullptr) return;

	// If the component that the PlayerCharacter is looking at is not part of the
	// array of interactive objects then we can return early.
	if (!InteractiveObjects.Contains(InteractiveComponentHit))
	{
		if (ObjectCurrentlyBeingLookedAt != nullptr)
		{
			// Set `RenderCustomDepth` to be false so that the object isn't highlighted anymore.
			ObjectCurrentlyBeingLookedAt->SetRenderCustomDepth(false);

			// Set the NoticeText to be invisible since the player can no longer interact with
			// with this object anymore.
			NoticeText->SetVisibility(false);
		}

		// Lastly since the PlayerCharacter is no longer looking at a valid interactive object
		// then we set `ObjectCurrentlyBeingLookedAt` to a nullptr and return.
		ObjectCurrentlyBeingLookedAt = nullptr;
		return;
	}

	// Loop through all of the objects marked as interactive and if the player
	// is currently looking at one of them then highlight it and set it as the
	// new value for `ObjectCurrentlyBeingLookedAt`.
	for (UStaticMeshComponent* InteractiveObject : InteractiveObjects)
	{
		if (InteractiveObject == InteractiveComponentHit)
		{
			// Set the `InteractiveComponentHit` to be the new value of `ObjectCurrentlyBeingLookedAt`
			// since that's what the PlayerCharacter is currently looking at.
			ObjectCurrentlyBeingLookedAt = InteractiveComponentHit;

			// Set `RenderCustomDepth` to be true so that the highlight is
			// added to the object.
			ObjectCurrentlyBeingLookedAt->SetRenderCustomDepth(true);

			// Set the NoticeText to be visible so the player knows that they can interact with it.
			NoticeText->SetVisibility(true);

			// Break out of the loop early since we only want to deal with one
			// interactive object at a time.
			break;
		}
	}
}

/**
 * When this Actor is interacted with we check to see what object it is and
 * perform the interaction for that object.
 */
void AInteractiveActor::Interact()
{
}
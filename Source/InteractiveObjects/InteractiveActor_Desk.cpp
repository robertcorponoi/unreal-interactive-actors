#include "InteractiveActor_Desk.h"
#include "Components/PointLightComponent.h"

/**
 * Sets default values.
 */
AInteractiveActor_Desk::AInteractiveActor_Desk()
{
	// Load the meshes used for all of the components.
	static ConstructorHelpers::FObjectFinder<UStaticMesh>DeskMeshAsset(TEXT("StaticMesh'/Game/Models/Furniture/Desk/desk_desk.desk_desk'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>DeskDrawerMeshAsset(TEXT("StaticMesh'/Game/Models/Furniture/Desk/desk_drawer.desk_drawer'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>ChairSeatMeshAsset(TEXT("StaticMesh'/Game/Models/Furniture/Chair/chair_chair.chair_chair'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>ChairBaseMeshAsset(TEXT("StaticMesh'/Game/Models/Furniture/Chair/chair_chairDesk.chair_chairDesk'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LampMeshAsset(TEXT("StaticMesh'/Game/Models/Furniture/Lamp/lampRoundTable.lampRoundTable'"));

	// Create the component for the Desk, set its mesh and position and then
	// attach it to the Root.
	Desk = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Desk"));
	Desk->SetRelativeLocationAndRotation(FVector(0.f, 110.f, -40.f), FRotator(0.f, 90.f, 0.f));
	Desk->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	Desk->SetStaticMesh(DeskMeshAsset.Object);
	Desk->SetupAttachment(Root);

	// Create the component for the Drawer, set its mesh and position and
	// attach it to the Desk.
	DeskDrawer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeskDrawer"));
	DeskDrawer->SetStaticMesh(DeskDrawerMeshAsset.Object);
	DeskDrawer->SetupAttachment(Desk);

	// Create the components for the Chair, set its mesh and position and
	// attach it to the Desk.
	ChairSeat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairSeat"));
	ChairSeat->SetStaticMesh(ChairSeatMeshAsset.Object);
	ChairSeat->SetRelativeLocationAndRotation(FVector(-95.f, -40.f, 45.f), FRotator(0.f, 180.f, 0.f));
	ChairSeat->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	ChairSeat->SetupAttachment(Desk);

	ChairBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairBase"));
	ChairBase->SetStaticMesh(ChairBaseMeshAsset.Object);
	ChairBase->SetRelativeLocation(FVector(-95.f, -40.f, 0.f));
	ChairBase->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	ChairBase->SetupAttachment(Desk);

	// Create the component for the Lamp, set its mesh and position and
	// attach it to the Desk.
	Lamp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lamp"));
	Lamp->SetStaticMesh(LampMeshAsset.Object);
	Lamp->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	Lamp->SetRelativeLocation(FVector(-40.f, 20.f, 57.f));
	Lamp->SetupAttachment(Desk);

	// Create the PointLightComponent for the Lamp.
	LampLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LampLight"));
	LampLight->SetRelativeLocation(FVector(-9.f, 9.f, 28.f));
	LampLight->SetIntensity(3000.f);
	LampLight->SetupAttachment(Lamp);
	LampLight->SetVisibility(false);
}

/**
 * Called when the game starts or when spawned.
 */
void AInteractiveActor_Desk::BeginPlay()
{
	Super::BeginPlay();

	// Add the DeskDrawer, ChairSeat, and Lamp to the `InteractiveObjects` array.
	InteractiveObjects.Add(DeskDrawer);
	InteractiveObjects.Add(Lamp);
}

/**
 * When this Actor is interacted with we check to see what object it is and
 * perform the interaction for that object.
 */
void AInteractiveActor_Desk::Interact()
{
	Super::Interact();

	if (ObjectCurrentlyBeingLookedAt == nullptr) return;

	// Otherwise if you have multiple interactive objects you can check their name
	// and perform a different action for each one.
	FString InteractiveComponentName = ObjectCurrentlyBeingLookedAt->GetName();

	if (InteractiveComponentName == FString("DeskDrawer"))
	{
		// If the PlayerCharacter is interacting with the desk drawer then we want to
		// move the desk drawer to that it looks like it was opened.
		FVector CurrentLocation = ObjectCurrentlyBeingLookedAt->GetRelativeLocation();
		if (CurrentLocation.Y == 0.f)
		{
			ObjectCurrentlyBeingLookedAt->SetRelativeLocation(FVector(CurrentLocation.X, -20.0f, CurrentLocation.Z));
		}
		else
		{
			ObjectCurrentlyBeingLookedAt->SetRelativeLocation(FVector(CurrentLocation.X, 0.0f, CurrentLocation.Z));
		}
	}
	else if (InteractiveComponentName == FString("Lamp"))
	{
		// If the PlayerCharacter is interacting with the lamp then we want to turn on
		// the light.
		LampLight->ToggleVisibility();
	}
}
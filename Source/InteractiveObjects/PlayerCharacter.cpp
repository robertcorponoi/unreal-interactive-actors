#include "PlayerCharacter.h"
#include "InteractiveActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h" 
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get the player's skeletal mesh and animation blueprint.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("SkeletalMesh'/Game/Models/XBot/xbot.xbot'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint>AnimBP(TEXT("AnimBlueprint'/Game/Blueprints/BP_PlayerAnim.BP_PlayerAnim'"));

	// Set the skeletal mesh and animation blueprint from above and set the
	// correct location and rotation for the mesh since it wasn't exported
	// with Unreal's X forward vector.
	PlayerSkeletalMesh = GetMesh();
	PlayerSkeletalMesh->SetSkeletalMesh(MeshAsset.Object);
	PlayerSkeletalMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	PlayerSkeletalMesh->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);

	// Configure character movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Attach the SpringArmComponent to the player and set its default values.
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 80.f));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 300.0f;
	CameraSpringArm->bUsePawnControlRotation = true;

	// Attach the CameraComponent to the SpringArmComponent and set its default
	// position and rotation.
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	// Set this player to be used when the game is started.
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

/**
 * Called when the game starts or when spawned.
 */
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Called every frame.
 *
 * @param DeltaTime The time between the previous Tick and the current one.
 */
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Called to bind functionality to input.
 *
 * @param PlayerInputComponent An Actor component that enables us to bind axis events and action inputs to methods.
 */
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// When the mouse is moved we turn the player.
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpDown", this, &APlayerCharacter::AddControllerPitchInput);

	// When the "Move_ForwardBack" or "Move_LeftRight" axis is used we call the methods to make
	// that happen.
	PlayerInputComponent->BindAxis("Move_ForwardBack", this, &APlayerCharacter::MoveForwardBack);
	PlayerInputComponent->BindAxis("Move_LeftRight", this, &APlayerCharacter::MoveLeftRight);

	// When the "Interact" action is used we call the `Interact` method.
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
}

/**
 * When the player moves forwards or backwards we add forward movement
 * multiplied by the axis value. A positive axis value will result in
 * forward movement and vice-versa.
 *
 * @param Value The input axis value.
 */
void APlayerCharacter::MoveForwardBack(float Value)
{
	APlayerCharacter::AddMovementInput(GetActorForwardVector(), Value);
}

/**
 * When the player moves left or right we add movement to the right
 * multiplied by the axis value. A positive axis value will result in
 * movement to the right and vice-versa.
 *
 * @param Value The input axis value.
 */
void APlayerCharacter::MoveLeftRight(float Value)
{
	APlayerCharacter::AddMovementInput(GetActorRightVector(), Value);
}

/**
 * When the player presses the interact action button we check to see
 * if `InteractionArea` is populated. If it's not populated then we return
 * as there's nothing to interact with otherwise we call that InteractiveActor's
 * `Interact` method.
 */
void APlayerCharacter::Interact()
{
	if (InteractionArea == nullptr) return;
	InteractionArea->Interact();
}
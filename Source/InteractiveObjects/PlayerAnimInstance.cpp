#include "PlayerAnimInstance.h"
#include "Math/Rotator.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/**
 * Updates the variables used by the "PlayerAnim_BP" Blueprint to
 * decide what animations to play.
 */
void UPlayerAnimInstance::UpdateAnimationProperties()
{
	// Try to get the Pawn being animated and return if null.
	APawn* PlayerPawn = TryGetPawnOwner();
	if (!PlayerPawn) return;

	// Try to cast the Pawn to our PlayerCharacter since that's the only
	// thing we want to animate.
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	if (PlayerCharacter == nullptr) return;

	// Get the PlayerCharacter's normalized velocity.
	UCharacterMovementComponent* PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
	FVector PlayerCharacterMovementVelocity = PlayerCharacterMovement->Velocity;
	PlayerCharacterMovementVelocity.Normalize();

	// Get the PlayerCharacter's CapsuleComponent rotation and un-rotate it.
	UCapsuleComponent* PlayerCapsule = PlayerCharacter->GetCapsuleComponent();
	FRotator PlayerCapsuleRotation = PlayerCapsule->GetComponentRotation();
	FVector PlayerVelocityUnrotated = PlayerCapsuleRotation.UnrotateVector(PlayerCharacterMovementVelocity);

	// Set the animation properties based on the unrotated velocity.
	bIsMovingForward = PlayerVelocityUnrotated.X > 0.1;
	bIsMovingBackward = PlayerVelocityUnrotated.X < -0.1;
	bIsMovingRight = PlayerVelocityUnrotated.Y > 0.8;
	bIsMovingLeft = PlayerVelocityUnrotated.Y < -0.8;
}
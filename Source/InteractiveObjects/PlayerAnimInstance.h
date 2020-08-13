#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * Handles updating the properties used by the PlayerAnim_BP Blueprint
 * to decide what animations to play.
 */
UCLASS()
class INTERACTIVEOBJECTS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Indicates whether the PlayerCharacter is moving forward or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMovingForward;

	// Indicates whether the PlayerCharacter is moving backward or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMovingBackward;

	// Indicates whether the PlayerCharacter is strafing to the left or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMovingLeft;

	// Indicates whether the PlayerCharacter is strafing to the right or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMovingRight;

	// Used by the PlayerAnim_BP Blueprint to update the animation properties
	// and decide what animations to play.
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimationProperties();
};

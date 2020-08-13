#pragma once

#include "CoreMinimal.h"
#include "InteractiveActor.h"
#include "InteractiveActor_Desk.generated.h"

/**
 * Creates an interactive desk with a drawer.
 */
UCLASS()
class INTERACTIVEOBJECTS_API AInteractiveActor_Desk : public AInteractiveActor
{
	GENERATED_BODY()
	
public:
	// Sets up the default values.
	AInteractiveActor_Desk();

	// A non-interactive desk.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Desk;

	// An interactive drawer in the desk that the PlayerCharacter can interact with.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* DeskDrawer;

	// A non-interactive chair base.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ChairBase;

	// A non-interactive chair seat.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ChairSeat;

	// An interactive lamp.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Lamp;

	// The light for the lamp.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPointLightComponent* LampLight;

	// The timer handle for updating the chair rotation.
	FTimerHandle ChairTimerHandle;

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:
	// Called when the PlayerCharacter interacts with an object in this Actor.
	virtual void Interact() override;
};

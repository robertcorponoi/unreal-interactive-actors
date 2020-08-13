#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveActor.generated.h"

class UStaticMeshComponent;

/**
 * Represents a series of interactive objects that the PlayerCharacter can
 * interact with.
 */
UCLASS()
class INTERACTIVEOBJECTS_API AInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties.
	AInteractiveActor();

	// The RootComponent of this Actor.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Root;

	//// A non-interactive desk.
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//UStaticMeshComponent* Desk;

	//// An interactive drawer in the desk that the PlayerCharacter can interact with.
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//UStaticMeshComponent* DeskDrawer;

	// The BoxComponent used as a collision trigger that lets us know when the
	// PlayerCharacter is close enough to interact with objects in this Actor.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxCollider;

	// The TextRenderComponent that appears above this Actor when the PlayerCharacter
	// is able to interact with a part of it.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UTextRenderComponent* NoticeText;

	// The StaticMeshComponents that the PlayerCharacter can interact with.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UStaticMeshComponent*> InteractiveObjects;

	// The interactive StaticMeshComponent currently being looked at.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ObjectCurrentlyBeingLookedAt;

	// Used to create a timer that runs a function every so often to check if
	// PlayerCharacter is looking at an object that can be interacted with or not.
	UPROPERTY()
	FTimerHandle PlayerLookTimer;

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Called when the PlayerCharacter enters the BoxCollider's trigger area.
	UFUNCTION()
	void OnComponentEnterInteractionArea(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the PlayerCharacter leaves the BoxCollider's trigger area.
	UFUNCTION()
	void OnComponentLeaveInteractionArea(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called when the PlayerCharacter interacts with an object in this Actor.
	virtual void Interact();

	// Called when the PlayerCharacter has entered the BoxCollider's trigger
	// area and is looking around for objects to interact with.
	UFUNCTION()
	void LookForInteractions(APlayerCharacter* PlayerCharacter);
};

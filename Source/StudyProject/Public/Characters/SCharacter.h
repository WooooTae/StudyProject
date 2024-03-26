//SCharacter.h.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class STUDYPROJECT_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "ASTPSCharacter",Meta = (AllowPrivateAccess))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "ASTPSCharacter",Meta = (AllowPrivateAccess))
	TObjectPtr<class UCameraComponent> CameraComponent;
};

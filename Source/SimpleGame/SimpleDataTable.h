#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SimpleDataTable.generated.h"

USTRUCT(BlueprintType)
struct FSimpleDataTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
    int32 ID; // row name
	UPROPERTY(EditDefaultsOnly) FName Code;
	UPROPERTY(EditDefaultsOnly) FName UserName;
	UPROPERTY(EditDefaultsOnly) int32 Age;
	UPROPERTY(EditDefaultsOnly) int32 Job;
};
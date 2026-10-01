// Paridyz

#pragma once

#include "CoreMinimal.h"
#include "Graph/EdGraphNode_RarityPollItem.h"
#include "EditorUtilityWidget.h"
#include "RarityPoolNodeWidget.generated.h"

UCLASS()
class RARITYPOOLEDITOR_API URarityPoolNodeWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateWidget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEdGraphNode_RarityPollItem* Node;
};

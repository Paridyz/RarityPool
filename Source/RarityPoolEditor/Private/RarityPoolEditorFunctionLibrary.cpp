// Paridyz


#include "RarityPoolEditorFunctionLibrary.h"

#include "ClassIconFinder.h"

FSlateBrush URarityPoolEditorFunctionLibrary::GetClassThumbnail(UClass* Class)
{
	return *FClassIconFinder::FindThumbnailForClass(Class, NAME_None);
}
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**  */
class FMinesweeperPluginStyle
{
public:

    static void Initialize();

    static void Shutdown();

    /** reloads textures used by slate renderer */
    static void ReloadTextures();

    static const ISlateStyle& Get();
    static const TSharedPtr<class FSlateStyleSet> GetPtr();

    static FName GetStyleSetName();

private:

    static TSharedRef<class FSlateStyleSet> Create();

private:

    static TSharedPtr<class FSlateStyleSet> StyleInstance;
};
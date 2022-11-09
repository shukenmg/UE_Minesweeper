// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperPluginStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMinesweeperPluginStyle::StyleInstance = nullptr;

void FMinesweeperPluginStyle::Initialize()
{
    if (!StyleInstance.IsValid())
    {
        StyleInstance = Create();
        FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
    }
}

void FMinesweeperPluginStyle::Shutdown()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
    ensure(StyleInstance.IsUnique());
    StyleInstance.Reset();
}

FName FMinesweeperPluginStyle::GetStyleSetName()
{
    static FName StyleSetName(TEXT("MinesweeperPluginStyle"));
    return StyleSetName;
}

TSharedRef<FSlateStyleSet> FMinesweeperPluginStyle::Create()
{
    const FVector2D Icon16x16(16.0f, 16.0f);
    const FVector2D Icon20x20(20.0f, 20.0f);
    const FVector2D Icon76x76(76.0f, 76.0f);

    TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("MinesweeperPluginStyle"));
    Style->SetContentRoot(IPluginManager::Get().FindPlugin("MinesweeperPlugin")->GetBaseDir() / TEXT("Resources"));

    Style->Set("MinesweeperPluginStyle.UIText", FCoreStyle::GetDefaultFontStyle("Regular", 12));
    Style->Set("MinesweeperPluginStyle.BoardText", FCoreStyle::GetDefaultFontStyle("Bold", 40));

    Style->Set("MinesweeperPlugin.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("icon"), Icon76x76));

    Style->Set("MinesweeperPluginStyle.Cell.Covered", new IMAGE_BRUSH(TEXT("tile000"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Uncovered", new IMAGE_BRUSH(TEXT("tile001"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Flagged", new IMAGE_BRUSH(TEXT("tile002"), Icon16x16));

    Style->Set("MinesweeperPluginStyle.Cell.Bomb", new IMAGE_BRUSH(TEXT("tile005"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Exploded", new IMAGE_BRUSH(TEXT("tile006"), Icon16x16));

    Style->Set("MinesweeperPluginStyle.Cell.Neighbor.0", new IMAGE_BRUSH(TEXT("tile007"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Neighbor.1", new IMAGE_BRUSH(TEXT("tile008"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Neighbor.2", new IMAGE_BRUSH(TEXT("tile009"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Neighbor.3", new IMAGE_BRUSH(TEXT("tile010"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Neighbor.4", new IMAGE_BRUSH(TEXT("tile011"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Neighbor.5", new IMAGE_BRUSH(TEXT("tile012"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Neighbor.6", new IMAGE_BRUSH(TEXT("tile013"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Neighbor.7", new IMAGE_BRUSH(TEXT("tile014"), Icon16x16));
    Style->Set("MinesweeperPluginStyle.Cell.Neighbor.8", new IMAGE_BRUSH(TEXT("tile015"), Icon16x16));

    return Style;
}

void FMinesweeperPluginStyle::ReloadTextures()
{
    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
    }
}

const ISlateStyle& FMinesweeperPluginStyle::Get()
{
    return *StyleInstance;
}

const TSharedPtr<class FSlateStyleSet> FMinesweeperPluginStyle::GetPtr()
{
    return StyleInstance;
}

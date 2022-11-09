// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperPluginModule.h"
#include "MinesweeperPluginCommands.h"
#include "MinesweeperPluginGameData.h"
#include "MinesweeperPluginGameManager.h"
#include "MinesweeperPluginGameView.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "FMinesweeperPluginModule"

FMinesweeperPluginModule& FMinesweeperPluginModule::Get()
{
    return FModuleManager::GetModuleChecked<FMinesweeperPluginModule>("MinesweeperPlugin");
}

FMinesweeperPluginModule* FMinesweeperPluginModule::GetPtr()
{
    return static_cast<FMinesweeperPluginModule*>(FModuleManager::Get().GetModule("MinesweeperPlugin"));
}

void FMinesweeperPluginModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    FMinesweeperPluginStyle::Initialize();
    FMinesweeperPluginStyle::ReloadTextures();

    FMinesweeperPluginCommands::Register();
}

void FMinesweeperPluginModule::ShutdownModule()
{
    FMinesweeperPluginStyle::Shutdown();

    FMinesweeperPluginCommands::Unregister();

    if (GameView.IsValid())
    {
        GameView->Destroy();
    }
}

void FMinesweeperPluginModule::OnToolbarButtonPressed()
{
    // Get a reference to this plugin
    FMinesweeperPluginModule& Module = FMinesweeperPluginModule::Get();

    // Setup plugin and only setup when needed, to save memory.
    if (!Module.GameManager.IsValid())
    {
        Module.GameData = MakeShared<FMinesweeperPluginGameData>();
        Module.GameView = MakeShared<FMinesweeperPluginGameView>();
        Module.GameManager = MakeShared<FMinesweeperPluginGameManager>();

        Module.GameView->Init();
    }

    Module.GameManager->OnToolbarButtonPressed();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMinesweeperPluginModule, MinesweeperPlugin)
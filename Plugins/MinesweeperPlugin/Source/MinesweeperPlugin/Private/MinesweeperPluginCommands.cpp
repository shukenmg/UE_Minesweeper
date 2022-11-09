// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperPluginCommands.h"
#include "MinesweeperPluginModule.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "Styling/SlateStyle.h"

#define LOCTEXT_NAMESPACE "FMinesweeperPluginModule"

void FMinesweeperPluginCommands::RegisterCommands()
{
    // Check if this has already been setup
    if (PluginCommands.IsValid())
    {
        return;
    }

    UI_COMMAND(OpenPluginWindow, "Minesweeper", "Brings up the Minesweeper window", EUserInterfaceActionType::Button, FInputChord());

    {
        // Add Commands to the editor
        PluginCommands = MakeShareable(new FUICommandList);

        FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
        PluginCommands->Append(LevelEditorModule.GetGlobalLevelEditorActions());

        PluginCommands->MapAction(
            OpenPluginWindow,
            FExecuteAction::CreateStatic(&FMinesweeperPluginModule::OnToolbarButtonPressed),
            FCanExecuteAction());

        // Set owner for auto cleanup
        FToolMenuOwnerScoped OwnerScoped(this);

        {
            UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
            {
                FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
                {
                    FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperPluginCommands::Get().OpenPluginWindow));
                    Entry.SetCommandList(PluginCommands);
                }
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE

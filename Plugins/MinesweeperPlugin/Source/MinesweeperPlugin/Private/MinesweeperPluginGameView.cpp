// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperPluginGameView.h"
#include "MinesweeperPluginGameManager.h"
#include "MinesweeperPluginModule.h"
#include "MinesweeperPluginBoardCell.h"
#include "MinesweeperPluginTab.h"

#include "Widgets/SCanvas.h"

const float FMinesweeperPluginGameView::CellSize = 32;

FMinesweeperPluginGameView::FMinesweeperPluginGameView()
{
    MinesweeperBoardWidth = 10;
    MinesweeperBoardHeight = 10;
    MinesweeperBombs = 10;

    MouseDownInput = EKeys::Invalid;
    MouseDownSelectedCell.Position = FVector2D(-1);
}

void FMinesweeperPluginGameView::Init()
{
    // Register the tab spawner.
    TSharedRef<FGlobalTabmanager> GlobalTabmanager = FGlobalTabmanager::Get();
    GlobalTabmanager->RegisterTabSpawner(SMinesweeperPluginTab::MinesweeperPluginTabName, FOnSpawnTab::CreateRaw(this, &FMinesweeperPluginGameView::CreateMinesweeperTab));
}

void FMinesweeperPluginGameView::Destroy()
{
    // Demolish everything, muahahaha
    TSharedRef<FGlobalTabmanager> GlobalTabmanager = FGlobalTabmanager::Get();
    GlobalTabmanager->UnregisterTabSpawner(SMinesweeperPluginTab::MinesweeperPluginTabName);

    // Politely tho
    if (Tab.IsValid())
    {
        Tab->RequestCloseTab();
    }
}

void FMinesweeperPluginGameView::ShowTab()
{
    TSharedRef<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
    Tab = StaticCastSharedPtr<SMinesweeperPluginTab>(TabManager->TryInvokeTab(SMinesweeperPluginTab::MinesweeperPluginTabName));

    if (!Tab.IsValid())
    {
        return;
    }

    // Show the user the board
    PopulateMinesweeperBoard();
}

void FMinesweeperPluginGameView::OnTabClosed(TSharedRef<SDockTab> DockTab)
{
    // Reset tab to make it close
    Tab.Reset();
}

TSharedRef<SDockTab> FMinesweeperPluginGameView::CreateMinesweeperTab(const FSpawnTabArgs& TabSpawnArgs)
{
    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Return blank page if error
    if (!PluginModule)
    {
        return SNew(SDockTab);
    }

    // Add the custom tab class to the window
    TSharedRef<SMinesweeperPluginTab> NewTab = SNew(SMinesweeperPluginTab).GameView(PluginModule->GetGameView());

    if (NewTab->GetStartNewGameButton().IsValid())
    {
        NewTab->GetStartNewGameButton()->SetOnClicked(FOnClicked::CreateSP(this, &FMinesweeperPluginGameView::OnNewGameButtonPressed));
    }

    if (NewTab->GetMinesweeperBoard().IsValid())
    {
        // Bind to button clicks on the SCanvas
        NewTab->GetMinesweeperBoard()->SetOnMouseButtonDown(FPointerEventHandler::CreateSP(this, &FMinesweeperPluginGameView::OnMinesweeperMouseButtonDown));
        NewTab->GetMinesweeperBoard()->SetOnMouseButtonUp(FPointerEventHandler::CreateSP(this, &FMinesweeperPluginGameView::OnMinesweeperMouseButtonUp));
    }

    return NewTab;
}

FCellData FMinesweeperPluginGameView::MouseEventToCellPosition(const FGeometry& Geometry, const FPointerEvent& Event)
{
    FVector2D MouseLocationOnWidget = Event.GetScreenSpacePosition() - Geometry.GetAbsolutePosition();
    MouseLocationOnWidget /= Geometry.Scale;

    uint8 CellX = MouseLocationOnWidget.X / FMinesweeperPluginGameView::CellSize;
    uint8 CellY = MouseLocationOnWidget.Y / FMinesweeperPluginGameView::CellSize;

    // Convert X/Y to CellData
    FCellData NewCellData = FCellData::DefaultValue();
    NewCellData.Position.X = CellX;
    NewCellData.Position.Y = CellY;

    return NewCellData;
}

// Clamp to stop invalid amounts
uint16 FMinesweeperPluginGameView::GetMinesweeperBombs() const
{
    return (uint16)FMath::Clamp((int)MinesweeperBombs, 1, (MinesweeperBoardWidth * MinesweeperBoardWidth) / 2);
}

void FMinesweeperPluginGameView::SetMinesweeperBoardWidth(uint8 NewVal)
{
    MinesweeperBoardWidth = NewVal;
}

void FMinesweeperPluginGameView::SetMinesweeperBoardHeight(uint8 NewVal)
{
    MinesweeperBoardHeight = NewVal;
}

void FMinesweeperPluginGameView::SetMinesweeperBombs(uint16 NewVal)
{
    MinesweeperBombs = NewVal;
}

void FMinesweeperPluginGameView::OnMinesweeperBoardWidthSliderValueChanged(uint8 NewVal)
{
    MinesweeperBoardWidth = NewVal;
}

void FMinesweeperPluginGameView::OnMinesweeperBoardHeightSliderValueChanged(uint8 NewVal)
{
    MinesweeperBoardHeight = NewVal;
}

void FMinesweeperPluginGameView::OnMinesweeperBombsSliderValueChanged(uint16 NewVal)
{
    MinesweeperBombs = NewVal;
}

bool FMinesweeperPluginGameView::PopulateMinesweeperBoard()
{
    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid())
    {
        return false;
    }

    // More valid checks
    if (!Tab.IsValid() || !Tab->GetMinesweeperBoardContainer().IsValid() || !Tab->GetMinesweeperBoard().IsValid())
    {
        return false;
    }

    // Resize board (SBox) to fit
    Tab->GetMinesweeperBoardContainer()->SetWidthOverride(PluginModule->GetGameData()->GetMinesweeperBoardWidth() * FMinesweeperPluginGameView::CellSize);
    Tab->GetMinesweeperBoardContainer()->SetHeightOverride(PluginModule->GetGameData()->GetMinesweeperBoardHeight() * FMinesweeperPluginGameView::CellSize);

    // Reset board
    Tab->GetMinesweeperBoard()->ClearChildren();

    // Fill board with new CellDatas
    for (uint8 HeightIndex = 0; HeightIndex < PluginModule->GetGameData()->GetMinesweeperBoardHeight(); ++HeightIndex)
    {
        for (uint8 WidthIndex = 0; WidthIndex < PluginModule->GetGameData()->GetMinesweeperBoardWidth(); ++WidthIndex)
        {
            // Convert Width/Height to CellData
            FCellData NewCellData = FCellData::DefaultValue();
            NewCellData.Position.X = WidthIndex;
            NewCellData.Position.Y = HeightIndex;

            Tab->GetMinesweeperBoard()->AddSlot()
                .Position(FVector2D(WidthIndex * FMinesweeperPluginGameView::CellSize, HeightIndex * FMinesweeperPluginGameView::CellSize))
                .Size(FVector2D(FMinesweeperPluginGameView::CellSize, FMinesweeperPluginGameView::CellSize))
                [
                    SNew(SMinesweeperPluginBoardCell).GameData(PluginModule->GetGameData()).CellData(NewCellData)
                ];
        }
    }

    // Update all cells
    UpdateMinesweeperBoardDisplay();

    return true;
}

void FMinesweeperPluginGameView::UpdateMinesweeperBoardDisplay()
{
    if (!Tab.IsValid() || !Tab->GetMinesweeperBoard().IsValid())
    {
        return;
    }

    // Get all the cells
    FChildren* Cells = Tab->GetMinesweeperBoard()->GetChildren();

    // Update them
    for (int Index = 0; Index < Cells->Num(); ++Index)
    {
        TSharedRef<SMinesweeperPluginBoardCell> Cell = StaticCastSharedRef<SMinesweeperPluginBoardCell>(Cells->GetChildAt(Index));

        Cell->Update();
    }
}

void FMinesweeperPluginGameView::UpdateCellImage(const FCellData& CellData)
{
    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameData()->IsValidCellData(CellData))
    {
        return;
    }

    // Valid checks
    if (!Tab.IsValid() || !Tab->GetMinesweeperBoard().IsValid())
    {
        return;
    }

    // Get index of cell in container
    int Index = PluginModule->GetGameData()->GetCellIndex(CellData);
    FChildren* Cells = Tab->GetMinesweeperBoard()->GetChildren();
    TSharedRef<SMinesweeperPluginBoardCell> Child = StaticCastSharedRef<SMinesweeperPluginBoardCell>(Cells->GetChildAt(Index));

    // Update image
    Child->Update();
}

FReply FMinesweeperPluginGameView::OnNewGameButtonPressed()
{
    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    if (!PluginModule)
    {
        return FReply::Handled();
    }

    if (!PluginModule->GetGameManager().IsValid())
    {
        return FReply::Handled();
    }


    PluginModule->GetGameManager()->StartNewGame(GetMinesweeperBoardWidth(), GetMinesweeperBoardHeight(), GetMinesweeperBombs());

    return FReply::Handled();
}

FReply FMinesweeperPluginGameView::OnMinesweeperMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& Event)
{
    // Check for mouse weirdness
    if (Event.GetEffectingButton() != EKeys::LeftMouseButton && Event.GetEffectingButton() != EKeys::RightMouseButton)
    {
        MouseDownInput = EKeys::Invalid;
    }
    else if (MouseDownInput != EKeys::Invalid)
    {
        return FReply::Handled();
    }

    MouseDownInput = Event.GetEffectingButton();
    MouseDownSelectedCell = FMinesweeperPluginGameView::MouseEventToCellPosition(Geometry, Event);

    return FReply::Handled();
}

FReply FMinesweeperPluginGameView::OnMinesweeperMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& Event)
{
    // Check for mouse weirdness
    if (Event.GetEffectingButton() != EKeys::LeftMouseButton && Event.GetEffectingButton() != EKeys::RightMouseButton)
    {
        return FReply::Handled();
    }
    else if (MouseDownInput != Event.GetEffectingButton()) // Check if button pressed is still the same
    {
        return FReply::Handled();
    }

    FCellData MouseUpSelectedCell = FMinesweeperPluginGameView::MouseEventToCellPosition(Geometry, Event);

    if (MouseUpSelectedCell.Position.X == MouseDownSelectedCell.Position.X && MouseUpSelectedCell.Position.Y == MouseDownSelectedCell.Position.Y)
    {
        OnMinesweeperBoardPressed(Event.GetEffectingButton(), MouseUpSelectedCell);
    }

    // Reset click
    MouseDownInput = EKeys::Invalid;

    return FReply::Handled();
}

void FMinesweeperPluginGameView::OnMinesweeperBoardPressed(FKey MouseButton, const FCellData& Cell)
{
    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameManager().IsValid())
    {
        return;
    }

    // Valid checks
    if (!PluginModule->GetGameData()->IsValidCellData(Cell))
    {
        return;
    }

    // Input actions
    if (MouseButton == EKeys::LeftMouseButton)
    {
        PluginModule->GetGameManager()->UncoverCell(Cell);
    }
    else if (MouseButton == EKeys::RightMouseButton)
    {
        PluginModule->GetGameManager()->FlagCell(Cell);
    }
}

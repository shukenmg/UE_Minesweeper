// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperPluginGameManager.h"
#include "MinesweeperPluginGameView.h"
#include "MinesweeperPluginModule.h"

#include "Kismet/KismetArrayLibrary.h"

#define LOCTEXT_NAMESPACE "FMinesweeperPluginModule"

void FMinesweeperPluginGameManager::Init()
{
    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameView().IsValid())
    {
        DisableGameplay();
        return;
    }
}

void FMinesweeperPluginGameManager::OnToolbarButtonPressed()
{
    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameView().IsValid())
    {
        return;
    }

    // Start game if not started already
    if (GameState == EGameStates::NotPlaying)
    {
        PluginModule->GetGameView()->SetMinesweeperBoardWidth(DEFAULT_BOARD_WIDTH);
        PluginModule->GetGameView()->SetMinesweeperBoardHeight(DEFAULT_BOARD_HEIGHT);
        PluginModule->GetGameView()->SetMinesweeperBombs(DEFAULT_BOMB_COUNT);

        StartNewGame(
            PluginModule->GetGameView()->GetMinesweeperBoardWidth(),
            PluginModule->GetGameView()->GetMinesweeperBoardHeight(),
            PluginModule->GetGameView()->GetMinesweeperBombs()
        );
    }

    // We need to actually show the tab after starting the game for the first time
    PluginModule->GetGameView()->ShowTab();
}

void FMinesweeperPluginGameManager::StartNewGame(uint8 Width, uint8 Height, uint16 Bombs)
{
    if (GameState == EGameStates::Disabled)
    {
        return;
    }

    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameView().IsValid())
    {
        return;
    }

    // Setup bombs
    TArray<bool> Cells;
    Cells.SetNum(Width * Height);
    DistributeBombs(Cells, Bombs);

    // Setup the game board
    PluginModule->GetGameData()->InitMinesweeper(Width, Height, Cells);
    PluginModule->GetGameView()->PopulateMinesweeperBoard();
    GameState = EGameStates::Playing;
}

// use a simple array shuffle to distibute the bombs
void FMinesweeperPluginGameManager::DistributeBombs(TArray<bool>& Cells, uint16 Bombs)
{
    for (uint16 i = 0; i < Bombs; i++)
    {
        Cells[i] = true;
    }

    // Shuffle Algo from: https://gist.github.com/KaosSpectrum/520b1521c898801c21f7ba3f90d70fcc
    const int32 LastIndex = Cells.Num() - 1;
    for (int32 i = 0; i < LastIndex; ++i)
    {
        int32 Index = FMath::RandRange(0, LastIndex);
        if (i != Index)
        {
            Cells.Swap(i, Index);
        }
    }
}

void FMinesweeperPluginGameManager::UncoverCell(const FCellData& Cell)
{
    if (GameState != EGameStates::Playing)
    {
        return;
    }

    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameView().IsValid())
    {
        DisableGameplay();
        return;
    }

    const FCellData& CellData = PluginModule->GetGameData()->GetCellData(Cell.Position);

    if (!PluginModule->GetGameData()->IsValidCellData(CellData))
    {
        return;
    }

    if (CellData.HasBomb)
    {
        PluginModule->GetGameData()->ExplodeCell(CellData);
        EndGameplay(EGameStates::Dead);
        return;
    }

    UncoverCellRecursive(Cell);
}

void FMinesweeperPluginGameManager::UncoverCellRecursive(const FCellData& Cell)
{
    if (GameState != EGameStates::Playing)
    {
        return;
    }

    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameView().IsValid())
    {
        DisableGameplay();
        return;
    }

    const FCellData& CellData = PluginModule->GetGameData()->GetCellData(Cell.Position);

    if (!PluginModule->GetGameData()->IsValidCellData(CellData))
    {
        return;
    }

    if (CellData.HasBomb)
    {
        return;
    }

    // Uncover and update cell
    if (!PluginModule->GetGameData()->UncoverCell(CellData))
    {
        return;
    }

    PluginModule->GetGameView()->UpdateCellImage(CellData);

    // Win Condition
    if ((PluginModule->GetGameData()->GetBombs() +
        PluginModule->GetGameData()->GetNumberOfCellsRevealed()) >= PluginModule->GetGameData()->GetMinesweeperBoardSize())
    {
        EndGameplay(EGameStates::Win);
        return;
    }

    // Return if has Neighboring Bombs
    if (CellData.NeighboringBombs > 0)
    {
        return;
    }

    FCellData NewCellData = FCellData::DefaultValue();

    // uncover each adjacent cell if valid
    // top
    if (CellData.Position.Y > 0)
    {
        NewCellData = CellData;
        NewCellData.Position.Y -= 1;
        UncoverCellRecursive(NewCellData);
    }

    // down
    if (CellData.Position.Y < (PluginModule->GetGameData()->GetMinesweeperBoardHeight() - 1))
    {
        NewCellData = CellData;
        NewCellData.Position.Y += 1;
        UncoverCellRecursive(NewCellData);
    }

    // left
    if (CellData.Position.X > 0)
    {
        NewCellData = CellData;
        NewCellData.Position.X -= 1;
        UncoverCellRecursive(NewCellData);
    }

    // right
    if (CellData.Position.X < (PluginModule->GetGameData()->GetMinesweeperBoardWidth() - 1))
    {
        NewCellData = CellData;
        NewCellData.Position.X += 1;
        UncoverCellRecursive(NewCellData);
    }

    // top left
    if (CellData.Position.X > 0 && CellData.Position.Y > 0)
    {
        NewCellData = CellData;
        NewCellData.Position.X -= 1;
        NewCellData.Position.Y -= 1;
        UncoverCellRecursive(NewCellData);
    }

    // top right
    if (CellData.Position.X < (PluginModule->GetGameData()->GetMinesweeperBoardWidth() - 1) && CellData.Position.Y> 0)
    {
        NewCellData = CellData;
        NewCellData.Position.X += 1;
        NewCellData.Position.Y -= 1;
        UncoverCellRecursive(NewCellData);
    }

    // bottom left (dum)
    if (CellData.Position.X > 0 && CellData.Position.Y < (PluginModule->GetGameData()->GetMinesweeperBoardHeight() - 1))
    {
        NewCellData = CellData;
        NewCellData.Position.X -= 1;
        NewCellData.Position.Y += 1;
        UncoverCellRecursive(NewCellData);
    }

    // bottom right
    if (CellData.Position.X < (PluginModule->GetGameData()->GetMinesweeperBoardWidth() - 1) && CellData.Position.Y < (PluginModule->GetGameData()->GetMinesweeperBoardHeight() - 1))
    {
        NewCellData = CellData;
        NewCellData.Position.X += 1;
        NewCellData.Position.Y += 1;
        UncoverCellRecursive(NewCellData);
    }
}

void FMinesweeperPluginGameManager::FlagCell(const FCellData& Cell)
{
    if (GameState != EGameStates::Playing)
    {
        return;
    }

    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameView().IsValid())
    {
        DisableGameplay();
        return;
    }

    const FCellData& CellData = PluginModule->GetGameData()->GetCellData(Cell.Position);

    switch (CellData.State)
    {
    case ECellStates::Covered:
        if (PluginModule->GetGameData()->FlagCell(Cell))
            PluginModule->GetGameView()->UpdateCellImage(Cell);
        break;

    case ECellStates::Flagged:
        if (PluginModule->GetGameData()->UnflagCell(Cell))
            PluginModule->GetGameView()->UpdateCellImage(Cell);
        break;
    }
}

FText FMinesweeperPluginGameManager::CurrentGameState()
{
    switch (GameState)
    {
    case EGameStates::Playing:
        return LOCTEXT("Playing", "Playing");
    case EGameStates::Dead:
        return LOCTEXT("Lost", "Lost");
    case EGameStates::NotPlaying:
        return LOCTEXT("Ready to Play", "Ready to Play");
    case EGameStates::Win:
        return LOCTEXT("Won", "You Won!");
    case EGameStates::Disabled:
        return LOCTEXT("Error", "Error");
    }

    return LOCTEXT("Error", "Error");
}

void FMinesweeperPluginGameManager::EndGameplay(EGameStates EndGameState)
{
    if (GameState != EGameStates::Playing)
    {
        return;
    }

    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameView().IsValid())
    {
        DisableGameplay();
        return;
    }

    // Finish the game like in minesweeper, reveal all cells and end the game
    PluginModule->GetGameData()->RevealBoard();
    PluginModule->GetGameView()->UpdateMinesweeperBoardDisplay();

    GameState = EndGameState;
}

void FMinesweeperPluginGameManager::DisableGameplay()
{
    GameState = EGameStates::Disabled;
}

#undef LOCTEXT_NAMESPACE
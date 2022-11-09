// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum ECellStates
{
    Covered,
    Flagged,
    Uncovered,
    Exploded
};

struct FCellData
{
    bool HasBomb;
    int NeighboringBombs;

    FVector2D Position;
    ECellStates State;

public:

    static FCellData DefaultValue()
    {
        FCellData NewCellData = FCellData();
        NewCellData.HasBomb = false;
        NewCellData.NeighboringBombs = 0;
        NewCellData.Position = FVector2D(0);
        NewCellData.State = ECellStates::Covered;

        return NewCellData;
    }
};

class MINESWEEPERPLUGIN_API FMinesweeperPluginGameData
{

public:

    FMinesweeperPluginGameData();

    // Game state data
    uint16 GetMinesweeperBoardSize() const { return (uint16)Cells.Num(); }
    uint8 GetMinesweeperBoardWidth() const { return MinesweeperBoardWidth; }
    uint8 GetMinesweeperBoardHeight() const { return MinesweeperBoardHeight; }
    uint16 GetBombs() const { return Bombs; }
    uint16 GetNumberOfCellsRevealed() const { return CellsUncovered; }

    // Start a new game
    void InitMinesweeper(uint8 GridWidth, uint8 GridHeight, TArray<bool> BombLocations);

    // Tools
    FCellData GetCellData(uint16 CellIndex) const;
    FCellData GetCellData(FVector2D CellPosition) const;
    uint16 GetCellIndex(FCellData CellData) const;
    bool IsValidCellData(FCellData CellData) const;
    uint16 GetNeighboringBombs(const FCellData& CellData) const;

    // Actions
    bool UncoverCell(const FCellData& CellData);
    bool FlagCell(const FCellData& CellData);
    bool UnflagCell(const FCellData& CellData);
    bool ExplodeCell(const FCellData& CellData);
    void RevealBoard();

protected:
    // Use a 1D array (parse to 2d with a function)
    TArray<FCellData> Cells;

    uint8 MinesweeperBoardWidth;
    uint8 MinesweeperBoardHeight;

    uint16 Bombs;

    uint16 CellsUncovered;
};

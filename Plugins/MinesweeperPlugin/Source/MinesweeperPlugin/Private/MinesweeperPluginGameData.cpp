// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperPluginGameData.h"


FMinesweeperPluginGameData::FMinesweeperPluginGameData()
{
    Cells.Empty();
    MinesweeperBoardWidth = 0;
    MinesweeperBoardHeight = 0;
    Bombs = 0;
    CellsUncovered = 0;
}

void FMinesweeperPluginGameData::InitMinesweeper(uint8 GridWidth, uint8 GridHeight, TArray<bool> BombLocations)
{
    CellsUncovered = 0;

    // Make sure we always have a grid
    MinesweeperBoardWidth = GridWidth > 0 ? GridWidth : 1;
    MinesweeperBoardHeight = GridHeight > 0 ? GridHeight : 1;

    Cells.Empty();
    Bombs = 0;

    // Fill board
    for (uint8 HeightIndex = 0; HeightIndex < GetMinesweeperBoardHeight(); ++HeightIndex)
    {
        for (uint8 WidthIndex = 0; WidthIndex < GetMinesweeperBoardWidth(); ++WidthIndex)
        {
            FCellData NewCellData = FCellData::DefaultValue();
            NewCellData.Position.X = WidthIndex;
            NewCellData.Position.Y = HeightIndex;

            Cells.Add(NewCellData);
        }
    }

    // BombLocations's size may not match the Cells size, so take that into account
    for (int Index = 0; Index < GetMinesweeperBoardSize(); ++Index)
    {
        if (Index < BombLocations.Num())
        {
            Cells[Index].HasBomb = BombLocations[Index];

            if (BombLocations[Index])
            {
                ++Bombs;
            }
        }
    }

    // Cache Neighboring Bombs
    for (FCellData& CellData : Cells)
    {
        CellData.NeighboringBombs = GetNeighboringBombs(CellData);
    }

    // Must have at least 1 bomb
    if (Bombs == 0)
    {
        Bombs = 1;
        Cells[0].HasBomb = true;
    }
}

// Tools
FCellData FMinesweeperPluginGameData::GetCellData(uint16 CellIndex) const
{
    if (!Cells.IsValidIndex(CellIndex))
    {
        return FCellData::DefaultValue();
    }

    return Cells[CellIndex];
}

FCellData FMinesweeperPluginGameData::GetCellData(FVector2D CellPosition) const
{
    FCellData NewCellData = FCellData::DefaultValue();
    NewCellData.Position = CellPosition;

    return GetCellData(GetCellIndex(NewCellData));
}

uint16 FMinesweeperPluginGameData::GetCellIndex(FCellData CellData) const
{
    if (!IsValidCellData(CellData))
    {
        return -1;
    }

    return ((uint16)CellData.Position.X) + ((uint16)MinesweeperBoardWidth) * ((uint16)CellData.Position.Y);
}

bool FMinesweeperPluginGameData::IsValidCellData(FCellData CellData) const
{
    return CellData.Position.X < MinesweeperBoardWidth&& CellData.Position.Y < MinesweeperBoardHeight;
}

uint16 FMinesweeperPluginGameData::GetNeighboringBombs(const FCellData& CellData) const
{
    if (!IsValidCellData(CellData))
    {
        return 0;
    }

    uint16 NeighboringBombs = 0;

    // top left
    if (CellData.Position.X > 0 && CellData.Position.Y > 0)
    {
        if (GetCellData(FVector2D(CellData.Position.X - 1, CellData.Position.Y - 1)).HasBomb)
        {
            NeighboringBombs++;
        }
    }

    // top 
    if (CellData.Position.Y > 0)
    {
        if (GetCellData(FVector2D(CellData.Position.X, CellData.Position.Y - 1)).HasBomb)
        {
            NeighboringBombs++;
        }
    }

    // top right
    if (CellData.Position.X < (MinesweeperBoardWidth - 1) && CellData.Position.Y> 0)
    {
        if (GetCellData(FVector2D(CellData.Position.X + 1, CellData.Position.Y - 1)).HasBomb)
        {
            NeighboringBombs++;
        }
    }

    // left
    if (CellData.Position.X > 0)
    {
        if (GetCellData(FVector2D(CellData.Position.X - 1, CellData.Position.Y)).HasBomb)
        {
            NeighboringBombs++;
        }
    }

    // right
    if (CellData.Position.X < (MinesweeperBoardWidth - 1))
    {
        if (GetCellData(FVector2D(CellData.Position.X + 1, CellData.Position.Y)).HasBomb)
        {
            NeighboringBombs++;
        }
    }

    // bottom left
    if (CellData.Position.X > 0 && CellData.Position.Y < (MinesweeperBoardHeight - 1))
    {
        if (GetCellData(FVector2D(CellData.Position.X - 1, CellData.Position.Y + 1)).HasBomb)
        {
            NeighboringBombs++;
        }
    }

    // bottom
    if (CellData.Position.Y < (MinesweeperBoardHeight - 1))
    {
        if (GetCellData(FVector2D(CellData.Position.X, CellData.Position.Y + 1)).HasBomb)
        {
            NeighboringBombs++;
        }
    }

    // bottom right
    if (CellData.Position.X < (MinesweeperBoardWidth - 1) && CellData.Position.Y < (MinesweeperBoardHeight - 1))
    {
        if (GetCellData(FVector2D(CellData.Position.X + 1, CellData.Position.Y + 1)).HasBomb)
        {
            NeighboringBombs++;
        }
    }

    return NeighboringBombs;
}

bool FMinesweeperPluginGameData::UncoverCell(const FCellData& CellData)
{
    if (!IsValidCellData(CellData))
    {
        return false;
    }

    uint16 Index = GetCellIndex(CellData);

    if (Index >= GetMinesweeperBoardSize())
    {
        return false;
    }

    if (Cells[Index].State == ECellStates::Uncovered)
    {
        return false;
    }

    Cells[Index].State = ECellStates::Uncovered;

    if (!Cells[Index].HasBomb)
    {
        CellsUncovered++;
    }

    return true;
}

bool FMinesweeperPluginGameData::FlagCell(const FCellData& CellData)
{
    if (!IsValidCellData(CellData))
    {
        return false;
    }

    uint16 Index = GetCellIndex(CellData);

    if (Index >= GetMinesweeperBoardSize())
    {
        return false;
    }

    if (Cells[Index].State != ECellStates::Covered)
    {
        return false;
    }

    Cells[Index].State = ECellStates::Flagged;

    return true;
}

bool FMinesweeperPluginGameData::UnflagCell(const FCellData& CellData)
{
    if (!IsValidCellData(CellData))
    {
        return false;
    }

    uint16 Index = GetCellIndex(CellData);

    if (Index >= GetMinesweeperBoardSize())
    {
        return false;
    }

    if (Cells[Index].State != ECellStates::Flagged)
    {
        return false;
    }

    Cells[Index].State = ECellStates::Covered;

    return true;
}

bool FMinesweeperPluginGameData::ExplodeCell(const FCellData& CellData)
{
    if (!IsValidCellData(CellData))
    {
        return false;
    }

    uint16 Index = GetCellIndex(CellData);

    if (Index >= GetMinesweeperBoardSize())
    {
        return false;
    }

    if (Cells[Index].State == ECellStates::Exploded)
    {
        return false;
    }

    Cells[Index].State = ECellStates::Exploded;

    return true;
}

void FMinesweeperPluginGameData::RevealBoard()
{
    for (FCellData& CellData : Cells)
    {
        if (CellData.State == ECellStates::Exploded)
        {
            continue;
        }

        CellData.State = ECellStates::Uncovered;
    }

    CellsUncovered = (uint16)(GetMinesweeperBoardSize() - Bombs);
}

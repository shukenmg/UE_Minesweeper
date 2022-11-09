// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "MinesweeperPluginGameData.h"

class SMinesweeperPluginTab;

class MINESWEEPERPLUGIN_API FMinesweeperPluginGameView : public TSharedFromThis<FMinesweeperPluginGameView>
{
public:

	// Uniform size of the buttons.
	static const float CellSize;

	FMinesweeperPluginGameView();

	void Init();
	void Destroy();

	// Tab helpers
	void ShowTab();
	bool IsTabOpen() const { return Tab.IsValid(); }
	void OnTabClosed(TSharedRef<SDockTab> Tab);

	// Game settings
	uint8 GetMinesweeperBoardWidth() const { return MinesweeperBoardWidth; };
	uint8 GetMinesweeperBoardHeight() const { return MinesweeperBoardHeight; };
	uint16 GetMinesweeperBombs() const;

	// Used for updating these values from code
	void SetMinesweeperBoardWidth(uint8 NewVal);
	void SetMinesweeperBoardHeight(uint8 NewVal);
	void SetMinesweeperBombs(uint16 NewVal);

	// Used for updating these values from the UI
	void OnMinesweeperBoardWidthSliderValueChanged(uint8 NewVal);
	void OnMinesweeperBoardHeightSliderValueChanged(uint8 NewVal);
	void OnMinesweeperBombsSliderValueChanged(uint16 NewVal);

	// UI Helpers
	bool PopulateMinesweeperBoard();
	void UpdateMinesweeperBoardDisplay();
	void UpdateCellImage(const FCellData& CellData);

protected:

	TSharedPtr<SMinesweeperPluginTab> Tab;

	TSharedRef<SDockTab> CreateMinesweeperTab(const FSpawnTabArgs& TabSpawnArgs);

	// Player Input
	FKey MouseDownInput;
	FCellData MouseDownSelectedCell;

	static FCellData MouseEventToCellPosition(const FGeometry& Geometry, const FPointerEvent& Event);

	// Game UI Inputs
	FReply OnNewGameButtonPressed();
	FReply OnMinesweeperMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& Event);
	FReply OnMinesweeperMouseButtonUp(const FGeometry& Geometry, const FPointerEvent& Event);
	void OnMinesweeperBoardPressed(FKey MouseButton, const FCellData& Cell);

	// Slider backend values
	uint8 MinesweeperBoardWidth;
	uint8 MinesweeperBoardHeight;
	uint16 MinesweeperBombs;
};

#pragma once

#include "game_if.h"
#include "pacman.h"

typedef std::unique_ptr<Pacman> PacmanPtr;

class PacmanController : public IController
{
public:
	PacmanController(const Config& config, GridPtr grid, PacmanPtr& pacman)
		:
		mConfig(config),
		mGrid(grid),
		mPacman(std::move(pacman))
	{
		int maxRows = mGrid->size();
		int maxCols = mGrid->at(0).size();
		mGridBoundaries = Point(maxRows, maxCols);
		int maxPixelRows = (maxRows * mConfig.FIELD_SIZE) + ((maxRows - 1) * mConfig.FIELD_OFFSET) - mConfig.MAP_START_ROW;
		int maxPixelCols = (maxCols * mConfig.FIELD_SIZE) + ((maxCols - 1) * mConfig.FIELD_OFFSET) - mConfig.MAP_START_COLUMN;
		mMapPixelBoundaries = Point(maxPixelRows, maxPixelCols);
	}

	// IController
	void Setup() override {};
	void Update(double delta) override;
	void Draw() override { mPacman->Draw(); }
	void KeyDown(Direction direction) override;
	void SetGameActive(bool gameActive) override { mGameActive = gameActive; }

private:
	//void AdjustPixelPosition(Direction direction, GameField* nextField);
	bool PacmanAllowedToEnterNextField(Direction direction, GameField* nextField);
	bool PacmanCanMoveInItsOwnField();
	GameField* GetNextField(Direction direction);
	bool PacmanIsInNextField(GameField* nextField);
	Point ReadjustedPosition(Point positionInPixels);

private:
	GridPtr mGrid;
	PacmanPtr mPacman;
	const Config& mConfig;
	double mLastUpdateTime = 0.0;
	bool mGameActive{ false };
	std::chrono::milliseconds mUpdateInterval{ 0 };
	std::chrono::duration<double>mTimeSinceLastUpdate{ 0.0 };
	Point mGridBoundaries{ 0,0 };
	Point mMapPixelBoundaries{ 0,0 };
};


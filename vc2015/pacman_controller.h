#pragma once

#include "game_if.h"
#include "pacman.h"

class PacmanController : public IController
{
public:
	PacmanController(const Config& config, GridPtr grid, PacmanPtr& pacman, Boundaries& boundaries)
		:
		mConfig(config),
		mGrid(grid),
		mPacman(std::move(pacman)),
		mBoundaries(boundaries)
	{
	}

	// IController
	void Setup() override {};
	void Update(double delta) override;
	void Draw() override;
	void KeyDown(Direction direction) override;
	void UpdateGameState(GameState gameState) override { mGameState = gameState; }

	void SetScoreUpdateCallback(std::function<void(int)> callBack) { mUpdateScore = callBack; }

private:
	bool PacmanAllowedToEnterNextField(Direction direction, GameField* nextField);
	bool PacmanCanMoveInItsOwnField();
	GameField* GetNextField(Direction direction);
	bool PacmanIsInNextField(GameField* nextField);

private:
	GridPtr mGrid;
	PacmanPtr mPacman;
	IController* mScoreController;
	const Config& mConfig;
	GameState mGameState{ NOT_STARTED };
	std::chrono::milliseconds mUpdateInterval{ 0ms };
	std::chrono::duration<double> mLastUpdate{ 0.0 };
	Boundaries& mBoundaries;
	std::function<void(int score)> mUpdateScore;
};
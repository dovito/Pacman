#pragma once

#include "game_if.h"
#include "config.h"
#include "pacman.h"

class GameCurtainController : public IController
{
public:
	GameCurtainController(const Config& config, GridPtr grid)
		:
		mConfig(config),
		mGrid(grid),
		mScreenCenter(Config::WINDOW_SIZE_Y / 2, Config::WINDOW_SIZE_X / 2)
	{
		Point pacmanCenter = Point(mScreenCenter.mRow - 50, mScreenCenter.mColumn);
		mPacman = std::make_unique<Pacman>(pacmanCenter, Point(0, 0), mConfig);
		mPacman->SetRadius(36);
	}

	void Setup() override;
	void Update(double elapsedTime) override;
	void Draw() override;

	void KeyDown(Direction direction) override {}
	void UpdateGameState(GameState gameState) override { mGameState = gameState; }

private:
	void DrawGameInitialCurtain();
	void DrawGameActiveCurtain();
	void DrawGamePausedCurtain();
	void DrawGameOverCurtain();
	void UpdatePacmanDirection();
	void ResetPacman();

private:
	GridPtr mGrid;
	const Config& mConfig;
	PacmanPtr mPacman;
	Point mScreenCenter;
	GameState mGameState{ NOT_STARTED };
	std::chrono::milliseconds mUpdateInterval{ 0ms };
	std::chrono::duration<double> mLastUpdate{ 0.0 };
};


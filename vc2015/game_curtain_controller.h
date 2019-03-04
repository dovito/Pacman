#pragma once

#include "game_if.h"
#include "config.h"
#include "pacman.h"

class GameCurtainController : public IController
{
public:
	GameCurtainController(const Config& config, GridPtr grid, Boundaries& boundaries)
		:
		mConfig(config),
		mGrid(grid),
		mBoundaries(boundaries),
		mScreenCenter(Config::WINDOW_SIZE_Y / 2, Config::WINDOW_SIZE_X / 2)
	{
		Point pacmanCenter = Point(mScreenCenter.mRow - 50, mScreenCenter.mColumn);
		mPacman = std::make_unique<Pacman>(pacmanCenter, Point(0, 0), mConfig);
		mPacman->SetRadius(30);
	}

	void Setup() override;
	void Update(double elapsedTime) override;
	void Draw() override;

	void KeyDown(Direction direction) override {}
	void UpdateGameState(GameState gameState) override { mGameState = gameState; }
	void UpdateScore(int score);

private:
	void DrawGameInitialCurtain();
	void DrawGameActiveCurtain();
	void DrawGamePausedCurtain();
	void DrawGameOverCurtain();
	void DrawHelpBoard();
	void DrawScore();


private:
	GridPtr mGrid;
	const Config& mConfig;
	PacmanPtr mPacman;
	Point mScreenCenter;
	const Boundaries& mBoundaries;
	GameState mGameState{ NOT_STARTED };
	int mScore{ 0 };
	std::chrono::milliseconds mUpdateInterval{ 0ms };
	std::chrono::duration<double> mLastUpdate{ 0.0 };
};

typedef std::unique_ptr<GameCurtainController> GameCurtainPtr;


#pragma once

#include "game_if.h"
#include "ghost.h"

typedef std::unique_ptr<Ghost> GhostPtr;

class GhostController : public IController
{
public:
	GhostController(const Config& config, GridPtr grid, GhostPtr& ghost, Boundaries& boundaries)
		:
		mConfig(config),
		mGrid(grid),
		mGhost(std::move(ghost)),
		mBoundaries(boundaries)
	{
	}

	// IController
	void Setup() override {};
	void Update(double delta) override;
	void Draw() override { mGhost->Draw(); }
	void KeyDown(Direction direction) override {};
	void SetGameActive(bool gameActive) override { mGameActive = gameActive; }
	void OnScoreUpdate(int score) override {};

private:
	bool GhostAllowedToEnterNextField(Direction direction, GameField* nextField);
	bool GhostCanMoveInItsOwnField();
	GameField* GetNextField(Direction direction);
	bool GhostIsInNextField(GameField* nextField);
	Direction GetRandomDirection();

private:
	GridPtr mGrid;
	GhostPtr mGhost;
	const Config& mConfig;
	bool mGameActive{ false };
	std::chrono::milliseconds mUpdateInterval{ 0ms };
	std::chrono::duration<double> mLastUpdate{ 0.0 };
	Boundaries& mBoundaries;
};

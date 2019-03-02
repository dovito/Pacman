#pragma once

#include "config.h"
#include "game_if.h"
#include "game_fields.h"
#include "pacman_controller.h"
#include "ghost_controller.h"

using namespace ci;

class GameController : public IController
{
public:
	GameController(const Config& config)
		:
		mConfig(config)
	{
	}

	void Setup() override;
	void Update(double elapsedTime) override;
	void Draw() override;

	void KeyDown(Direction direction) override;
	void SetGameActive(bool gameActive) override;
	void OnScoreUpdate(int score) override;

private:
	
	void CreateGameWorld();
	void SetupMapBoundaries();
	void SetupGameObjectControllers();
	void DrawScore();
	void ReleaseGhosts();

private:
	const Config& mConfig;
	GridPtr mGrid;
	PacmanPtr mPacman;
	Boundaries mBoundaries;
	int mScore{ 0 };
	bool mGameActive{ false };
	ObjectControllers mObjectControllers;
	std::vector<GhostPtr> mGhosts;
	bool mGhostsReleased{ false };

};
		

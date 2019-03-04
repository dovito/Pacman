#pragma once

#include "config.h"
#include "game_if.h"
#include "game_fields.h"
#include "pacman_controller.h"
#include "ghost_controller.h"
#include "game_curtain_controller.h"

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
	void UpdateGameState(GameState gameState) override;

private:
	
	void CreateGameWorld();
	void SetupMapBoundaries();
	void SetupControllers();
	
	void ReleaseGhosts();

private:
	const Config& mConfig;
	GridPtr mGrid;
	PacmanPtr mPacman;
	Boundaries mBoundaries;
	int mScore{ 0 };
	GameState mGameState{ NOT_STARTED };
	Controllers mObjectControllers;
	GameCurtainController* mCurtainController = nullptr;
	double mTimeSinceLastUpdate = 0.0;
	std::chrono::duration<double> mTimeSinceGameActive{ 0.0 };
	std::vector<GhostPtr> mGhosts;
	bool mGhostsReleased{ false };

};
		

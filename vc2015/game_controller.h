#pragma once

#include "config.h"
#include "game_if.h"
#include "game_fields.h"
#include "pacman_controller.h"

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

private:
	
	void CreateGameWorld();
	void SetupGameObjectControllers();

private:
	const Config& mConfig;
	GridPtr mGrid;
	PacmanPtr mPacman;
	bool mGameActive{ false };
	ObjectControllers mObjectControllers;
};
		

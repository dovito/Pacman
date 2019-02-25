#pragma once

#include "config.h"
#include "game_if.h"

using namespace ci;

class GameController : public IController
{
public:
	GameController(const Config& config)
		:
		mConfig(config)
	{
	}

	void setup() override;
	void update(double delta) override;
	void draw() override;

private:
	

	void ParseMap();


private:
	const Config& mConfig;
	GameMap mGameMap;
	std::unique_ptr<BoardController> mBoardController;

};
		

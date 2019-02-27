#pragma once

#include "config.h"
#include "game_if.h"
#include "game_fields.h"

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
	typedef std::unique_ptr<GameField> FieldPtr;
	typedef std::vector<std::vector<std::unique_ptr<GameField>>> GameMap;
	void ParseMap();
	bool PacmanAllowedToMove();
	Point GetMapPosition(Point positionInPixels);

private:
	const Config& mConfig;
	GameMap mGameMap;
	Pacman* mPacman = nullptr;
	bool mGameActive{ false };
	//std::unique_ptr<BoardController> mBoardController;

};
		

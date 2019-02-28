#pragma once

#include "config.h"
#include "game_if.h"
#include "game_fields.h"

#include <cinder/app/App.h>
#include <cinder/app/KeyEvent.h>

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

	void KeyDown(Direction direction);
	void GameStarted() { mGameActive = true; }

private:
	typedef std::unique_ptr<GameField> FieldPtr;
	typedef std::vector<std::vector<std::unique_ptr<GameField>>> GameMap;
	void ParseMap();
	bool PacmanAllowedToMove(Direction direction, GameField* nextField);
	Point GetMapPosition(Point point);
	GameField* GetNextField(Direction direction);
	void AdjustPixelPosition(Direction direction, GameField* nextField);

private:
	const Config& mConfig;
	GameMap mGameMap;
	std::unique_ptr<Pacman> mPacman;
	bool mGameActive{ false };
};
		

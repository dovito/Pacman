#include "game_controller.h"
#include <cinder/app/KeyEvent.h>
#include <cinder/Log.h>
#include <boost/algorithm/string.hpp>
#include <fstream>

void GameController::Setup()
{
	CreateGameWorld();
	SetupGameObjectControllers();
}

void GameController::Update(double elapsedTime)
{
	for (auto& objectController : mObjectControllers)
	{
		objectController->Update(elapsedTime);
	}
}


void GameController::Draw()
{
	for (auto& objectController : mObjectControllers)
	{
		objectController->Draw();
	}

	for (auto& row : *mGrid)
	{
		for (auto& field : row)
		{
			field->Draw();
		}
	}
}

void GameController::KeyDown(Direction direction)
{
	for (auto& objectController : mObjectControllers)
	{
		objectController->KeyDown(direction);
	}
}

void GameController::CreateGameWorld()
{
	
	int fieldRow = mConfig.MAP_START_ROW;
	std::ifstream inFile(mConfig.MAP_FILE_NAME);
	Grid grid;
	for (std::string line; getline(inFile, line);)
	{
		int fieldColumn = mConfig.MAP_START_COLUMN;
		std::vector<std::string> tokens;
		boost::split(tokens, line, boost::is_any_of(","));

		std::vector<FieldPtr> gameFields;
		for (auto token : tokens)
		{
			Point center(fieldRow, fieldColumn);
			Point mapPosition{ static_cast<int>(grid.size()), static_cast<int>(gameFields.size()) };

			auto it = stringToField.find(token);
			if (it != stringToField.end())
			{
				switch (it->second)
				{
				case FIELD::WALL:
					gameFields.push_back(std::make_unique<Wall>(center, mapPosition, mConfig));
					break;
				case FIELD::BASIC:
					gameFields.push_back(std::make_unique<BasicField>(center, mapPosition, mConfig));
					break;
				case FIELD::COIN:
					gameFields.push_back(std::make_unique<Coin>(center, mapPosition, mConfig));
					break;
				case FIELD::PACMAN:
					gameFields.push_back(std::make_unique<BasicField>(center, mapPosition, mConfig));
					mPacman = std::make_unique<Pacman>(center, mapPosition, mConfig);
					break;
				default: // EMPTY
					gameFields.push_back(std::make_unique<EmptyField>(center, mapPosition, mConfig));
					break;
				}
			}
			fieldColumn += mConfig.FIELD_SIZE + mConfig.FIELD_OFFSET;
		}
		grid.push_back(std::move(gameFields));
		fieldRow += mConfig.FIELD_SIZE + mConfig.FIELD_OFFSET;
	}
	mGrid = std::make_shared<Grid>(std::move(grid));
}

void GameController::SetupGameObjectControllers()
{
	mObjectControllers.push_back(std::make_unique<PacmanController>(mConfig, mGrid,mPacman));
}

void GameController::SetGameActive(bool gameActive)
{
	for (auto& objectController : mObjectControllers)
	{
		objectController->SetGameActive(gameActive);
	}
}
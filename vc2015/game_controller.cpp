#include "game_controller.h"
#include "game_fields.h"
#include <cinder/Log.h>
#include <boost/algorithm/string.hpp>
#include <fstream>

void GameController::setup()
{
	ParseMap();
}

void GameController::update(double delta)
{
	for (auto& row : mGameMap)
	{
		for (auto& field : row)
		{
			field->update(delta);
		}
	}
}

void GameController::draw()
{
	for (auto& row : mGameMap)
	{
		for (auto& field : row)
		{
			field->draw();
		}
	}
}

void GameController::ParseMap()
{
	int fieldRow = mConfig.MAP_START_ROW;
	std::ifstream inFile(mConfig.MAP_FILE_NAME);
	CI_LOG_I(mConfig.MAP_FILE_NAME);

	for (std::string line; getline(inFile, line);)
	{
		int fieldColumn = mConfig.MAP_START_COLUMN;
		CI_LOG_I(line);
		std::vector<std::string> tokens;
		boost::split(tokens, line, boost::is_any_of(","));

		std::vector<FieldPtr> gameFields;
		for (auto token : tokens)
		{
			Point center(fieldRow, fieldColumn);
			Point mapPosition{ static_cast<int>(mGameMap.size()), static_cast<int>(gameFields.size()) };

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
					gameFields.push_back(std::make_unique<Pacman>(center, mapPosition, mConfig));
					break;
				default: // EMPTY
					break;
				}
			}
			fieldColumn += mConfig.FIELD_SIZE + mConfig.FIELD_OFFSET;
		}
		mGameMap.push_back(std::move(gameFields));
		fieldRow += mConfig.FIELD_SIZE + mConfig.FIELD_OFFSET;
	}
}
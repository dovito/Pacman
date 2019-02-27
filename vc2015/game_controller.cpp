#include "game_controller.h"
#include <cinder/Log.h>
#include <boost/algorithm/string.hpp>
#include <fstream>

void GameController::setup()
{
	ParseMap();
}

void GameController::update(double delta)
{
	if (mGameActive && PacmanAllowedToMove())
		mPacman->SetAllowedToMove(true);

	mPacman->update(delta);
	PacmanAllowedToMove();

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
					mPacman = static_cast<Pacman*>(gameFields.back().get());
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

bool GameController::PacmanAllowedToMove()
{
	auto pacmanPosition = GetMapPosition(mPacman->GetCenter());
	CI_LOG_I("map pos=" << pacmanPosition);
	CI_LOG_I("real pos=" << mPacman->GetMapPosition());
	switch (mPacman->GetDirection())
	{
	case Direction::LEFT:
		return pacmanPosition.mColumn > 0 &&
			mGameMap.at(pacmanPosition.mRow).at(pacmanPosition.mColumn - 1)->IsVisitable();
	case Direction::RIGHT:
		return pacmanPosition.mColumn < (int)(mGameMap.at(pacmanPosition.mRow).size() - 1) &&
			mGameMap.at(pacmanPosition.mRow).at(pacmanPosition.mColumn + 1)->IsVisitable();
	case Direction::UP:
		return pacmanPosition.mRow > 0 &&
			mGameMap.at(pacmanPosition.mRow - 1).at(pacmanPosition.mColumn)->IsVisitable();
	case Direction::DOWN:
		return pacmanPosition.mRow < (int)(mGameMap.size() - 1) &&
			mGameMap.at(pacmanPosition.mRow + 1).at(pacmanPosition.mColumn)->IsVisitable();
	default:
		return false;
	}
}

Point GameController::GetMapPosition(Point positionInPixels)
{
	int adjustment = mConfig.MAP_START_ROW + mConfig.FIELD_SIZE + mConfig.FIELD_OFFSET;
	int divisor = mConfig.FIELD_SIZE + mConfig.FIELD_OFFSET;
	int row = (positionInPixels.mRow - adjustment) / divisor;
	int column = (positionInPixels.mColumn - adjustment) / divisor;

	return Point(row+1, column+1);
}


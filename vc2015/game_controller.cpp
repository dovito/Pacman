#include "game_controller.h"
#include <cinder/app/KeyEvent.h>
#include <cinder/Log.h>
#include <boost/algorithm/string.hpp>
#include <fstream>

void GameController::setup()
{
	ParseMap();
}

void GameController::update(double delta)
{
	if (mGameActive)
	{
		Direction direction = mPacman->GetDirection();
		GameField* nextField = GetNextField(direction);

		if (PacmanAllowedToMove(direction, nextField))
		{
			mPacman->SetDirection(direction);
			mPacman->MakeStep();
			AdjustPixelPosition(direction, nextField);
		}
	}

	mPacman->UpdateMouth(delta);
}

void GameController::AdjustPixelPosition(Direction direction, GameField* nextField)
{
	switch (direction)
	{
	case LEFT:
	case RIGHT:
		mPacman->SetCenter(Point(nextField->GetCenter().mRow, mPacman->GetCenter().mColumn));
		break;
	case DOWN:
	case UP:
		mPacman->SetCenter(Point(mPacman->GetCenter().mRow, nextField->GetCenter().mColumn));
		break;
	case NONE:
	default:
		break;
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

	mPacman->draw();
}

void GameController::KeyDown(Direction direction)
{
	GameField* nextField = GetNextField(direction);
	if (nextField && PacmanAllowedToMove(direction, nextField))
	{	
		CI_LOG_I("next_field=" << nextField->GetMapPosition());
		mPacman->SetDirection(direction);
		AdjustPixelPosition(direction, nextField);
	}
}

void GameController::ParseMap()
{
	int fieldRow = mConfig.MAP_START_ROW;
	std::ifstream inFile(mConfig.MAP_FILE_NAME);

	for (std::string line; getline(inFile, line);)
	{
		int fieldColumn = mConfig.MAP_START_COLUMN;
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
		mGameMap.push_back(std::move(gameFields));
		fieldRow += mConfig.FIELD_SIZE + mConfig.FIELD_OFFSET;
	}
}

bool GameController::PacmanAllowedToMove(Direction direction, GameField* nextField)
{
	auto pacmanPixelPosition = mPacman->GetCenter();
	auto pacmanGridPosition = GetMapPosition(pacmanPixelPosition);
	CI_LOG_I("next_field=" << nextField->GetMapPosition());
	CI_LOG_I("pacman=" << pacmanGridPosition);
	switch (direction)
	{
	case LEFT:
		return nextField->IsVisitable() && pacmanGridPosition.mColumn > 0;
	case RIGHT:
		return nextField->IsVisitable() &&
			pacmanGridPosition.mColumn < (int)(mGameMap.at(pacmanGridPosition.mRow).size() - 1);
	case UP:
		return nextField->IsVisitable() && pacmanGridPosition.mRow > 0;
	case DOWN:
		return nextField->IsVisitable() && 
			pacmanGridPosition.mRow < (int)(mGameMap.size() - 1);
	default:
		return false;
	}
}

GameField* GameController::GetNextField(Direction direction)
{
	auto pacmanGridPosition = GetMapPosition(mPacman->GetCenter());
	CI_LOG_I(pacmanGridPosition);
	switch (direction)
	{
	case LEFT:
		return mGameMap.at(pacmanGridPosition.mRow).at(pacmanGridPosition.mColumn - 1).get();
	case RIGHT:
		return mGameMap.at(pacmanGridPosition.mRow).at(pacmanGridPosition.mColumn + 1).get();
	case UP:
		return mGameMap.at(pacmanGridPosition.mRow - 1).at(pacmanGridPosition.mColumn).get();
	case DOWN:
		return mGameMap.at(pacmanGridPosition.mRow + 1).at(pacmanGridPosition.mColumn).get();
	default:
		return nullptr;
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


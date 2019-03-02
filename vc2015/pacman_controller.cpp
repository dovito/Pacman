#include "pacman_controller.h"

void PacmanController::Update(double elapsedTime)
{
	double timeDelta = elapsedTime - mLastUpdateTime;
	mPacman->UpdateMouth(timeDelta);

	if (mGameActive)
	{
		Direction direction = mPacman->GetDirection();
		GameField* nextField = GetNextField(direction);

		if (PacmanAllowedToEnterNextField(direction, nextField))
		{
			mPacman->MakeStep();
			if (PacmanIsInNextField(nextField))
			{
				nextField->SetHasPoints(false);
				mPacman->SetCenter(nextField->GetCenter());
				mPacman->SetGridPosition(nextField->GetGridPosition());
				CI_LOG_I("pacman= " << mPacman->GetGridPosition());
				CI_LOG_I("next field= " << nextField->GetGridPosition());
				CI_LOG_I("direction" << ToString(direction));
				CI_LOG_I("next field visitable= " << (nextField->IsVisitable() ? "true" : "false"));
			}
		}
		else if (PacmanCanMoveInItsOwnField())
		{
			mPacman->MakeStep();
		}
	}
	mLastUpdateTime = elapsedTime;
}

void PacmanController::KeyDown(Direction direction)
{
	GameField* nextField = GetNextField(direction);
	if (nextField && PacmanAllowedToEnterNextField(direction, nextField))
	{
		mPacman->SetDirection(direction);
	}
}

bool PacmanController::PacmanAllowedToEnterNextField(Direction direction, GameField* nextField)
{
	if (nextField)
	{
		auto pacmanPixelPosition = mPacman->GetCenter();
		
		if (direction == LEFT || direction == RIGHT)
		{
			return nextField->IsVisitable() &&
				pacmanPixelPosition.mRow == nextField->GetCenter().mRow;
		}
		else if (direction == UP || direction == DOWN)
		{
			return nextField->IsVisitable() &&
				pacmanPixelPosition.mColumn == nextField->GetCenter().mColumn;
		}
	}
	return false;
}

bool PacmanController::PacmanCanMoveInItsOwnField()
{
	auto pacmanPixelPosition = mPacman->GetCenter();
	auto pacmanGridPosition = mPacman->GetGridPosition();
	auto currentField = mGrid->at(pacmanGridPosition.mRow).at(pacmanGridPosition.mRow).get();
	switch (mPacman->GetDirection())
	{
	case LEFT:
		return pacmanPixelPosition.mColumn > currentField->GetCenter().mColumn;
	case RIGHT:
		return pacmanPixelPosition.mColumn < currentField->GetCenter().mColumn;
	case UP:
		return pacmanPixelPosition.mRow > currentField->GetCenter().mRow;
	case DOWN:
		return pacmanPixelPosition.mRow < currentField->GetCenter().mRow;
	default:
		return false;
	}
}

GameField* PacmanController::GetNextField(Direction direction)
{
	auto pacmanGridPosition = mPacman->GetGridPosition();

	if (direction == LEFT)
	{
		if (pacmanGridPosition.mColumn <= 0)
		{
			return mGrid->at(pacmanGridPosition.mRow).at(mGridBoundaries.mColumn - 1).get();;
		}
		else
		{
			return mGrid->at(pacmanGridPosition.mRow).at(pacmanGridPosition.mColumn - 1).get();
		}
	}
	else if (direction == RIGHT)
	{
		if (pacmanGridPosition.mColumn >= mGridBoundaries.mColumn - 1)
		{
			return mGrid->at(pacmanGridPosition.mRow).at(0).get();;
		}
		else
		{
			return mGrid->at(pacmanGridPosition.mRow).at(pacmanGridPosition.mColumn + 1).get();
		}
	}
	else if (direction == UP)
	{
		if (pacmanGridPosition.mRow <= 0)
		{
			return mGrid->at(mGridBoundaries.mRow - 1).at(pacmanGridPosition.mColumn).get();
		}
		else
		{
			return mGrid->at(pacmanGridPosition.mRow - 1).at(pacmanGridPosition.mColumn).get();
		}
	}
	else if (direction == DOWN)
	{
		if (pacmanGridPosition.mRow >= mGridBoundaries.mRow - 1)
		{
			return mGrid->at(0).at(pacmanGridPosition.mColumn).get();
		}
		else
		{
			return mGrid->at(pacmanGridPosition.mRow + 1).at(pacmanGridPosition.mColumn).get();
		}
	}
	return nullptr;
}

bool PacmanController::PacmanIsInNextField(GameField* nextField)
{
	auto pacmanPixelPosition = mPacman->GetCenter();

	if (pacmanPixelPosition.mRow < 0 || mMapPixelBoundaries.mRow < pacmanPixelPosition.mRow ||
		pacmanPixelPosition.mColumn < 0 || mMapPixelBoundaries.mColumn < pacmanPixelPosition.mColumn)
	{
		return true;
	}

	auto nextFieldCenter = nextField->GetCenter();
	double fieldWidth = mConfig.FIELD_SIZE / 2.0;
	
	if (nextFieldCenter.mRow == pacmanPixelPosition.mRow)
	{
		return abs(nextFieldCenter.mColumn - pacmanPixelPosition.mColumn) < fieldWidth;
	}
	else if (nextFieldCenter.mColumn == pacmanPixelPosition.mColumn)
	{
		return abs(nextFieldCenter.mRow - pacmanPixelPosition.mRow) < fieldWidth;

	}

	return false;
}

Point PacmanController::ReadjustedPosition(Point positionInPixels)
{
	int adjustment = mConfig.MAP_START_ROW + mConfig.FIELD_SIZE + mConfig.FIELD_OFFSET;
	int divisor = mConfig.FIELD_SIZE + mConfig.FIELD_OFFSET;
	int row = (positionInPixels.mRow - adjustment) / divisor;
	int column = (positionInPixels.mColumn - adjustment) / divisor;

	return Point(row + 1, column + 1);
}
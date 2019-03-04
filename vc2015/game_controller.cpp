#include "game_controller.h"

#include <boost/algorithm/string.hpp>
#include <cinder/Text.h>
#include <fstream>

void GameController::Setup()
{
	CreateGameWorld();
	SetupMapBoundaries();
	SetupControllers();
}

void GameController::Update(double elapsedTime)
{
	for (auto& objectController : mObjectControllers)
	{
		objectController->Update(elapsedTime);
	}
	
	double timeDelta = elapsedTime - mTimeSinceLastUpdate;
	mTimeSinceLastUpdate = elapsedTime;
	if (mGameState == ACTIVE)
	{
		mTimeSinceGameActive += std::chrono::duration<double>(timeDelta);
		if (!mGhostsReleased && mTimeSinceGameActive > mConfig.RELEASE_GHOSTS)
		{
			ReleaseGhosts();
		}
	}
	
}

void GameController::Draw()
{
	for (auto& objectController : mObjectControllers)
	{
		objectController->Draw();
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
	Grid grid;
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
			Point gridPosition{ static_cast<int>(grid.size()), static_cast<int>(gameFields.size()) };

			auto it = stringToField.find(token);
			if (it != stringToField.end())
			{
				switch (it->second)
				{
				case WALL:
					gameFields.push_back(std::make_unique<Wall>(center, gridPosition, mConfig));
					break;
				case TMP_WALL:
					gameFields.push_back(std::make_unique<TemporaryWall>(center, gridPosition, mConfig));
					break;
				case BASIC:
					gameFields.push_back(std::make_unique<BasicField>(center, gridPosition, mConfig));
					break;
				case COIN:
					gameFields.push_back(std::make_unique<Coin>(center, gridPosition, mConfig));
					break;
				case PACMAN:
					gameFields.push_back(std::make_unique<EmptyField>(center, gridPosition, mConfig));
					mPacman = std::make_unique<Pacman>(center, gridPosition, mConfig);
					break;
				case GHOST:
					gameFields.push_back(std::make_unique<EmptyField>(center, gridPosition, mConfig));
					mGhosts.push_back(std::make_unique<Ghost>(center, gridPosition, mConfig));
					break;
				default: // EMPTY
					gameFields.push_back(std::make_unique<EmptyField>(center, gridPosition, mConfig));
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

void GameController::SetupMapBoundaries()
{
	int maxRows = mGrid->size();
	int maxCols = mGrid->at(0).size();
	mBoundaries.mGrid = Point(maxRows, maxCols);

	int mapRowOffset = static_cast<int>(mConfig.MAP_START_ROW - mConfig.FIELD_SIZE / 2);
	int mapColOffset = static_cast<int>(mConfig.MAP_START_COLUMN - mConfig.FIELD_SIZE / 2);
	int maxPixelRows = (maxRows * mConfig.FIELD_SIZE) + ((maxRows - 1) * mConfig.FIELD_OFFSET) + mapRowOffset;
	int maxPixelCols = (maxCols * mConfig.FIELD_SIZE) + ((maxCols - 1) * mConfig.FIELD_OFFSET) + mapColOffset;
	
	mBoundaries.mMapPixelsMax = Point(maxPixelRows, maxPixelCols);
	mBoundaries.mMapPixelsMin = Point(mapRowOffset, mapColOffset);
}

void GameController::SetupControllers()
{
	auto pacmanController = std::make_unique<PacmanController>(mConfig, mGrid, mPacman, mBoundaries);
	auto curtainController = std::make_unique<GameCurtainController>(mConfig, mGrid, mBoundaries);
	mCurtainController = curtainController.get();
	pacmanController->SetScoreUpdateCallback([this](int score)
	{
		mCurtainController->UpdateScore(score);
	
	});

	mObjectControllers.push_back(std::move(pacmanController));
	mObjectControllers.push_back(std::move(curtainController));
	
	for (auto& ghost : mGhosts)
	{
		mObjectControllers.push_back(std::make_unique<GhostController>(mConfig, mGrid, ghost, mBoundaries));
	}
}

void GameController::UpdateGameState(GameState gameState)
{
	for (auto& objectController : mObjectControllers)
	{
		objectController->UpdateGameState(gameState);
	}
	mGameState = gameState;
}

void GameController::ReleaseGhosts()
{
	for (auto& row : *mGrid)
	{
		for (auto& field : row)
		{
			field->SetIsVisitable(true);
		}
	}
	mGhostsReleased = true;
}
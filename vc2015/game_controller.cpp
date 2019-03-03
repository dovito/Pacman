#include "game_controller.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
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
	for (auto& controller : mControllers)
	{
		controller->Update(elapsedTime);
	}
	
	if (!mGhostsReleased && elapsedTime > 5.0)
	{
		ReleaseGhosts();
	}
}


void GameController::Draw()
{
	
	for (auto& controller : mControllers)
	{
		controller->Draw();
	}

	if (mGameState == ACTIVE)
	{
		DrawScore();
		DrawHelpBoard();
	}
}

void GameController::KeyDown(Direction direction)
{
	for (auto& controller : mControllers)
	{
		controller->KeyDown(direction);
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
	std::unique_ptr<PacmanController> pacmanController = std::make_unique<PacmanController>(mConfig, mGrid, mPacman, mBoundaries);
	pacmanController->SetScoreUpdateCallback([this](int score) {mScore += score; });
	mControllers.push_back(std::move(pacmanController));
	
	for (auto& ghost : mGhosts)
	{
		mControllers.push_back(std::make_unique<GhostController>(mConfig, mGrid, ghost, mBoundaries)); 
	}

	mControllers.push_back(std::make_unique<GameCurtainController>(mConfig, mGrid));
}

void GameController::UpdateGameState(GameState gameState)
{
	for (auto& objectController : mControllers)
	{
		objectController->UpdateGameState(gameState);
	}
	mGameState = gameState;
}

void GameController::OnScoreUpdate(int score)
{
	mScore += score;
}

void GameController::DrawScore()
{
	ci::gl::color(mConfig.RED);

	ci::TextLayout simple;
	ci::gl::Texture2dRef texture;
	simple.setFont(ci::Font("Arial", 28));
	simple.setColor(mConfig.RED);
	simple.addLine("Score: " + boost::lexical_cast<std::string>(mScore));
	texture = ci::gl::Texture2d::create(simple.render(true, false));
	ci::gl::draw(texture, ci::vec2(mBoundaries.mMapPixelsMin.mColumn, mBoundaries.mMapPixelsMax.mRow + 5));
}

void GameController::DrawHelpBoard()
{
	ci::gl::color(mConfig.YELLOW);

	ci::TextLayout simple;
	ci::gl::Texture2dRef texture;
	simple.setFont(ci::Font("Arial", 20));
	simple.setColor(mConfig.YELLOW);
	simple.addLine("pres [ESC] to pause");
	texture = ci::gl::Texture2d::create(simple.render(true, false));
	auto columnOffset = texture->getWidth();
	ci::gl::draw(texture, ci::vec2(mBoundaries.mMapPixelsMax.mColumn - columnOffset, mBoundaries.mMapPixelsMax.mRow + 5));
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

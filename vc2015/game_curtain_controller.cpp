#include "game_curtain_controller.h"

#include "pacman_controller.h"
#include <cinder/Text.h>

void GameCurtainController::Setup()
{
	
}

void GameCurtainController::Update(double elapsedTime)
{
	auto updateNow = std::chrono::duration<double>(elapsedTime);
	auto timeDelta = updateNow - mLastUpdate;
	mUpdateInterval += std::chrono::duration_cast<std::chrono::milliseconds>(timeDelta);

	if (mGameState != ACTIVE && mUpdateInterval >= mConfig.PACMAN_UPDATE_INTERVAL)
	{
		Direction direction = mPacman->GetDirection();
		UpdatePacmanDirection();
		mPacman->UpdateMouth(timeDelta.count());
		//mPacman->MakeStep();
	}

	mLastUpdate = updateNow;
}

void GameCurtainController::Draw()
{
	if (mGameState == NOT_STARTED)
	{
		DrawGameInitialCurtain();
	}
	else if (mGameState == ACTIVE)
	{
		DrawGameActiveCurtain();
	}
	else if (mGameState = PAUSED)
	{
		DrawGamePausedCurtain();
	}
	else if (mGameState == OVER)
	{
		DrawGameOverCurtain();
	}
}

void GameCurtainController::DrawGameInitialCurtain()
{
	mPacman->Draw();

	ci::gl::color(mConfig.BLUE);
	ci::gl::Texture2dRef texture;
	ci::TextLayout startText;
	startText.setFont(ci::Font("Arial", 35));
	startText.setColor(mConfig.BLUE);
	startText.addCenteredLine("Press SPACE to start game");
	texture = ci::gl::Texture2d::create(startText.render(true, false));
	ci::gl::draw(texture, ci::vec2(mScreenCenter.mRow - 90, mScreenCenter.mColumn-50));
}

void GameCurtainController::DrawGameActiveCurtain()
{
	for (auto& row : *mGrid)
	{
		for (auto& field : row)
		{
			field->Draw();
		}
	}
}

void GameCurtainController::DrawGamePausedCurtain()
{
	mPacman->Draw();

	ci::gl::color(mConfig.BLUE);
	ci::gl::Texture2dRef texture;
	ci::TextLayout startText;
	startText.setFont(ci::Font("Arial", 35));
	startText.setColor(mConfig.BLUE);
	startText.addCenteredLine("Game is paused");
	startText.addCenteredLine("Press SPACE to continue");
	texture = ci::gl::Texture2d::create(startText.render(true, false));
	ci::gl::draw(texture, ci::vec2(mScreenCenter.mRow - 90, mScreenCenter.mColumn - 50));
}

void GameCurtainController::DrawGameOverCurtain()
{

}

void GameCurtainController::UpdatePacmanDirection()
{

}

void GameCurtainController::ResetPacman()
{

}
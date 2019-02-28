#pragma once

#include "game_if.h"
#include "config.h"
#include <cinder/Log.h>


class Pacman : public GameField
{
public:
	Pacman(Point center, Point mapPosition, const Config& config)
		:
		mCenter(center),
		mMapPosition(mapPosition),
		mConfig(config)
	{
		mUpdateInterval = std::chrono::milliseconds(mConfig.PACMAN_MOVE_UPDATE);
		mStepSize = mConfig.PACMAN_STEP_IN_PX;
	}

	// IController
	void setup() override {};
	void update(double delta) override;
	void draw() override;

	// GameField
	bool IsVisitable() const override { return false; }
	const Point& GetCenter() const override { return mCenter;  }
	const Point& GetMapPosition() const override { return mMapPosition; }
	void SetCenter(const Point& center) override { mCenter = center; }
	void SetPosition(const Point& position) override { mMapPosition = position; }
	bool HasPoints() const { return false; }
	const int GetPoints() const { return 0; }

	void SetAllowedToMove(bool allowed) { mAllowedToMove = allowed; }
	const Direction GetDirection() const { return mDirection; }
	void SetDirection(Direction direction) { mDirection = direction;  }
	void MakeStep();
	void UpdateMouth(const double delta);

private:

	std::vector<ci::vec2> GetSkeleton();
	std::vector<ci::vec2> GetBorder(std::vector<ci::vec2>& skeleton);
	void AlignDirectionAngle(float& angle);
	
	enum MouthState
	{
		CLOSING,
		OPENING
	};

private:
	Point mCenter;
	Point mMapPosition;
	const Config& mConfig;
	Direction mDirection{ Direction::LEFT };
	float mOpenMouth{ 1.0f };
	MouthState mMouthState{ MouthState::CLOSING };
	std::chrono::milliseconds mUpdateInterval{ 0 };
	std::chrono::duration<float>mTimeSinceLastUpdate{ 0.0 };
	int mStepSize;
	bool mAllowedToMove{ false };
};


#pragma once

#include "game_if.h"
#include "config.h"

class Ghost : public IGameObject
{
public:
	Ghost(Point center, Point mapPosition, const Config& config)
		:
		mCenter(center),
		mMapPosition(mapPosition),
		mConfig(config),
		mStepSize(mConfig.GHOST_STEP_IN_PX),
		mGhostSkelton(std::move(GetSkeleton()))
	{
	}

	// IGameObject
	const Direction GetDirection() const override { return mDirection; }
	void SetDirection(Direction direction) override { mDirection = direction; }
	const Point& GetCenter() const override { return mCenter; }
	void SetCenter(const Point& center) override { mCenter = center; }
	const Point& GetGridPosition() const override { return mMapPosition; }
	void SetGridPosition(const Point& position) override { mMapPosition = position; }
	void MakeStep() override;
	void Draw() override;

private:
	std::vector<ci::vec2> GetSkeleton();
	std::vector<ci::vec2> GetBody();
	void DrawBody(const std::vector<ci::vec2>& body);
	//void AlignDirectionAngle(float& angle);

private:
	Point mCenter;
	Point mMapPosition;
	const Config& mConfig;
	Direction mDirection{ Direction::LEFT };
	int mStepSize;
	bool mAllowedToMove{ false };
	std::vector<ci::vec2> mGhostSkelton;
};


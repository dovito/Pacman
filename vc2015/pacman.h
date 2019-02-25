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
	//const int GetRow() const {/* return mCenter.mColumn; */}
	//const int GetColumn() const { /*return mCenter.mRow; */}
	bool HasPoints() const { return false; }
	const int GetPoints() const { return 0; }

private:

	std::vector<ci::vec2> GetSkeleton();
	std::vector<ci::vec2> GetBorder(std::vector<ci::vec2>& skeleton);
	void AlignDirectionAngle(float& angle);

	enum Direction
	{
		LEFT,
		DOWN,
		RIGHT,
		UP
	};

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
};


#pragma once

#include "game_if.h"
#include "config.h"
#include <cinder/Log.h>

class EmptyField : public GameField
{
public:
	EmptyField(Point center, Point gridPosition, const Config& config)
		:
		mCenter(center),
		mGridPosition(gridPosition),
		mConfig(config)
	{
	}

	// GameField
	bool IsVisitable() const { return true; }
	const Point& GetCenter() const override { return mCenter; }
	const Point& GetGridPosition() const override { return mGridPosition; }
	void SetCenter(const Point& center) override {};
	void SetGridPosition(const Point& position) override {};
	void SetHasPoints(bool) override {};
	const int GetPoints() const { return 0; }
	void Draw() override {};

private:
	Point mCenter;
	Point mGridPosition;
	const Config& mConfig;
};

#pragma once

#include "game_if.h"
#include "config.h"
#include <cinder/Log.h>

class Wall : public GameField
{
public:
	Wall(Point center, Point gridPosition, const Config& config)
		:
		mCenter(center),
		mGirdPosition(gridPosition),
		mConfig(config),
		mFieldSize(config.FIELD_SIZE),
		mOffset(config.FIELD_OFFSET)
	{
	}
	
	// GameField
	bool IsVisitable() const override{ return false; }
	const Point& GetCenter() const override { return mCenter; }
	const Point& GetGridPosition() const override { return mGirdPosition; }
	void SetCenter(const Point& center) override {};
	void SetGridPosition(const Point& position) override {};
	void SetHasPoints(bool hasPoints) override {}
	const int GetPoints() const { return 0;  }
	void Draw() override;

private:
	Point mCenter;
	Point mGirdPosition;
	const Config& mConfig;
	const int mFieldSize;
	const int mOffset;
};
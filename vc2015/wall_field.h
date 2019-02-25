#pragma once

#include "game_if.h"
#include "config.h"
#include <cinder/Log.h>

class Wall : public GameField
{
public:
	Wall(Point center, Point mapPosition, const Config& config)
		:
		mCenter(center),
		mMapPosition(mapPosition),
		mConfig(config),
		mFieldSize(config.FIELD_SIZE),
		mOffset(config.FIELD_OFFSET)
	{
	}

	// IController
	void setup() override {};
	void update(double delta) override {};
	void draw() override;

	// GameField
	bool IsVisitable() const override{ return false; }
	const Point& GetCenter() const override { return mCenter; }
	const Point& GetMapPosition() const override { return mMapPosition; }
	void SetCenter(const Point& center) override {};
	void SetPosition(const Point& position) override {};
	bool HasPoints() const { return false; }
	const int GetPoints() const { return 0;  }

private:
	Point mCenter;
	Point mMapPosition;
	const Config& mConfig;
	const int mFieldSize;
	const int mOffset;
};
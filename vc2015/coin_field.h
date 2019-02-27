#pragma once

#include "game_if.h"
#include "config.h"
#include <cinder/Log.h>

class Coin : public GameField
{
public:
	Coin(Point& center, Point& mapPosition, const Config& config)
		:
		mCenter(std::move(center)),
		mMapPosition(std::move(mapPosition)),
		mConfig(config)
	{
	}

	// IController
	void setup() override {};
	void update(double delta) override {};
	void draw() override;

	// GameField
	bool IsVisitable() const { return true; }
	const Point& GetCenter() const override { return mCenter; }
	const Point& GetMapPosition() const override { return mMapPosition; }
	void SetCenter(const Point& center) override {};
	void SetPosition(const Point& position) override {};
	
	bool HasPoints() const override { return mHasPoints; }
	const int GetPoints() const override { return mConfig.COIN_POINTS; }
	
	void SetHasPoints(bool hasPoints) { mHasPoints = hasPoints; }

private:
	Point mCenter;
	Point mMapPosition;
	const Config& mConfig;
	bool mHasPoints = true;
};


#pragma once

#include "types.h"
#include "config.h"

class BasicField : public GameField
{
public:
	BasicField(const Point center, const Point gridPosition, const Config& config)
		:
		mCenter(center), 
		mGridPosition(gridPosition),
		mConfig(config),
		mPoints(mConfig.BASIC_POINTS)
	{
	}

	// GameField
	bool IsVisitable() const { return true;	}
	void SetIsVisitable(bool visitable) override {}
	const Point& GetCenter() const override { return mCenter; }
	const Point& GetGridPosition() const override { return mGridPosition; }
	void SetCenter(const Point& center) override {}
	void SetGridPosition(const Point& position) override {}
	void UnsetPoints() override { mPoints = 0; }
	const int GetPoints() const { return mPoints; }
	void Draw() override;
	void Reset() override;

private:
	Point mCenter;
	Point mGridPosition;
	const Config& mConfig;
	int mPoints;
};


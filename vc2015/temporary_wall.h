#pragma once

#include "game_if.h"
#include "config.h"

class TemporaryWall : public GameField
{
public:
	TemporaryWall(Point center, Point gridPosition, const Config& config)
		:
		mCenter(center),
		mGirdPosition(gridPosition),
		mConfig(config),
		mFieldSize(config.FIELD_SIZE),
		mOffset(config.FIELD_OFFSET)
	{
	}

	// GameField
	bool IsVisitable() const override { return mIsVisitable; }
	void SetIsVisitable(bool visitable) override { mIsVisitable = visitable; }
	const Point& GetCenter() const override { return mCenter; }
	const Point& GetGridPosition() const override { return mGirdPosition; }
	void SetCenter(const Point& center) override {}
	void SetGridPosition(const Point& position) override {}
	void UnsetPoints() override {}
	const int GetPoints() const { return 0; }
	void Draw() override;

private:
	Point mCenter;
	Point mGirdPosition;
	const Config& mConfig;
	const int mFieldSize;
	const int mOffset;
	bool mIsVisitable{ false };
};


#pragma once

#include <cinder/gl/gl.h>
#include <cinder/Rect.h>
#include <iostream>

typedef std::unique_ptr<GameField> FieldPtr;
typedef std::vector<std::vector<std::unique_ptr<GameField>>> GameMap;

struct Point
{	
	Point(int row, int column)
		:
		mRow(row),
		mColumn(column)
	{
	}
	int mRow;
	int mColumn;
};

inline std::ostream& operator<<(std::ostream& output, const Point& point)
{
	output << "row=" << point.mRow << ", column=" << point.mColumn;
	return output;
}

class IController
{
public:
	virtual ~IController() {}

	virtual void setup() = 0;
	virtual void update(double delta) = 0;
	virtual void draw() = 0;
};

class GameField : public IController
{
public:
	virtual ~GameField() {}

	virtual bool IsVisitable() const = 0;
	//virtual const int GetRow() const = 0;
	//virtual const int GetColumn() const = 0;
	virtual bool HasPoints() const = 0;
	virtual const int GetPoints() const = 0;

	virtual const Point& GetCenter() const = 0;
	virtual const Point& GetMapPosition() const = 0;
	virtual void SetCenter(const Point& center) = 0;
	virtual void SetPosition(const Point& position) = 0;
};
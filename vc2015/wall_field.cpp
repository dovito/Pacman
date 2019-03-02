#include "wall_field.h"

void Wall::Draw()
{
	ci::gl::color(mConfig.BLUE);

	const float x1 = mCenter.mColumn - (mFieldSize * 0.5);
	const float y1 = mCenter.mRow + (mFieldSize * 0.5);
	const float x2 = mCenter.mColumn + (mFieldSize * 0.5);
	const float y2 = mCenter.mRow - (mFieldSize * 0.5);
	ci::Rectf rect(x1, y1, x2, y2);

	ci::gl::drawSolidRect(rect);
}
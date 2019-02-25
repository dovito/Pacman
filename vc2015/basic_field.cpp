#include "basic_field.h"

void BasicField::draw()
{
	ci::gl::color(mConfig.YELLOW);

	ci::gl::drawSolidCircle(
		ci::vec2(mCenter.mColumn, mCenter.mRow),
		mConfig.BASIC_FIELD_RADIUS);
}

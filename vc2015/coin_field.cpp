#include "coin_field.h"

void Coin::draw()
{
	ci::gl::color(mConfig.YELLOW);

	ci::gl::drawSolidCircle(
		ci::vec2(mCenter.mColumn, mCenter.mRow),
		mConfig.COIN_FIELD_RADIUS);
}

#include "pacman.h"

void Pacman::UpdateMouth(const double delta)
{
	if (mMouthState == MouthState::CLOSING)
	{
		mOpenMouth -= delta * 2.0f;
		if (mOpenMouth <= 0.0f)
		{
			mOpenMouth = 0.0f;
			mMouthState = MouthState::OPENING;
		}
	}
	else
	{
		mOpenMouth += delta * 2.0f;
		if (mOpenMouth >= 1.0f)
		{
			mOpenMouth = 1.0f;
			mMouthState = MouthState::CLOSING;
		}
	}
}

void Pacman::MakeStep()
{
	switch (mDirection)
	{
	case Direction::DOWN:
		mCenter.mRow += mStepSize;
		break;
	case Direction::UP:
		mCenter.mRow -= mStepSize;
		break;
	case Direction::RIGHT:
		mCenter.mColumn += mStepSize;
		break;
	case Direction::LEFT:
		mCenter.mColumn -= mStepSize;
		break;
	default:
		break;
	}
}

void Pacman::Draw()
{
	std::vector<ci::vec2> skeleton = std::move(GetSkeleton());
	std::vector<ci::vec2> border = std::move(GetBorder(skeleton));
	ci::vec2 position{ mCenter.mColumn, mCenter.mRow };
	ci::gl::color(mConfig.YELLOW);
	for (auto i = 0u; i < border.size() - 1; ++i)
	{
		const ci::vec2& p1 = border[i];
		const ci::vec2& p2 = border[i + 1];

		ci::gl::drawSolidTriangle(p1, p2, position);
	}
}

std::vector<ci::vec2> Pacman::GetSkeleton()
{
	std::vector<ci::vec2> skeleton;

	auto openMouthAngle = mOpenMouth * M_PI / 4.0f;
	float startAngle = openMouthAngle;
	AlignDirectionAngle(startAngle);
	float endAngle = startAngle + 2.0f * M_PI - 2.0f * openMouthAngle;
	if (endAngle < startAngle)
		openMouthAngle += 2.0f * M_PI;

	const float angleStep = 0.2;

	for (float angle = startAngle; angle <= endAngle; angle += angleStep)
	{
		const float x = mConfig.PACMAN_RADIUS * std::cos(angle);
		const float y = mConfig.PACMAN_RADIUS * std::sin(angle);
		skeleton.emplace_back(x, y);
	}

	return skeleton;
}

std::vector<ci::vec2> Pacman::GetBorder(std::vector<ci::vec2>& skeleton)
{
	std::vector<ci::vec2> points;
	ci::vec2 position{ mCenter.mColumn, mCenter.mRow };

	std::transform(
		std::begin(skeleton),
		std::end(skeleton),
		std::back_inserter(points),
		[&position](const ci::vec2& v) { return v + position; });

	return points;
}

void Pacman::AlignDirectionAngle(float& angle)
{
	if (mDirection == Direction::DOWN)
		angle += M_PI / 2.0f;
	else if (mDirection == Direction::LEFT)
		angle += M_PI;
	else if (mDirection == Direction::UP)
		angle += M_PI * 3.0f / 2.0f;
}

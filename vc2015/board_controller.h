#pragma once

#include "game_if.h"

class BoardController : public IController
{
public:
	virtual ~BoardController();

	// IController
	void setup() override {};
	void update(double delta) override {}; // move pacman and eat coins
	void draw() override {};
};


#pragma once

#include <cinder/Color.h>
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>

using boost::assign::map_list_of;


struct Config
{
	const int FIELD_SIZE = 12;
	const int FIELD_OFFSET = 2;

	const int BASIC_FIELD_RADIUS = 1;
	const int COIN_FIELD_RADIUS = 3;
	const int PACMAN_RADIUS = 6;
	const int PACMAN_MOUTH_SPEED = 3;

	const int PACMAN_UPDATE_INTERVAL = 500; // milliseconds
	const int PACMAN_STEP_IN_PX = 4; // per update interval

	const int MAP_START_ROW = 6;
	const int MAP_START_COLUMN = 6;

	const std::string MAP_FILE_NAME = "C:\\Users\\Dovile\\Desktop\\pman\\Pacman\\map.txt";

	ci::Color YELLOW{ 1, 1, 0 };
	ci::Color BLUE{ 0, 0, 1 };
	ci::Color BLACK{ 0, 0, 0 };

	const int BASIC_POINTS = 1;
	const int COIN_POINTS = 3;
};

enum FIELD 
{ 
	WALL,
	BASIC,
	COIN,
	EMPTY,
	PACMAN,
	GHOST
};
const boost::unordered_map<const std::string, FIELD> stringToField = map_list_of
("W", WALL)
("-", BASIC)
("C", COIN)
(" ", EMPTY)
("P", PACMAN)
("G", GHOST);
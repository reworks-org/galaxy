///
/// GameLevel.cpp
/// sandbox
///
/// Created by reworks on 20/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "GameLevel.hpp"

GameLevel::GameLevel(const sl::Rect<float, int>& bounds)
	:Level(bounds, "csv.tmx", 2.5f)
{
}

void GameLevel::update(const double dt)
{
}
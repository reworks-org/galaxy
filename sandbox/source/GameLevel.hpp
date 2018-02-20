///
/// GameLevel.hpp
/// sandbox
///
/// Created by reworks on 20/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_GAMELEVEL_HPP_
#define SANDBOX_GAMELEVEL_HPP_

#include <sl/mapping/Level.hpp>

class GameLevel : public sl::Level
{
public:
	GameLevel(const sl::Rect<float, int>& bounds);

	void update(const double dt) final override;
};

#endif
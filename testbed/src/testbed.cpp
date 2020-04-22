///
/// testbed.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>
#include <protostar/system/Keys.hpp>
#include <protostar/events/KeyDownEvent.hpp>
#include <protostar/math/Random.hpp>
#include <protostar/math/Rect.hpp>
#include <protostar/math/RectPack.hpp>

int main()
{
	protostar::KeyDownEvent e;
	protostar::Keys k;
	k = protostar::Keys::E;

	e.m_keycode = protostar::Keys::A;

	protostar::Rect<float> r;
	protostar::RectPack pack;

	std::cin.get();
}
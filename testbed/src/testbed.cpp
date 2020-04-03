///
/// testbed.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>
#include <protostar/system/Keys.hpp>
#include <protostar/events/KeyDownEvent.hpp>

int main()
{
	protostar::KeyDownEvent e;
	protostar::Keys k;
	k = protostar::Keys::E;

	e.m_keycode = protostar::Keys::A;

	std::cin.get();
}
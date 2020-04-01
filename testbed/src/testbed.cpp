///
/// testbed.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>
#include <protostar/events/KeyUpEvent.hpp>

int main()
{
	protostar::KeyUpEvent e;

	e.m_keycode = protostar::Keys::A;

	std::cin.get();
}
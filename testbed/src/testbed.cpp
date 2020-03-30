///
/// testbed.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <frb/Context.hpp>
#include <frb/audio/Audible.hpp>

int main()
{
	frb::Context context;
	context.initialize();

	frb::Audible music;
	music.loadFromFile("bin/test.ogg");
	music.play();
	
	std::cin.get();
	music.stop();
}
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
	context.setListenerGain(0.2f);

	frb::Audible music;
	music.load("bin/test.ogg");
	music.play();
	
	std::cin.get();
	music.stop();
}
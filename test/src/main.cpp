///
/// main.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include <iostream>

#include <pl/Log.hpp>

int main(int argc, char** argv)
{
	pl::Log::i().init("logs/a.txt");
	pl::Log::i().setMinimumLevel(pl::Log::Level::WARNING);

	PL_LOG(pl::Log::Level::INFO, "Should not log.\n");
	PL_LOG(pl::Log::Level::WARNING, "Should Log.");

	std::cin.get();
	
	return 0;
}
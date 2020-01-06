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
	pl::Log::i().setMinimumLevel(pl::Log::Level::INFO);

	PL_LOG(pl::Log::Level::INFO, "Should not log unless INFO is min level.");
	PL_LOG(pl::Log::Level::WARNING, "Should Log.");

	std::cin.get();
	
	return 0;
}
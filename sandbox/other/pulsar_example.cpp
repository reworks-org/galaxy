///
/// main.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include <iostream>

#include <pulsar/Log.hpp>

int main(int argc, char** argv)
{
	pulsar::Log::get().init("logs/a.txt");
	pulsar::Log::get().setMinimumLevel(pulsar::Log::Level::INFO);

	PL_LOG(PL_INFO, "Should not log unless INFO is min level.");
	PL_LOG(PL_WARNING, "Should Log.");

	std::cin.get();
	
	return 0;
}
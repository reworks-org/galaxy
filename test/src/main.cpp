///
/// main.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#include <pl/Log.hpp>

int main(int argc, char** argv)
{
	pl::Log::init("logs/a.txt");
	pl::Log::setMinimumLevel(pl::Log::Level::WARNING);

	LOG_S(pl::Log::Level::INFO) << "Should not log.\n";
	LOG_S(pl::Log::Level::WARNING) << "Shoud Log.";

	std::cin.get();
	
	return 0;
}
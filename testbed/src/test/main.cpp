///
/// main.cpp
///
/// solar
/// See LICENSE.txt.
///

#include "gtest/gtest.h"
#include <solar/entity/Heliosphere.hpp>

struct a { a(int newvar) { var = newvar; } int var = 100; };
struct b { int var = 200; };

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	auto result = RUN_ALL_TESTS();

	sr::Heliosphere hs;

	sr::Entity ea = hs.create();
	//sr::Entity eb = hs.create();
	//sr::Entity ec = hs.create();
	hs.add<a>(ea, 50);
	hs.add<b>(ea);

	//std::cout << "ea: " << ea << "\neb: " << eb << "\nec: " << ec << std::endl;

	auto t = hs.get<a, b>(ea);

	//std::cout << sr::cuid::uid<decltype(std::get<0>(t))>() << std::endl;
	//std::cout << sr::cuid::uid<decltype(std::get<1>(t))>() << std::endl;

	std::cout << std::get<0>(t)->var << std::endl;
	std::cout << std::get<1>(t)->var << std::endl;

	std::cin.get();

	return result;
}
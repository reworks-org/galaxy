///
/// PhysfsInputStream.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "PhysfsInputStream.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	PhysfsInputStream::PhysfsInputStream()
	{
	}
	PhysfsInputStream::PhysfsInputStream(const std::string& file)
	{
	}
	PhysfsInputStream::~PhysfsInputStream()
	{
	}
	bool PhysfsInputStream::open(const std::string& file)
	{
		return false;
	}
	sf::Int64 PhysfsInputStream::read(void* data, sf::Int64 size)
	{
		return sf::Int64();
	}
	sf::Int64 PhysfsInputStream::seek(sf::Int64 position)
	{
		return sf::Int64();
	}
	sf::Int64 PhysfsInputStream::tell()
	{
		return sf::Int64();
	}
	sf::Int64 PhysfsInputStream::getSize()
	{
		return sf::Int64();
	}
}
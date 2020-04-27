///
/// Serializer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Serializer.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Serializer::Serializer() noexcept
		:m_folder("saves/")
	{	
	}

	Serializer::Serializer(const std::string& folder) noexcept
		:m_folder(folder)
	{
	}

	Serializer::~Serializer() noexcept
	{
		m_objects.clear();
	}

	void Serializer::add(Serializable* serializable) noexcept
	{
		m_objects.push_back(serializable);
	}

	void Serializer::setFolder(const std::string& folder) noexcept
	{
		m_folder = folder;
	}

	void Serializer::serialize() noexcept
	{
		// todo.
	}

	void Serializer::deserialize(const std::string& file)
	{
		// todo.
	}
}
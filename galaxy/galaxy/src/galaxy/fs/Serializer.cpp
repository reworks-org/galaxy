///
/// Serializer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "Serializer.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Serializer::Serializer() noexcept
		:m_folder("saves/"), m_count(0)
	{	
	}

	Serializer::Serializer(const std::string& folder) noexcept
		:m_folder(folder), m_count(0)
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
		nlohmann::json json = nlohmann::json::object();
		for (Serializable* s : m_objects)
		{
			auto opt = s->serialize();
			if (opt != std::nullopt)
			{
				json.push_back(opt.value());
			}
		}

		std::ofstream write;

		write.open(std::filesystem::path(m_folder).string() + "Save0" + std::to_string(m_count) + ".save", std::ofstream::out | std::ofstream::trunc);
		write << json << std::endl;
		write.close();
	}

	void Serializer::deserialize(const std::string& file)
	{
		nlohmann::json json;
		std::ifstream read;
		read.open(std::filesystem::path(file).string(), std::ifstream::in);
		read >> json;
		read.close();

		unsigned int count = 0;
		for (auto& element : json)
		{
			m_objects[count]->deserialize(element);
			count++;
		}
	}
}
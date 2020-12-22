///
/// Serializer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/scripting/JSONUtils.hpp"

#include "Serializer.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Serializer::Serializer()
	    : m_counter {0}
	{
	}

	Serializer::~Serializer()
	{
		m_objects.clear();
	}

	unsigned int Serializer::register_obj(Serializable* s)
	{
		const auto id = m_counter++;
		s->m_id       = id;

		m_objects[id] = s;
		return id;
	}

	void Serializer::remove(const unsigned int id)
	{
		if (m_objects.contains(id))
		{
			m_objects.erase(id);
		}
	}

	void Serializer::serialize(std::string_view file)
	{
		nlohmann::json root;
		for (auto& [id, obj] : m_objects)
		{
			root[std::to_string(id)] = *obj;
		}

		json::save_to_disk(file, root);
	}

	void Serializer::deserialize(std::string_view file)
	{
		nlohmann::json json = json::parse_from_disk(file);
		for (auto& [key, value] : json.items())
		{
			value.get_to(*m_objects[std::stol(key)]);
		}
	}
} // namespace galaxy
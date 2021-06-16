///
///	Renderable.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>

#include "Renderable.hpp"

namespace galaxy
{
	namespace components
	{
		Renderable::Renderable() noexcept
		    : Serializable {this}, m_type {graphics::Renderables::BATCHED}, m_z_level {0}
		{
		}

		Renderable::Renderable(const graphics::Renderables type, const int z_level) noexcept
		    : Serializable {this}, m_type {type}, m_z_level {z_level}
		{
		}

		Renderable::Renderable(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		Renderable::Renderable(Renderable&& r) noexcept
		    : Serializable {this}
		{
			this->m_type    = r.m_type;
			this->m_z_level = r.m_z_level;
		}

		Renderable& Renderable::operator=(Renderable&& r) noexcept
		{
			if (this != &r)
			{
				this->m_type    = r.m_type;
				this->m_z_level = r.m_z_level;
			}

			return *this;
		}

		nlohmann::json Renderable::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["type"]        = static_cast<std::string>(magic_enum::enum_name<graphics::Renderables>(m_type));
			json["z-level"]     = m_z_level;

			return json;
		}

		void Renderable::deserialize(const nlohmann::json& json)
		{
			// clang-format off
			m_type = magic_enum::enum_cast<graphics::Renderables>(json.at("type").get<std::string>()).value();
			m_z_level = json.at("z-level");
			// clang-format on
		}
	} // namespace components
} // namespace galaxy
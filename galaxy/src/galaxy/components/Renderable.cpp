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
		    : Serializable {this}, m_type {graphics::Renderables::SPRITE}
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
			this->m_type = r.m_type;
		}

		Renderable& Renderable::operator=(Renderable&& r) noexcept
		{
			if (this != &r)
			{
				this->m_type = r.m_type;
			}

			return *this;
		}

		nlohmann::json Renderable::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["type"]        = static_cast<std::string>(magic_enum::enum_name<graphics::Renderables>(m_type));

			return json;
		}

		void Renderable::deserialize(const nlohmann::json& json)
		{
			m_type = magic_enum::enum_cast<graphics::Renderables>(json.at("type").get<std::string>()).value();
		}
	} // namespace components
} // namespace galaxy
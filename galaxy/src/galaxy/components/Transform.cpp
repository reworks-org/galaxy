///
/// Transform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Transform.hpp"

namespace galaxy
{
	namespace components
	{
		Transform::Transform()
			: Serializable {}
		{
		}

		Transform::Transform(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Transform::Transform(Transform&& t)
			: Serializable {}
		{
		}

		Transform& Transform::operator=(Transform&& t)
		{
			if (this != &t)
			{
				this->m_tf = std::move(t.m_tf);
			}

			return *this;
		}

		Transform::~Transform()
		{
		}

		nlohmann::json Transform::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["pos"]["x"]    = m_tf.get_pos().x;
			json["pos"]["y"]    = m_tf.get_pos().y;
			json["rotation"]    = m_tf.get_rotation();
			json["scale"]["x"]  = m_tf.get_scale().x;
			json["scale"]["y"]  = m_tf.get_scale().y;
			json["origin"]["x"] = m_tf.get_origin().x;
			json["origin"]["y"] = m_tf.get_origin().y;

			return json;
		}

		void Transform::deserialize(const nlohmann::json& json)
		{
			const auto& pos = json.at("pos");
			m_tf.set_pos(pos.at("x"), pos.at("y"));

			m_tf.set_rotation(json.at("rotation"));

			const auto& scale = json.at("scale");
			m_tf.set_scale_horizontal(scale.at("x"));
			m_tf.set_scale_vertical(scale.at("y"));

			const auto& origin = json.at("origin");
			m_tf.set_origin(origin.at("x"), origin.at("y"));
		}
	} // namespace components
} // namespace galaxy

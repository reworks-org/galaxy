///
/// Primitive.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>
#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"

#include "Primitive.hpp"

namespace galaxy
{
	namespace components
	{
		Primitive::Primitive()
			: Serializable {}
			, m_width {0}
			, m_height {0}
			, m_shape {math::Shape::POLYGON}
			, m_mode {0}
			, m_layer {0}
		{
		}

		Primitive::Primitive(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Primitive::Primitive(Primitive&& p)
			: Serializable {}
		{
			this->m_colour         = std::move(p.m_colour);
			this->m_width          = p.m_width;
			this->m_height         = p.m_height;
			this->m_shape          = p.m_shape;
			this->m_data.fragments = p.m_data.fragments;
			this->m_data.points    = std::move(p.m_data.points);
			this->m_data.radii     = std::move(p.m_data.radii);
			this->m_data.radius    = p.m_data.radius;
			this->m_data.start_end = std::move(p.m_data.start_end);
			this->m_vao            = std::move(p.m_vao);
			this->m_mode           = p.m_mode;
			this->m_layer          = p.m_layer;
		}

		Primitive& Primitive::operator=(Primitive&& p)
		{
			if (this != &p)
			{
				this->m_colour         = std::move(p.m_colour);
				this->m_width          = p.m_width;
				this->m_height         = p.m_height;
				this->m_shape          = p.m_shape;
				this->m_data.fragments = p.m_data.fragments;
				this->m_data.points    = std::move(p.m_data.points);
				this->m_data.radii     = std::move(p.m_data.radii);
				this->m_data.radius    = p.m_data.radius;
				this->m_data.start_end = std::move(p.m_data.start_end);
				this->m_vao            = std::move(p.m_vao);
				this->m_mode           = p.m_mode;
				this->m_layer          = p.m_layer;
			}

			return *this;
		}

		Primitive::~Primitive()
		{
		}

		const Primitive::PrimitiveData& Primitive::get_data() const
		{
			return m_data;
		}

		math::Shape Primitive::get_shape() const
		{
			return m_shape;
		}

		float Primitive::get_width() const
		{
			return m_width;
		}

		float Primitive::get_height() const
		{
			return m_height;
		}

		int Primitive::get_instances() const
		{
			return 1;
		}

		unsigned int Primitive::get_mode() const
		{
			return m_mode;
		}

		unsigned int Primitive::get_vao() const
		{
			return m_vao.id();
		}

		unsigned int Primitive::get_texture() const
		{
			return 0;
		}

		unsigned int Primitive::get_count() const
		{
			return m_vao.index_count();
		}

		int Primitive::get_layer() const
		{
			return m_layer;
		}

		nlohmann::json Primitive::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["shape"]       = std::string(magic_enum::enum_name(m_shape));
			json["layer"]       = m_layer;
			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			switch (m_shape)
			{
				case math::Shape::CIRCLE:
					json["radius"]    = m_data.radius;
					json["fragments"] = m_data.fragments;
					break;

				case math::Shape::ELLIPSE:
					json["radii"]      = nlohmann::json::object();
					json["radii"]["x"] = m_data.radii.x;
					json["radii"]["y"] = m_data.radii.y;
					json["fragments"]  = m_data.fragments;
					break;

				case math::Shape::LINE:
					json["line_points"]       = nlohmann::json::object();
					json["line_points"]["x1"] = m_data.start_end.x;
					json["line_points"]["y1"] = m_data.start_end.y;
					json["line_points"]["x2"] = m_data.start_end.z;
					json["line_points"]["y2"] = m_data.start_end.w;
					break;

				case math::Shape::POLYGON:
				case math::Shape::POLYLINE:
					json["points"] = nlohmann::json::array();
					for (const auto& point : m_data.points)
					{
						auto obj = nlohmann::json::object();
						obj["x"] = point.x;
						obj["y"] = point.y;

						json["points"].push_back(obj);
					}
					break;
			}

			return json;
		}

		void Primitive::deserialize(const nlohmann::json& json)
		{
			Primitive::PrimitiveData data;

			if (json.count("radius") > 0)
			{
				data.radius = json.at("radius").get<float>();
			}

			if (json.count("radii") > 0)
			{
				const auto& radii = json.at("radii");
				data.radii.x      = radii.at("x");
				data.radii.y      = radii.at("y");
			}

			if (json.count("fragments") > 0)
			{
				data.fragments = json.at("fragments").get<float>();
			}

			if (json.count("line_points") > 0)
			{
				const auto& points = json.at("line_points");
				data.start_end.x   = points.at("x1");
				data.start_end.y   = points.at("y1");
				data.start_end.z   = points.at("x2");
				data.start_end.w   = points.at("y2");
			}

			if (json.count("points") > 0)
			{
				meta::vector<glm::vec2> points;

				const auto& points_array = json.at("points");
				for (const auto& point : points_array)
				{
					data.points.emplace_back(point.at("x"), point.at("y"));
				}
			}

			const auto& col_json = json.at("colour");
			const auto  colour   = graphics::Colour {col_json.at("r"), col_json.at("g"), col_json.at("b"), col_json.at("a")};

			const int  layer     = json.at("layer");
			const auto shape_opt = magic_enum::enum_cast<math::Shape>(json.at("shape").get<std::string>());
			if (shape_opt.has_value())
			{
				const auto shape = shape_opt.value();
				switch (shape)
				{
					case math::Shape::CIRCLE:
						create<math::Shape::CIRCLE>(data, colour, layer);
						break;

					case math::Shape::ELLIPSE:
						create<math::Shape::ELLIPSE>(data, colour, layer);
						break;

					case math::Shape::LINE:
						create<math::Shape::LINE>(data, colour, layer);
						break;

					case math::Shape::POINT:
						create<math::Shape::POINT>(data, colour, layer);
						break;

					case math::Shape::POLYGON:
						create<math::Shape::POLYGON>(data, colour, layer);
						break;

					case math::Shape::POLYLINE:
						create<math::Shape::POLYLINE>(data, colour, layer);
						break;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load shape enum from json, make sure it matches enum name exactly, including case.");
			}
		}
	} // namespace components
} // namespace galaxy

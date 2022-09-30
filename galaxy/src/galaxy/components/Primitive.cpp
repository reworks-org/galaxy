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
		Primitive::Primitive() noexcept
			: Renderable {}
			, Serializable {}
			, m_width {0}
			, m_height {0}
			, m_shape {graphics::Shape::POLYGON}
			, m_colour {0, 0, 0, 255}
		{
		}

		Primitive::Primitive(const nlohmann::json& json)
			: Renderable {}
			, Serializable {}
			, m_width {0}
			, m_height {0}
			, m_shape {graphics::Shape::POLYGON}
			, m_colour {0, 0, 0, 255}
		{
			deserialize(json);
		}

		Primitive::Primitive(Primitive&& p) noexcept
			: Renderable {std::move(p)}
			, Serializable {}
			, m_width {0}
			, m_height {0}
			, m_shape {graphics::Shape::POLYGON}
			, m_colour {0, 0, 0, 255}
		{
			this->m_width  = p.m_width;
			this->m_height = p.m_height;
			this->m_shape  = p.m_shape;
			this->m_data   = std::move(p.m_data);
			this->m_vao    = std::move(p.m_vao);
			this->m_colour = std::move(p.m_colour);
		}

		Primitive& Primitive::operator=(Primitive&& p) noexcept
		{
			if (this != &p)
			{
				this->Renderable::operator=(std::move(p));

				this->m_width  = p.m_width;
				this->m_height = p.m_height;
				this->m_shape  = p.m_shape;
				this->m_data   = std::move(p.m_data);
				this->m_vao    = std::move(p.m_vao);
				this->m_colour = std::move(p.m_colour);
			}

			return *this;
		}

		Primitive::~Primitive() noexcept
		{
		}

		const Primitive::PrimitiveData& Primitive::get_data() const noexcept
		{
			return m_data;
		}

		graphics::Shape Primitive::get_shape() const noexcept
		{
			return m_shape;
		}

		float Primitive::get_width() const noexcept
		{
			return m_width;
		}

		float Primitive::get_height() const noexcept
		{
			return m_height;
		}

		void Primitive::configure() noexcept
		{
			m_vao_id      = m_vao.id();
			m_index_count = m_vao.index_count();
			m_instances   = 1;
		}

		nlohmann::json Primitive::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["shape"]       = static_cast<std::string>(magic_enum::enum_name(m_shape));
			json["layer"]       = m_layer;
			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			switch (m_shape)
			{
				case graphics::Shape::CIRCLE:
					json["radius"]    = m_data.m_radius;
					json["fragments"] = m_data.m_fragments;
					break;

				case graphics::Shape::ELLIPSE:
					json["radii"]      = nlohmann::json::object();
					json["radii"]["x"] = m_data.m_radii.x;
					json["radii"]["y"] = m_data.m_radii.y;
					json["fragments"]  = m_data.m_fragments;
					break;

				case graphics::Shape::LINE:
					json["line-points"]       = nlohmann::json::object();
					json["line-points"]["x1"] = m_data.m_start_end.x;
					json["line-points"]["y1"] = m_data.m_start_end.y;
					json["line-points"]["x2"] = m_data.m_start_end.z;
					json["line-points"]["y2"] = m_data.m_start_end.w;
					break;

				case graphics::Shape::POLYGON:
					json["points"] = nlohmann::json::array();
					for (const auto& point : m_data.m_points)
					{
						json["points"].push_back(nlohmann::json::object({{"x", point.x}, {"y", point.y}}));
					}
					break;

				case graphics::Shape::POLYLINE:
					json["points"] = nlohmann::json::array();
					for (const auto& point : m_data.m_points)
					{
						json["points"].push_back(nlohmann::json::object({{"x", point.x}, {"y", point.y}}));
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
				data.m_radius = json.at("radius").get<float>();
			}

			if (json.count("radii") > 0)
			{
				const auto& radii = json.at("radii");
				data.m_radii.x    = radii.at("x");
				data.m_radii.y    = radii.at("y");
			}

			if (json.count("fragments") > 0)
			{
				data.m_fragments = json.at("fragments").get<float>();
			}

			if (json.count("line-points") > 0)
			{
				const auto& points = json.at("line-points");
				data.m_start_end.x = points.at("x1");
				data.m_start_end.y = points.at("y1");
				data.m_start_end.z = points.at("x2");
				data.m_start_end.w = points.at("y2");
			}

			if (json.count("points") > 0)
			{
				std::vector<glm::vec2> points;

				const auto& points_array = json.at("points");
				for (const auto& point : points_array)
				{
					data.m_points.emplace_back(point.at("x"), point.at("y"));
				}
			}

			const auto& col_json = json.at("colour");
			const auto colour    = graphics::Colour {col_json.at("r"), col_json.at("g"), col_json.at("b"), col_json.at("a")};

			const int layer      = json.at("layer");
			const auto shape_opt = magic_enum::enum_cast<graphics::Shape>(json.at("shape").get<std::string>());
			if (shape_opt.has_value())
			{
				const auto shape = shape_opt.value();
				switch (shape)
				{
					case graphics::Shape::CIRCLE:
						create<graphics::Shape::CIRCLE>(data, colour, layer);
						break;

					case graphics::Shape::ELLIPSE:
						create<graphics::Shape::ELLIPSE>(data, colour, layer);
						break;

					case graphics::Shape::LINE:
						create<graphics::Shape::LINE>(data, colour, layer);
						break;

					case graphics::Shape::POINT:
						create<graphics::Shape::POINT>(data, colour, layer);
						break;

					case graphics::Shape::POLYGON:
						create<graphics::Shape::POLYGON>(data, colour, layer);
						break;

					case graphics::Shape::POLYLINE:
						create<graphics::Shape::POLYLINE>(data, colour, layer);
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
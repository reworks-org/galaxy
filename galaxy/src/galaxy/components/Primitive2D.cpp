///
/// Primitive2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>

#include "Primitive2D.hpp"

namespace galaxy
{
	namespace components
	{
		Primitive2D::Primitive2D() noexcept
		    : Serializable {this}, m_width {0}, m_height {0}, m_type {graphics::Primitives::CIRCLE}, m_colour {0, 0, 0, 255}, m_layer {""}
		{
		}

		Primitive2D::Primitive2D(const nlohmann::json& json)
		    : Serializable {this}
		{
			deserialize(json);
		}

		Primitive2D::Primitive2D(Primitive2D&& p2d) noexcept
		    : Serializable {this}
		{
			this->m_colour  = std::move(p2d.m_colour);
			this->m_data    = std::move(p2d.m_data);
			this->m_height  = p2d.m_height;
			this->m_type    = p2d.m_type;
			this->m_vao     = std::move(p2d.m_vao);
			this->m_vertexs = std::move(p2d.m_vertexs);
			this->m_width   = p2d.m_width;
			this->m_layer   = std::move(p2d.m_layer);
		}

		Primitive2D& Primitive2D::operator=(Primitive2D&& p2d) noexcept
		{
			if (this != &p2d)
			{
				this->m_colour  = std::move(p2d.m_colour);
				this->m_data    = std::move(p2d.m_data);
				this->m_height  = p2d.m_height;
				this->m_type    = p2d.m_type;
				this->m_vao     = std::move(p2d.m_vao);
				this->m_vertexs = std::move(p2d.m_vertexs);
				this->m_width   = p2d.m_width;
				this->m_layer   = std::move(p2d.m_layer);
			}

			return *this;
		}

		Primitive2D::~Primitive2D() noexcept
		{
			m_vertexs.clear();
		}

		void Primitive2D::set_colour(const graphics::Colour& col)
		{
			m_colour = col;
		}

		void Primitive2D::set_opacity(const std::uint8_t opacity) noexcept
		{
			m_colour.m_alpha = std::clamp<std::uint8_t>(opacity, 0, 255);
		}

		graphics::Colour& Primitive2D::get_colour() noexcept
		{
			return m_colour;
		}

		Primitive2D::PrimitiveData& Primitive2D::get_data() noexcept
		{
			return m_data;
		}

		const graphics::Primitives Primitive2D::get_type() const noexcept
		{
			return m_type;
		}

		std::vector<graphics::Vertex>& Primitive2D::get_vertices() noexcept
		{
			return m_vertexs;
		}

		const int Primitive2D::get_width() const noexcept
		{
			return m_width;
		}

		const int Primitive2D::get_height() const noexcept
		{
			return m_height;
		}

		const std::string& Primitive2D::get_layer() const noexcept
		{
			return m_layer;
		}

		const unsigned int Primitive2D::vao() const noexcept
		{
			return m_vao.id();
		}

		const int Primitive2D::count() const noexcept
		{
			return m_vao.count();
		}

		nlohmann::json Primitive2D::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["type"]        = static_cast<std::string>(magic_enum::enum_name(m_type));
			json["layer"]       = m_layer;
			json["colour"]      = nlohmann::json::object();
			json["colour"]["r"] = m_colour.m_red;
			json["colour"]["g"] = m_colour.m_green;
			json["colour"]["b"] = m_colour.m_blue;
			json["colour"]["a"] = m_colour.m_alpha;

			if (m_data.m_radius != std::nullopt)
			{
				json["radius"] = *m_data.m_radius;
			}

			if (m_data.m_radii != std::nullopt)
			{
				const auto& radii = *m_data.m_radii;

				json["radii"]      = nlohmann::json::object();
				json["radii"]["x"] = radii.x;
				json["radii"]["y"] = radii.y;
			}

			if (m_data.m_fragments != std::nullopt)
			{
				json["fragments"] = *m_data.m_fragments;
			}

			if (m_data.m_start_end != std::nullopt)
			{
				const auto& points = *m_data.m_start_end;

				json["line-points"]       = nlohmann::json::object();
				json["line-points"]["x1"] = points.x;
				json["line-points"]["y1"] = points.y;
				json["line-points"]["x2"] = points.z;
				json["line-points"]["y2"] = points.w;
			}

			if (m_data.m_points != std::nullopt)
			{
				json["points"] = nlohmann::json::array();
				for (const auto& point : *m_data.m_points)
				{
					json["points"].push_back(nlohmann::json::object({{"x", point.x}, {"y", point.y}}));
				}
			}

			return json;
		}

		void Primitive2D::deserialize(const nlohmann::json& json)
		{
			Primitive2D::PrimitiveData data;
			m_type            = magic_enum::enum_cast<graphics::Primitives>(json.at("type").get<std::string>()).value();
			std::string layer = json.at("layer");

			const auto& col_json    = json.at("colour");
			graphics::Colour colour = {col_json.at("r"), col_json.at("g"), col_json.at("b"), col_json.at("a")};

			if (json.count("radius") > 0)
			{
				data.m_radius = json.at("radius").get<float>();
			}

			if (json.count("radii") > 0)
			{
				const auto& radii = json.at("radii");
				data.m_radii      = std::make_optional<glm::vec2>(radii.at("x"), radii.at("y"));
			}

			if (json.count("fragments") > 0)
			{
				data.m_fragments = json.at("fragments").get<float>();
			}

			if (json.count("line-points") > 0)
			{
				const auto& points = json.at("line-points");
				data.m_start_end   = std::make_optional<glm::vec4>(
                                    points.at("x1"),
                                    points.at("y1"),
                                    points.at("x2"),
                                    points.at("y2"));
			}

			if (json.count("points") > 0)
			{
				std::vector<glm::vec2> points;

				const auto& points_array = json.at("points");
				for (const auto& point : points_array)
				{
					points.emplace_back(point.at("x"), point.at("y"));
				}

				data.m_points = std::make_optional(points);
			}

			switch (m_type)
			{
				case graphics::Primitives::CIRCLE:
					create<graphics::Primitives::CIRCLE>(data, colour, layer);
					break;

				case graphics::Primitives::ELLIPSE:
					create<graphics::Primitives::ELLIPSE>(data, colour, layer);
					break;

				case graphics::Primitives::LINE:
					create<graphics::Primitives::LINE>(data, colour, layer);
					break;

				case graphics::Primitives::POINT:
					create<graphics::Primitives::POINT>(data, colour, layer);
					break;

				case graphics::Primitives::POLYGON:
					create<graphics::Primitives::POLYGON>(data, colour, layer);
					break;

				case graphics::Primitives::POLYLINE:
					create<graphics::Primitives::POLYLINE>(data, colour, layer);
					break;
			}
		}
	} // namespace components
} // namespace galaxy
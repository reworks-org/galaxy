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
			: Serializable {this}
			, m_width {0}
			, m_height {0}
			, m_type {graphics::Primitives::CIRCLE}
			, m_colour {0, 0, 0, 255}
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

		const float Primitive2D::get_width() const noexcept
		{
			return m_width;
		}

		const float Primitive2D::get_height() const noexcept
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

		const int Primitive2D::index_count() const noexcept
		{
			return m_vao.index_count();
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

			switch (m_type)
			{
				case graphics::Primitives::CIRCLE:
					json["radius"]    = m_data.m_radius;
					json["fragments"] = m_data.m_fragments;
					break;

				case graphics::Primitives::ELLIPSE:
					json["radii"]      = nlohmann::json::object();
					json["radii"]["x"] = m_data.m_radii.x;
					json["radii"]["y"] = m_data.m_radii.y;
					json["fragments"]  = m_data.m_fragments;
					break;

				case graphics::Primitives::LINE:
					json["line-points"]       = nlohmann::json::object();
					json["line-points"]["x1"] = m_data.m_start_end.x;
					json["line-points"]["y1"] = m_data.m_start_end.y;
					json["line-points"]["x2"] = m_data.m_start_end.z;
					json["line-points"]["y2"] = m_data.m_start_end.w;
					break;

				case graphics::Primitives::POLYGON:
					json["points"] = nlohmann::json::array();
					for (const auto& point : m_data.m_points)
					{
						json["points"].push_back(nlohmann::json::object({{"x", point.x}, {"y", point.y}}));
					}
					break;

				case graphics::Primitives::POLYLINE:
					json["points"] = nlohmann::json::array();
					for (const auto& point : m_data.m_points)
					{
						json["points"].push_back(nlohmann::json::object({{"x", point.x}, {"y", point.y}}));
					}
					break;
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
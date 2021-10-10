///
/// Primitive2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_PRIMITIVE2D_HPP_
#define GALAXY_COMPONENTS_PRIMITIVE2D_HPP_

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Primitives.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// 2D primitive shape.
		///
		class Primitive2D final : public fs::Serializable
		{
		public:
			///
			/// Holds variables relating to constructing primitive's vertexs.
			///
			struct PrimitiveData final
			{
				///
				/// Used for Circle.
				///
				float m_radius = 1.0f;

				///
				/// Used for Ellipse.
				///
				glm::vec2 m_radii = {1.0f, 1.0f};

				///
				/// Used for Circle and Ellipse.
				///
				float m_fragments = 1.0f;

				///
				/// Used by line. Contains start and end point.
				///
				glm::vec4 m_start_end = {0.0f, 0.0f, 1.0f, 1.0f};

				///
				/// Used by Polyline and Polygon.
				///
				std::vector<glm::vec2> m_points;
			};

		public:
			///
			/// Constructor.
			///
			Primitive2D() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Primitive2D(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Primitive2D(Primitive2D&&) noexcept;

			///
			/// Move assignment operator.
			///
			Primitive2D& operator=(Primitive2D&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Primitive2D() noexcept;

			///
			/// Create the primitive vertexs.
			///
			/// \param data Contains variables to construct vertex's for a primitive.
			/// \param colour Colour of primitive.
			/// \param layer Rendering layer.
			///
			template<graphics::Primitives type>
			void create(const PrimitiveData& data, const graphics::Colour& colour, std::string_view layer);

			///
			/// Set RGBA colour.
			///
			/// \param col Colour of primitive.
			///
			void set_colour(const graphics::Colour& col);

			///
			/// Set opacity.
			///
			/// \param opacity 0 - 255.
			///
			void set_opacity(const std::uint8_t opacity) noexcept;

			///
			/// Get colour.
			///
			/// \return Reference to Colour object.
			///
			[[nodiscard]] graphics::Colour& get_colour() noexcept;

			///
			/// Get current primitive data.
			///
			/// \return Reference to data. Can be modified.
			///
			[[nodiscard]] PrimitiveData& get_data() noexcept;

			///
			/// Get current type of primitive.
			///
			/// \return Const graphics::Primitive enum.
			///
			[[nodiscard]] const graphics::Primitives get_type() const noexcept;

			///
			/// Get current primitive vertexs.
			///
			/// \return Const reference to vertexs.
			///
			[[nodiscard]] std::vector<graphics::Vertex>& get_vertices() noexcept;

			///
			/// Get width of primitive.
			///
			/// \return Const float. 0 if object does not have.
			///
			[[nodiscard]] const float get_width() const noexcept;

			///
			/// Get height of primitive.
			///
			/// \return Const float. 0 if object does not have.
			///
			[[nodiscard]] const float get_height() const noexcept;

			///
			/// Get rendering layer.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_layer() const noexcept;

			///
			/// Get GL VAO.
			///
			/// \return Const uint.
			///
			[[nodiscard]] const unsigned int vao() const noexcept;

			///
			/// Gets the index count.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int index_count() const noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy assignment operator.
			///
			Primitive2D& operator=(const Primitive2D&) = delete;

			///
			/// Copy constructor.
			///
			Primitive2D(const Primitive2D&) = delete;

		private:
			///
			/// Width.
			///
			float m_width;

			///
			/// Height.
			///
			float m_height;

			///
			/// Type of primitive.
			///
			graphics::Primitives m_type;

			///
			/// Used by all primitives.
			///
			graphics::Colour m_colour;
			///
			/// Creation data.
			///
			PrimitiveData m_data;

			///
			/// GPU vertex data.
			///
			graphics::VertexArray m_vao;

			///
			/// CPU vertex data.
			///
			std::vector<graphics::Vertex> m_vertexs;

			///
			/// Rendering layer.
			///
			std::string m_layer;
		};

		template<graphics::Primitives type>
		inline void Primitive2D::create(const PrimitiveData& data, const graphics::Colour& colour, std::string_view layer)
		{
			m_type  = type;
			m_data  = data;
			m_layer = static_cast<std::string>(layer);
			set_colour(colour);

			m_vertexs.clear();
			std::vector<unsigned int> indices;

			if constexpr (type == graphics::Primitives::CIRCLE)
			{
				// Thanks to https://stackoverflow.com/a/33859443
				// For help with maths.

				unsigned int    count     = 0;
				constexpr float incr_stat = 2.0f * glm::pi<float>();
				const float     increment = incr_stat / data.m_fragments;
				for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
				{
					graphics::Vertex vertex;
					vertex.m_pos.x = (data.m_radius * glm::cos(angle)) + data.m_radius;
					vertex.m_pos.y = (data.m_radius * glm::sin(angle) + data.m_radius);

					m_vertexs.emplace_back(vertex);
					indices.push_back(count);

					count++;
				}

				graphics::VertexBuffer vbo;
				graphics::IndexBuffer  ibo;

				vbo.create(m_vertexs, true);
				ibo.create(indices, true);
				m_vao.create(vbo, ibo);

				m_width  = data.m_radius * 2.0f;
				m_height = data.m_radius * 2.0f;
			}
			else if constexpr (type == graphics::Primitives::ELLIPSE)
			{
				// Thanks to https://stackoverflow.com/a/34735255
				// For help with maths.

				const float theta  = 2.0f * glm::pi<float>() / m_data.m_fragments;
				const float cosine = std::cosf(theta);
				const float sine   = std::sinf(theta);

				float temp = 0.0f;
				float x    = 1.0f;
				float y    = 0.0f;

				unsigned int count = 0;
				for (auto i = 0; i < std::floor(m_data.m_fragments); i++)
				{
					graphics::Vertex vertex;
					vertex.m_pos.x = (x * m_data.m_radii.x) + m_data.m_radii.x;
					vertex.m_pos.y = (y * m_data.m_radii.y) + m_data.m_radii.y;

					m_vertexs.emplace_back(vertex);
					indices.push_back(count);
					count++;

					temp = x;
					x    = cosine * x - sine * y;
					y    = sine * temp + cosine * y;
				}

				graphics::VertexBuffer vbo;
				graphics::IndexBuffer  ibo;

				vbo.create(m_vertexs, true);
				ibo.create(indices, true);
				m_vao.create(vbo, ibo);

				m_width  = m_data.m_radii.x * 2.0f;
				m_height = m_data.m_radii.y * 2.0f;
			}
			else if constexpr (type == graphics::Primitives::LINE)
			{
				graphics::Vertex vertex;

				vertex.m_pos.x = m_data.m_start_end.x;
				vertex.m_pos.y = m_data.m_start_end.y;
				m_vertexs.emplace_back(vertex);

				vertex.m_pos.x = m_data.m_start_end.z;
				vertex.m_pos.y = m_data.m_start_end.w;
				m_vertexs.emplace_back(vertex);

				indices.push_back(0);
				indices.push_back(1);

				graphics::VertexBuffer vbo;
				graphics::IndexBuffer  ibo;

				vbo.create(m_vertexs, true);
				ibo.create(indices, true);
				m_vao.create(vbo, ibo);

				m_width  = 0.0f;
				m_height = 0.0f;
			}
			else if constexpr (type == graphics::Primitives::POINT)
			{
				graphics::Vertex vertex;

				vertex.m_pos = {0.0f, 0.0f};
				m_vertexs.emplace_back(vertex);

				indices.push_back(0);

				graphics::VertexBuffer vbo;
				graphics::IndexBuffer  ibo;

				vbo.create(m_vertexs, true);
				ibo.create(indices, true);
				m_vao.create(vbo, ibo);

				m_width  = 0.0f;
				m_height = 0.0f;
			}
			else if constexpr ((type == graphics::Primitives::POLYLINE) || (type == graphics::Primitives::POLYGON))
			{
				m_width  = 0.0f;
				m_height = 0.0f;

				unsigned int count = 0;
				for (const auto& point : m_data.m_points)
				{
					if constexpr (type == graphics::Primitives::POLYGON)
					{
						if (point.x > m_width)
						{
							m_width = point.x;
						}

						if (point.y > m_height)
						{
							m_height = point.y;
						}
					}

					graphics::Vertex vertex;

					vertex.m_pos.x = point.x;
					vertex.m_pos.y = point.y;

					m_vertexs.emplace_back(vertex);
					indices.push_back(count);

					count++;
				}

				graphics::VertexBuffer vbo;
				graphics::IndexBuffer  ibo;

				vbo.create(m_vertexs, true);
				ibo.create(indices, true);
				m_vao.create(vbo, ibo);
			}
		}
	} // namespace components
} // namespace galaxy

#endif
///
/// Primitive.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_PRIMITIVE_HPP_
#define GALAXY_COMPONENTS_PRIMITIVE_HPP_

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Primitives.hpp"
#include "galaxy/graphics/Renderable.hpp"
#include "galaxy/graphics/Shape.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// 2D primitive shape.
		///
		class Primitive final : public graphics::Renderable, public fs::Serializable
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
			Primitive() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Primitive(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Primitive(Primitive&&) noexcept;

			///
			/// Move assignment operator.
			///
			Primitive& operator=(Primitive&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Primitive() noexcept;

			///
			/// Create the primitive vertexs.
			///
			/// \tparam shape Enum shape definition.
			///
			/// \param data Contains variables to construct vertices for a primitive.
			/// \param colour Colour of primitive.
			/// \param layer Rendering layer.
			///
			template<graphics::Shape shape>
			void create(const PrimitiveData& data, const graphics::Colour& colour, const int layer);

			///
			/// Set opacity.
			///
			/// \param opacity 0 - 255.
			///
			void set_opacity(const std::uint8_t opacity) noexcept;

			///
			/// Get current primitive data.
			///
			/// \return Reference to data. Can be modified.
			///
			[[nodiscard]] const PrimitiveData& get_data() const noexcept;

			///
			/// Get current shape.
			///
			/// \return Enum.
			///
			[[nodiscard]] graphics::Shape get_shape() const noexcept;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float get_width() const noexcept;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float get_height() const noexcept;

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
			Primitive& operator=(const Primitive&) = delete;

			///
			/// Copy constructor.
			///
			Primitive(const Primitive&) = delete;

			///
			/// Configure renderable.
			///
			void configure() noexcept override;

		public:
			///
			/// Used by all primitives.
			///
			graphics::Colour m_colour;

		private:
			///
			/// Cached width.
			///
			float m_width;

			///
			/// Cached height.
			///
			float m_height;

			///
			/// Shape of primitive.
			///
			graphics::Shape m_shape;

			///
			/// Creation data.
			///
			PrimitiveData m_data;

			///
			/// GPU vertex data.
			///
			graphics::VertexArray m_vao;
		};

		template<graphics::Shape shape>
		inline void Primitive::create(const PrimitiveData& data, const graphics::Colour& colour, const int layer)
		{
			m_shape = shape;
			m_data  = data;

			set_layer(layer);

			std::vector<unsigned int> indices;
			std::vector<graphics::Vertex> vertices;

			if constexpr (shape == graphics::Shape::CIRCLE)
			{
				// Thanks to https://stackoverflow.com/a/33859443
				// For help with maths.

				auto count                     = 0u;
				constexpr const auto incr_stat = 2.0f * glm::pi<float>();
				const auto increment           = incr_stat / data.m_fragments;

				for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
				{
					graphics::Vertex vertex;
					vertex.m_pos.x = (data.m_radius * glm::cos(angle)) + data.m_radius;
					vertex.m_pos.y = (data.m_radius * glm::sin(angle) + data.m_radius);

					vertices.emplace_back(vertex);
					indices.push_back(count);

					count++;
				}

				set_primitive_type(graphics::Primitives::LINE_LOOP);

				m_width  = data.m_radius * 2.0f;
				m_height = data.m_radius * 2.0f;
			}
			else if constexpr (shape == graphics::Shape::ELLIPSE)
			{
				// Thanks to https://stackoverflow.com/a/34735255
				// For help with maths.

				const auto theta  = 2.0f * glm::pi<float>() / m_data.m_fragments;
				const auto cosine = std::cosf(theta);
				const auto sine   = std::sinf(theta);

				auto temp = 0.0f;
				auto x    = 1.0f;
				auto y    = 0.0f;

				auto count = 0u;
				for (auto i = 0; i < std::floor(m_data.m_fragments); i++)
				{
					graphics::Vertex vertex;
					vertex.m_pos.x = (x * m_data.m_radii.x) + m_data.m_radii.x;
					vertex.m_pos.y = (y * m_data.m_radii.y) + m_data.m_radii.y;

					vertices.emplace_back(vertex);
					indices.push_back(count);
					count++;

					temp = x;
					x    = cosine * x - sine * y;
					y    = sine * temp + cosine * y;
				}

				set_primitive_type(graphics::Primitives::LINE_LOOP);

				m_width  = m_data.m_radii.x * 2.0f;
				m_height = m_data.m_radii.y * 2.0f;
			}
			else if constexpr (shape == graphics::Shape::LINE)
			{
				graphics::Vertex a;
				a.m_pos.x = m_data.m_start_end.x;
				a.m_pos.y = m_data.m_start_end.y;

				graphics::Vertex b;
				b.m_pos.x = m_data.m_start_end.z;
				b.m_pos.y = m_data.m_start_end.w;

				vertices.emplace_back(a);
				vertices.emplace_back(b);
				indices.push_back(0);
				indices.push_back(1);

				set_primitive_type(graphics::Primitives::LINE);

				m_width  = 0.0f;
				m_height = 0.0f;
			}
			else if constexpr (shape == graphics::Shape::POINT)
			{
				graphics::Vertex vertex;
				vertex.m_pos = {0.0f, 0.0f};

				vertices.emplace_back(vertex);
				indices.push_back(0);

				set_primitive_type(graphics::Primitives::POINT);

				m_width  = 0.0f;
				m_height = 0.0f;
			}
			else if constexpr ((shape == graphics::Shape::POLYLINE) || (shape == graphics::Shape::POLYGON))
			{
				m_width  = 0.0f;
				m_height = 0.0f;

				auto count = 0u;
				for (const auto& point : m_data.m_points)
				{
					if constexpr (shape == graphics::Shape::POLYGON)
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

					vertices.emplace_back(vertex);
					indices.push_back(count);

					count++;
				}

				set_primitive_type(graphics::Primitives::LINE_LOOP);
			}

			m_vao.create(vertices, graphics::StorageFlag::STATIC_DRAW, indices, graphics::StorageFlag::STATIC_DRAW);
			configure();
		}
	} // namespace components
} // namespace galaxy

#endif
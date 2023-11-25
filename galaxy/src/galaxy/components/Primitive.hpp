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
#include <glm/vec4.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Primitives.hpp"
#include "galaxy/graphics/Renderable.hpp"
#include "galaxy/graphics/VertexArray.hpp"
#include "galaxy/math/Shape.hpp"

namespace sc
{
	namespace panel
	{
		class EntityEditor;
	} // namespace panel
} // namespace sc

namespace galaxy
{
	namespace systems
	{
		class RenderSystem;
	} // namespace systems

	namespace components
	{
		///
		/// 2D primitive shape.
		///
		class Primitive final : public fs::Serializable, public graphics::Renderable
		{
			friend class sc::panel::EntityEditor;
			friend class systems::RenderSystem;

		  public:
			///
			/// Holds variables relating to constructing primitive's vertexs.
			///
			struct PrimitiveData final
			{
				///
				/// Used for Circle.
				///
				float radius = 1.0f;

				///
				/// Used for Ellipse.
				///
				glm::vec2 radii = {1.0f, 1.0f};

				///
				/// Used for Circle and Ellipse.
				///
				float fragments = 1.0f;

				///
				/// Used by line. Contains start and end point.
				///
				glm::vec4 start_end = {0.0f, 0.0f, 1.0f, 1.0f};

				///
				/// Used by Polyline and Polygon.
				///
				meta::vector<glm::vec2> points;
			};

		  public:
			///
			/// Constructor.
			///
			Primitive();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Primitive(const nlohmann::json& json);

			///
			/// Pointer constructor.
			///
			/// \param ptr Data to COPY into this component.
			///
			Primitive(Primitive* ptr);

			///
			/// Move constructor.
			///
			Primitive(Primitive&&);

			///
			/// Move assignment operator.
			///
			Primitive& operator=(Primitive&&);

			///
			/// Destructor.
			///
			virtual ~Primitive();

			///
			/// Create the primitive vertexs.
			///
			/// \tparam shape Enum shape definition.
			///
			/// \param data Contains variables to construct vertices for a primitive.
			/// \param colour Colour of primitive.
			/// \param layer Rendering layer.
			///
			template<math::Shape shape>
			void create(const PrimitiveData& data, const graphics::Colour& colour, const int layer);

			///
			/// Get current primitive data.
			///
			/// \return Reference to data. Can be modified.
			///
			[[nodiscard]] const PrimitiveData& get_data() const;

			///
			/// Get current shape.
			///
			/// \return Enum.
			///
			[[nodiscard]] math::Shape get_shape() const;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float get_width() const;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float get_height() const;

			///
			/// Number of instances to render.
			///
			/// \return Count for number of instances.
			///
			[[nodiscard]] int get_instances() const override;

			///
			/// Get OpenGL rendering mode.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int get_mode() const override;

			///
			/// Get vertex array object.
			///
			/// \return VAO handle.
			///
			[[nodiscard]] unsigned int get_vao() const override;

			///
			/// Gets OpenGL texture id.
			///
			/// \return unsigned int. 0 if no texture.
			///
			[[nodiscard]] unsigned int get_texture() const override;

			///
			/// Get index (element) buffer count.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int get_count() const override;

			///
			/// Get rendering layer.
			///
			/// \return Integer. 0 is valid as a layer. So are negatives.
			///
			[[nodiscard]] int get_layer() const override;

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
			math::Shape m_shape;

			///
			/// Creation data.
			///
			PrimitiveData m_data;

			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Type to render i.e. GL_LINES, GL_TRIANGLES, etc.
			///
			unsigned int m_mode;

			///
			/// Object z-level for drawing.
			///
			int m_layer;
		};

		template<math::Shape shape>
		inline void Primitive::create(const PrimitiveData& data, const graphics::Colour& colour, const int layer)
		{
			m_shape = shape;
			m_data  = data;
			m_layer = layer;

			meta::vector<unsigned int>     indices;
			meta::vector<graphics::Vertex> vertices;

			if constexpr (shape == math::Shape::CIRCLE)
			{
				// Thanks to https://stackoverflow.com/a/33859443
				// For help with maths.

				auto                 count     = 0u;
				constexpr const auto incr_stat = 2.0f * glm::pi<float>();
				const auto           increment = incr_stat / data.fragments;

				for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
				{
					graphics::Vertex vertex;
					vertex.m_pos.x = (data.radius * glm::cos(angle)) + data.radius;
					vertex.m_pos.y = (data.radius * glm::sin(angle) + data.radius);

					vertices.emplace_back(vertex);
					indices.push_back(count);

					count++;
				}

				m_mode   = graphics::primitive_to_gl(graphics::Primitives::LINE_LOOP);
				m_width  = data.radius * 2.0f;
				m_height = data.radius * 2.0f;
			}
			else if constexpr (shape == math::Shape::ELLIPSE)
			{
				// Thanks to https://stackoverflow.com/a/34735255
				// For help with maths.

				const auto theta  = 2.0f * glm::pi<float>() / m_data.fragments;
				const auto cosine = std::cosf(theta);
				const auto sine   = std::sinf(theta);

				auto temp = 0.0f;
				auto x    = 1.0f;
				auto y    = 0.0f;

				auto count = 0u;
				for (auto i = 0; i < std::floor(m_data.fragments); i++)
				{
					graphics::Vertex vertex;
					vertex.m_pos.x = (x * m_data.radii.x) + m_data.radii.x;
					vertex.m_pos.y = (y * m_data.radii.y) + m_data.radii.y;

					vertices.emplace_back(vertex);
					indices.push_back(count);
					count++;

					temp = x;
					x    = cosine * x - sine * y;
					y    = sine * temp + cosine * y;
				}

				m_mode   = graphics::primitive_to_gl(graphics::Primitives::LINE_LOOP);
				m_width  = m_data.radii.x * 2.0f;
				m_height = m_data.radii.y * 2.0f;
			}
			else if constexpr (shape == math::Shape::LINE)
			{
				graphics::Vertex a;
				a.m_pos.x = m_data.start_end.x;
				a.m_pos.y = m_data.start_end.y;

				graphics::Vertex b;
				b.m_pos.x = m_data.start_end.z;
				b.m_pos.y = m_data.start_end.w;

				vertices.emplace_back(a);
				vertices.emplace_back(b);
				indices.push_back(0);
				indices.push_back(1);

				m_mode   = graphics::primitive_to_gl(graphics::Primitives::LINE);
				m_width  = 0.0f;
				m_height = 0.0f;
			}
			else if constexpr (shape == math::Shape::POINT)
			{
				graphics::Vertex vertex;
				vertex.m_pos = {0.0f, 0.0f};

				vertices.emplace_back(vertex);
				indices.push_back(0);

				m_mode   = graphics::primitive_to_gl(graphics::Primitives::POINT);
				m_width  = 0.0f;
				m_height = 0.0f;
			}
			else if constexpr ((shape == math::Shape::POLYLINE) || (shape == math::Shape::POLYGON))
			{
				m_width  = 0.0f;
				m_height = 0.0f;

				auto count = 0u;
				for (const auto& point : m_data.points)
				{
					if constexpr (shape == math::Shape::POLYGON)
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

				m_mode = graphics::primitive_to_gl(graphics::Primitives::LINE_LOOP);
			}

			m_vao.create(vertices, graphics::StorageFlag::STATIC_DRAW, indices, graphics::StorageFlag::STATIC_DRAW);
		}
	} // namespace components
} // namespace galaxy

#endif

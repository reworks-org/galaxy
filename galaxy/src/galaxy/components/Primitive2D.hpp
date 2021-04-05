///
/// Primitive2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_PRIMITIVE2D_HPP_
#define GALAXY_COMPONENTS_PRIMITIVE2D_HPP_

#include <optional>

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Primitives.hpp"
#include "galaxy/graphics/VertexData.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// 2D primitive shape.
		///
		class Primitive2D final : public fs::Serializable, public graphics::VertexData
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
				std::optional<float> m_radius = std::nullopt;

				///
				/// Used for Ellipse.
				///
				std::optional<glm::vec2> m_radii = std::nullopt;

				///
				/// Used for Circle and Ellipse.
				///
				std::optional<float> m_fragments = std::nullopt;

				///
				/// Used for Point.
				///
				std::optional<int> m_pointsize = std::nullopt;

				///
				/// Used by all primitives.
				///
				graphics::Colour m_colour = {255, 255, 255, 255};

				///
				/// Used by line. Contains start and end point.
				///
				std::optional<glm::vec4> m_start_end = std::nullopt;

				///
				/// Used by Polyline and Polygon.
				///
				std::optional<std::vector<glm::vec2>> m_points = std::nullopt;
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
			///
			template<graphics::Primitives type>
			void create(const PrimitiveData& data);

			///
			/// Bind as active VA.
			///
			void bind() noexcept;

			///
			/// Unbind as active VA.
			///
			void unbind() noexcept;

			///
			/// \brief Set opacity.
			///
			/// This modifies internal colour objects opacity.
			///
			/// \param opacity Opacity 0 - 255.
			///
			void set_opacity(const std::uint8_t opacity) noexcept;

			///
			/// \brief Set colour.
			///
			/// Opacity is shared with set_opacity.
			///
			/// \param col Colour of primitive.
			///
			void set_colour(const graphics::Colour& col);

			///
			/// Get opacity.
			///
			/// \return Get opacity as normalized float.
			///
			[[nodiscard]] const float get_opacity() noexcept;

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
			[[nodiscard]] std::vector<graphics::PrimitiveVertex>& get_vertexs() noexcept;

			///
			/// Get width of primitive.
			///
			/// \return Const float. 0 if object is not 2D.
			///
			[[nodiscard]] const float get_width() const noexcept;

			///
			/// Get height of primitive.
			///
			/// \return Const float. 0 if object is not 2D.
			///
			[[nodiscard]] const float get_height() const noexcept;

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
			/// Creation data.
			///
			PrimitiveData m_data;

			///
			/// Type of primitive.
			///
			graphics::Primitives m_type;

			///
			/// Vertex Data
			///
			std::vector<graphics::PrimitiveVertex> m_vertexs;
		};

		template<graphics::Primitives type>
		inline void Primitive2D::create(const PrimitiveData& data)
		{
			m_type = type;
			m_data = data;
			m_vertexs.clear();
			std::vector<unsigned int> indices;

			if constexpr (type == graphics::Primitives::CIRCLE)
			{
				// Thanks to https://stackoverflow.com/a/33859443
				// For help with maths.

				unsigned int count        = 0;
				constexpr float incr_stat = 2.0f * glm::pi<float>();
				const float increment     = incr_stat / data.m_fragments.value();
				for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
				{
					m_vertexs.emplace_back((data.m_radius.value() * glm::cos(angle)) + data.m_radius.value(), (data.m_radius.value() * glm::sin(angle) + data.m_radius.value()), m_data.m_colour);
					indices.push_back(count);

					count++;
				}

				m_vb.create<graphics::PrimitiveVertex>(m_vertexs);
				m_ib.create(indices);

				m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
				m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

				m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);

				m_width  = data.m_radius.value() * 2.0f;
				m_height = data.m_radius.value() * 2.0f;
			}
			else if constexpr (type == graphics::Primitives::ELLIPSE)
			{
				// Thanks to https://stackoverflow.com/a/34735255
				// For help with maths.

				const float theta  = 2.0f * glm::pi<float>() / m_data.m_fragments.value();
				const float cosine = std::cosf(theta);
				const float sine   = std::sinf(theta);

				float temp = 0.0f;
				float x    = 1.0f;
				float y    = 0.0f;

				unsigned int count = 0;
				for (auto i = 0; i < std::floor(m_data.m_fragments.value()); i++)
				{
					m_vertexs.emplace_back((x * m_data.m_radii.value().x) + m_data.m_radii.value().x, (y * m_data.m_radii.value().y) + m_data.m_radii.value().y, m_data.m_colour);
					indices.push_back(count);
					count++;

					temp = x;
					x    = cosine * x - sine * y;
					y    = sine * temp + cosine * y;
				}

				m_vb.create<graphics::PrimitiveVertex>(m_vertexs);
				m_ib.create(indices);

				m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
				m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

				m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);

				m_width  = m_data.m_radii.value().x * 2.0f;
				m_height = m_data.m_radii.value().y * 2.0f;
			}
			else if constexpr (type == graphics::Primitives::LINE)
			{
				m_vertexs.emplace_back(m_data.m_start_end.value().x, m_data.m_start_end.value().y, m_data.m_colour);
				m_vertexs.emplace_back(m_data.m_start_end.value().z, m_data.m_start_end.value().w, m_data.m_colour);

				m_vb.create<graphics::PrimitiveVertex>(m_vertexs);

				indices.push_back(0);
				indices.push_back(1);
				m_ib.create(indices);

				m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
				m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

				m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);

				m_width  = 0.0f;
				m_height = 0.0f;
			}
			else if constexpr (type == graphics::Primitives::POINT)
			{
				m_vertexs.emplace_back(0.0f, 0.0f, m_data.m_colour);

				m_vb.create<graphics::PrimitiveVertex>(m_vertexs);

				indices.push_back(0);
				m_ib.create(indices);

				m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
				m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

				m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);

				m_width  = 0.0f;
				m_height = 0.0f;
			}
			else if constexpr ((type == graphics::Primitives::POLYLINE) || (type == graphics::Primitives::POLYGON))
			{
				m_width  = 0.0f;
				m_height = 0.0f;

				unsigned int count = 0;
				for (const auto& point : *m_data.m_points)
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

					m_vertexs.emplace_back(point.x, point.y, m_data.m_colour);
					indices.push_back(count);

					count++;
				}

				m_vb.create<graphics::PrimitiveVertex>(m_vertexs);
				m_ib.create(indices);

				m_layout.add<graphics::PrimitiveVertex, meta::VAPosition>(2);
				m_layout.add<graphics::PrimitiveVertex, meta::VAColour>(4);

				m_va.create<graphics::PrimitiveVertex>(m_vb, m_ib, m_layout);
			}
		}
	} // namespace components
} // namespace galaxy

#endif
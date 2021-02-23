///
/// Circle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_CIRCLE_HPP_
#define QUASAR_CIRCLE_HPP_

#include <glm/vec2.hpp>
#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Circle definition for renderer.
		///
		class Circle final : public graphics::VertexData, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Circle() noexcept;

			///
			/// Constructor.
			///
			/// \param radius Radius of the circle.
			/// \param fragments Number of fragments (i.e. vertexs) defining circle shape. More means more circular, but more vertexs.
			/// \param colour Colour.
			///
			Circle(const float radius, const unsigned int fragments, const graphics::Colour& colour);

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Circle(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Circle(Circle&&) noexcept;

			///
			/// Move assignment operator.
			///
			Circle& operator=(Circle&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Circle() noexcept = default;

			///
			/// Create the Circle.
			///
			/// \param radius Radius of the circle.
			/// \param fragments Number of fragments (i.e. vertexs) defining circle shape. More means more circular, but more vertexs.
			/// \param colour Colour.
			///
			void create(const float radius, const unsigned int fragments, const graphics::Colour& colour);

			///
			/// Update vertexs.
			///
			void update();

			///
			/// Change circle radius.
			///
			/// \param radius Radius of the circle.
			///
			void set_radius(const float radius);

			///
			/// Change circle fragments.
			///
			/// \param fragments Amount of points in the circle.
			///
			void set_fragments(const float fragments);

			///
			/// Change colour.
			///
			/// \param col Colour of line.
			///
			void change_colour(const graphics::Colour& col);

			///
			/// Bind as active VA.
			///
			void bind() noexcept;

			///
			/// Unbind as active VA.
			///
			void unbind() noexcept;

			///
			/// Set opacity.
			///
			/// \param opacity Opacity 0 - 255.
			///
			void set_opacity(const std::uint8_t opacity) noexcept;

			///
			/// Get opacity.
			///
			/// \return Const std::uint8_t.
			///
			[[nodiscard]] const std::uint8_t get_opacity() const noexcept;

			///
			/// Get current colour.
			///
			/// \return Const reference to the current line colour.
			///
			[[nodiscard]] const graphics::Colour& get_colour() const noexcept;

			///
			/// Get radius of circle.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float radius() const noexcept;

			///
			/// Get fragments of circle.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float fragments() const noexcept;

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
			Circle& operator=(const Circle&) = delete;

			///
			/// Copy constructor.
			///
			Circle(const Circle&) = delete;

		private:
			///
			/// Radius of circle.
			///
			float m_radius;

			///
			/// Fragments (aka points in the circle).
			///
			float m_fragments;

			///
			/// Colour.
			///
			graphics::Colour m_colour;
		};
	} // namespace components
} // namespace galaxy

#endif
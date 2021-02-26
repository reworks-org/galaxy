///
/// Ellipse.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ELLIPSE_HPP_
#define GALAXY_COMPONENTS_ELLIPSE_HPP_

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
		/// Ellipse definition for renderer.
		///
		class Ellipse final : public graphics::VertexData, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Ellipse() noexcept;

			///
			/// Constructor.
			///
			/// \param radius Radius of the Ellipse. x is horizontal radius, y is vertical radius.
			/// \param fragments Number of fragments (i.e. vertexs) defining Ellipse shape. More means more circular, but more vertexs.
			/// \param colour Colour.
			///
			Ellipse(const glm::vec2& radius, const unsigned int fragments, const graphics::Colour& colour);

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Ellipse(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Ellipse(Ellipse&&) noexcept;

			///
			/// Move assignment operator.
			///
			Ellipse& operator=(Ellipse&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Ellipse() noexcept = default;

			///
			/// Create the ellipse.
			///
			/// \param radius Radius of the Ellipse. x is horizontal radius, y is vertical radius.
			/// \param fragments Number of fragments (i.e. vertexs) defining Ellipse shape. More means more circular, but more vertexs.
			/// \param colour Colour.
			///
			void create(const glm::vec2& radius, const unsigned int fragments, const graphics::Colour& colour);

			///
			/// Update vertexs.
			///
			void update();

			///
			/// Change ellipse radius.
			///
			/// \param radius Radius of the Ellipse. x is horizontal radius, y is vertical radius.
			///
			void set_radius(const glm::vec2& radius);

			///
			/// Change ellipse fragments.
			///
			/// \param fragments Amount of points in the Ellipse.
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
			/// Get radius of ellipse.
			///
			/// \return Const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& radius() const noexcept;

			///
			/// Get fragments of ellipse.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float fragments() const noexcept;

			///
			/// Get shape vertexs.
			///
			/// \return Const reference to std::vector of primitive vertexs.
			///
			[[nodiscard]] const std::vector<graphics::PrimitiveVertex>& get_vertexs() const noexcept;

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
			Ellipse& operator=(const Ellipse&) = delete;

			///
			/// Copy constructor.
			///
			Ellipse(const Ellipse&) = delete;

		private:
			///
			/// Radius of ellipse.
			///
			glm::vec2 m_radius;

			///
			/// Fragments (aka points in the ellipse).
			///
			float m_fragments;

			///
			/// Colour of ellipse.
			///
			graphics::Colour m_colour;

			///
			/// Ellipse vertexs.
			///
			std::vector<graphics::PrimitiveVertex> m_vertexs;
		};
	} // namespace components
} // namespace galaxy

#endif
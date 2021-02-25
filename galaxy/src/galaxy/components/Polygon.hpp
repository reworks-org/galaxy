///
/// Polygon.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_POLYGON_HPP_
#define GALAXY_COMPONENTS_POLYGON_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	///
	/// Shorthand for point storage to keep api clean.
	///
	using PointStorage = std::vector<std::pair<float, float>>;

	namespace components
	{
		///
		/// Polygon graphic definition.
		///
		class Polygon final : public graphics::VertexData, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Polygon() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Polygon(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Polygon(Polygon&&) noexcept;

			///
			/// Move assignment operator.
			///
			Polygon& operator=(Polygon&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Polygon() noexcept = default;

			///
			/// Adds points to the polygon.
			///
			/// \param x X position of point to add.
			/// \param y Y position of point to add.
			///
			void add_point(const float x, const float y);

			///
			/// Adds points to the polygon.
			///
			/// \param point (x,y) position of a point in the world.
			///
			void add_point(const std::pair<float, float>& point);

			///
			/// Create the polygon.
			///
			/// \param col Colour of polygon.
			///
			void create(const graphics::Colour& col);

			///
			/// Update the polygon.
			///
			void update();

			///
			/// Change colour.
			///
			/// \param col Colour of polygon.
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
			/// Get all points in polygon.
			///
			/// \return Const reference to std::vector of points.
			///
			[[nodiscard]] const PointStorage& get_points() const noexcept;

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
			Polygon& operator=(const Polygon&) = delete;

			///
			/// Copy constructor.
			///
			Polygon(const Polygon&) = delete;

		private:
			///
			/// Point storage.
			///
			PointStorage m_points;

			///
			/// Colour.
			///
			graphics::Colour m_colour;
		};
	} // namespace components
} // namespace galaxy

#endif
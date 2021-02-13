///
/// Point.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_POINT_HPP_
#define GALAXY_COMPONENTS_POINT_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Point graphic definition.
		///
		class Point final : public graphics::VertexData, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Point() noexcept;

			///
			/// Constructor.
			///
			/// \param size Size of the point.
			/// \param colour Colour.
			///
			Point(const unsigned int size, const graphics::Colour& colour) noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Point(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Point(Point&&) noexcept;

			///
			/// Move assignment operator.
			///
			Point& operator=(Point&&) noexcept;

			///
			/// Destructor.
			///
			~Point() noexcept = default;

			///
			/// Create the point.
			///
			/// \param size Size of the point.
			/// \param colour Colour.
			///
			void create(const unsigned int size, const graphics::Colour& colour);

			///
			/// Bind as active VA.
			///
			void bind() noexcept;

			///
			/// Unbind as active VA.
			///
			void unbind() noexcept;

			///
			/// Set point size.
			///
			/// \param size New size of the point.
			///
			void set_size(const int size) noexcept;

			///
			/// Get size of point.
			///
			/// \return Const integer.
			///
			[[nodiscard]] const int get_size() const noexcept;

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
			Point& operator=(const Point&) = delete;

			///
			/// Copy constructor.
			///
			Point(const Point&) = delete;

		private:
			///
			/// Size of point.
			///
			int m_size;

			///
			/// Colour.
			///
			graphics::Colour m_colour;
		};
	} // namespace components
} // namespace galaxy

#endif
///
/// Point.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_POINT_HPP_
#define GALAXY_COMPONENTS_POINT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/shapes/Point.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// 2D shape.
		///
		class Point final : public fs::Serializable
		{
		  public:
			///
			/// Constructor.
			///
			Point();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Point(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Point(Point&&);

			///
			/// Move assignment operator.
			///
			Point& operator=(Point&&);

			///
			/// Destructor.
			///
			virtual ~Point();

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

		  public:
			///
			/// Shape.
			///
			graphics::Point m_shape;
		};
	} // namespace components
} // namespace galaxy

#endif

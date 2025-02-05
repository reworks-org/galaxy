///
/// Polygon.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_POLYGON_HPP_
#define GALAXY_COMPONENTS_POLYGON_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/shapes/Polygon.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// 2D shape.
		///
		class Polygon final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Polygon();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Polygon(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Polygon(Polygon&&);

			///
			/// Move assignment operator.
			///
			Polygon& operator=(Polygon&&);

			///
			/// Destructor.
			///
			virtual ~Polygon();

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]]
			nlohmann::json serialize() override;

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

		public:
			///
			/// Shape.
			///
			graphics::Polygon m_shape;
		};
	} // namespace components
} // namespace galaxy

#endif

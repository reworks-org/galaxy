///
/// Polyline.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_POLYLINE_HPP_
#define GALAXY_COMPONENTS_POLYLINE_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/shapes/Polyline.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// 2D shape.
		///
		class Polyline final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Polyline();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Polyline(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Polyline(Polyline&&);

			///
			/// Move assignment operator.
			///
			Polyline& operator=(Polyline&&);

			///
			/// Destructor.
			///
			virtual ~Polyline();

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
			Polyline& operator=(const Polyline&) = delete;

			///
			/// Copy constructor.
			///
			Polyline(const Polyline&) = delete;

		public:
			///
			/// Shape.
			///
			graphics::Polyline m_shape;
		};
	} // namespace components
} // namespace galaxy

#endif

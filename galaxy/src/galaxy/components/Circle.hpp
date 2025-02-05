///
/// Circle.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_CIRCLE_HPP_
#define GALAXY_COMPONENTS_CIRCLE_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/shapes/Circle.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// 2D shape.
		///
		class Circle final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Circle();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Circle(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Circle(Circle&&);

			///
			/// Move assignment operator.
			///
			Circle& operator=(Circle&&);

			///
			/// Destructor.
			///
			virtual ~Circle();

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
			Circle& operator=(const Circle&) = delete;

			///
			/// Copy constructor.
			///
			Circle(const Circle&) = delete;

		public:
			///
			/// Shape.
			///
			graphics::Circle m_shape;
		};
	} // namespace components
} // namespace galaxy

#endif

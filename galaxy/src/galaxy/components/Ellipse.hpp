///
/// Ellipse.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ELLIPSE_HPP_
#define GALAXY_COMPONENTS_ELLIPSE_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/shapes/Ellipse.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// 2D shape.
		///
		class Ellipse final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Ellipse();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Ellipse(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Ellipse(Ellipse&&);

			///
			/// Move assignment operator.
			///
			Ellipse& operator=(Ellipse&&);

			///
			/// Destructor.
			///
			virtual ~Ellipse();

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
			Ellipse& operator=(const Ellipse&) = delete;

			///
			/// Copy constructor.
			///
			Ellipse(const Ellipse&) = delete;

		public:
			///
			/// Shape.
			///
			graphics::Ellipse m_shape;
		};
	} // namespace components
} // namespace galaxy

#endif

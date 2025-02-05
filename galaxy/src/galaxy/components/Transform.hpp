///
/// Transform.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TRANSFORM_HPP_
#define GALAXY_COMPONENTS_TRANSFORM_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Transform.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Defines the 2D transformation of an entity.
		///
		class Transform final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Transform();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Transform(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Transform(Transform&&);

			///
			/// Move assignment operator.
			///
			Transform& operator=(Transform&&);

			///
			/// Destructor.
			///
			virtual ~Transform();

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
			Transform& operator=(const Transform&) = delete;

			///
			/// Copy constructor.
			///
			Transform(const Transform&) = delete;

		public:
			///
			/// Transformation object.
			///
			graphics::Transform m_tf;
		};
	} // namespace components
} // namespace galaxy

#endif

///
/// OnEvent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ONEVENT_HPP_
#define GALAXY_COMPONENTS_ONEVENT_HPP_

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Component data is called when entity is collided.
		///
		class OnEvent final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			OnEvent() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			OnEvent(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			OnEvent(OnEvent&&) noexcept;

			///
			/// Move assignment operator.
			///
			OnEvent& operator=(OnEvent&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~OnEvent() noexcept = default;

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
			OnEvent& operator=(const OnEvent&) = delete;

			///
			/// Copy constructor.
			///
			OnEvent(const OnEvent&) = delete;

		public:
		};
	} // namespace components
} // namespace galaxy

#endif
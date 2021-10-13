///
/// OnCollision.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ONCOLLISION_HPP_
#define GALAXY_COMPONENTS_ONCOLLISION_HPP_

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Component data is called when entity is collided.
		///
		class OnCollision final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			OnCollision() noexcept;

			///
			/// Constructor.
			///
			/// \param script_id Lua script ID in the ScriptBook.
			///
			OnCollision(std::string_view script_id) noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			OnCollision(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			OnCollision(OnCollision&&) noexcept;

			///
			/// Move assignment operator.
			///
			OnCollision& operator=(OnCollision&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~OnCollision() noexcept = default;

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
			OnCollision& operator=(const OnCollision&) = delete;

			///
			/// Copy constructor.
			///
			OnCollision(const OnCollision&) = delete;

		public:
			///
			/// Script to be called when a collision occurs with this entity.
			///
			std::string m_script;
		};
	} // namespace components
} // namespace galaxy

#endif
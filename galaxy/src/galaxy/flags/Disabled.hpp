///
/// Disabled.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FLAGS_DISABLED_HPP_
#define GALAXY_FLAGS_DISABLED_HPP_

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace flags
	{
		///
		/// Used to stop an entity from being updated/rendered.
		///
		class Disabled final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Disabled();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Disabled(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Disabled(Disabled&&);

			///
			/// Move assignment operator.
			///
			Disabled& operator=(Disabled&&);

			///
			/// Destructor.
			///
			virtual ~Disabled();

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
			Disabled& operator=(const Disabled&) = delete;

			///
			/// Copy constructor.
			///
			Disabled(const Disabled&) = delete;
		};
	} // namespace flags
} // namespace galaxy

#endif

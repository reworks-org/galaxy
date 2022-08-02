///
/// Serializable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_SERIALIZABLE_HPP_
#define GALAXY_FS_SERIALIZABLE_HPP_

#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace fs
	{
		///
		/// \brief Allows a class to be serialized.
		///
		/// A class wanting to gain serializability must inherit from this class and implement the interface.
		///
		class Serializable
		{
		public:
			///
			/// Destructor.
			///
			virtual ~Serializable() noexcept = default;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] virtual nlohmann::json serialize() = 0;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			virtual void deserialize(const nlohmann::json& json) = 0;

		protected:
			///
			/// Constructor.
			///
			Serializable() noexcept = default;
		};
	} // namespace fs
} // namespace galaxy

#endif
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
			virtual ~Serializable() = default;

			//
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

			///
			/// Get key identifying this data section.
			///
			/// \return Needs to return a key to identify what data this is when (de)serializing.
			///
			[[nodiscard]] virtual std::string& get_key() noexcept = 0;

		protected:
			///
			/// Arugment constructor.
			///
			/// \param obj Should be a 'this' pointer.
			///
			Serializable(Serializable* obj);

			///
			/// Copy constructor.
			///
			Serializable(const Serializable&) = default;

			///
			/// Move constructor.
			///
			Serializable(Serializable&&) = default;

			///
			/// Copy assignment operator.
			///
			Serializable& operator=(const Serializable&) = default;

			///
			/// Move assignment operator.
			///
			Serializable& operator=(Serializable&&) = default;

		private:
			///
			/// Constructor.
			///
			Serializable() = delete;
		};
	} // namespace fs
} // namespace galaxy

#endif
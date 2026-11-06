///
/// Serializable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_SERIALIZABLE_HPP_
#define GALAXY_META_SERIALIZABLE_HPP_

#include <nlohmann/json_fwd.hpp>

namespace galaxy
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
		virtual ~Serializable();

		///
		/// Move assignment operator.
		///
		virtual Serializable& operator=(Serializable&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		virtual Serializable& operator=(const Serializable&) noexcept = default;

		///
		/// Serializes object.
		///
		/// \return JSON object containing data to be serialized.
		///
		[[nodiscard]]
		virtual nlohmann::json serialize() = 0;

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

		///
		/// Move constructor.
		///
		Serializable(Serializable&&) noexcept = default;

		///
		/// Copy constructor.
		///
		Serializable(const Serializable&) noexcept = default;
	};
} // namespace galaxy

#endif

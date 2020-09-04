///
/// Serializable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SERIALIZABLE_HPP_
#define GALAXY_SERIALIZABLE_HPP_

#include <optional>

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
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
		/// Default virtual destructor.
		///
		virtual ~Serializable() = default;

		///
		/// \brief Serialization interface.
		///
		/// You need to fill out a json object with data from the class to be serialized
		/// and return it from this object. You will need to call std::make_optional. Cannot throw exceptions.
		///
		/// \return Json object containing data to be serialized, as an std::optional, to test if there was any errors creating object.
		///
		virtual std::optional<nlohmann::json> serialize() noexcept = 0;

		///
		/// \brief Deserialization interface.
		///
		/// Pass in a json object containing the section of json that your class data resides in.
		/// Can throw exceptions.
		///
		/// \param json Json object containing class data.
		///
		virtual void deserialize(const nlohmann::json& json) = 0;

	protected:
		///
		/// Default constructor.
		///
		Serializable() = default;

	private:
		///
		/// Move constructor.
		///
		Serializable(Serializable&&) = delete;

		///
		/// Copy constructor.
		///
		Serializable(const Serializable&) = delete;

		///
		/// Copy assignment operator.
		///
		Serializable& operator=(const Serializable&) = delete;

		///
		/// Move assignment operator.
		///
		Serializable& operator=(Serializable&&) = delete;
	};
}

#endif
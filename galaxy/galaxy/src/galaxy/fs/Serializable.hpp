///
/// Serializable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SERIALIZABLE_HPP_
#define GALAXY_SERIALIZABLE_HPP_

#include <optional>

#include <nlohmann/json.hpp>

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
		friend class Serializer;

	public:
		///
		/// Default virtual destructor.
		///
		virtual ~Serializable();

		///
		/// Serializes object.
		///
		/// \param j Json object needing to be filled out.
		///
		virtual nlohmann::json serialize() = 0;

		///
		/// Deserializes from object.
		///
		/// \param j Json object to retrieve data from.
		///
		virtual void deserialize(const nlohmann::json& j) = 0;

	protected:
		///
		/// Constructor.
		///
		Serializable();

		///
		/// Move constructor.
		///
		Serializable(Serializable&&) = default;

		///
		/// Copy constructor.
		///
		Serializable(const Serializable&) = default;

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
		/// Serial ID.
		///
		long m_id;
	};

	inline void to_json(nlohmann::json& j, const Serializable& s)
	{
		auto* as_ptr = const_cast<galaxy::Serializable*>(&s);
		j            = as_ptr->serialize();
	}

	inline void from_json(const nlohmann::json& j, Serializable& s)
	{
		s.deserialize(j);
	}
} // namespace galaxy

#endif
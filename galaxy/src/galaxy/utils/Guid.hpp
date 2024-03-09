///
/// Guid.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UTILS_GUID_HPP_
#define GALAXY_UTILS_GUID_HPP_

#include <compare>
#include <string>

namespace galaxy
{
	namespace utils
	{
		///
		/// Contains a 128bit randomly generated GUID,
		/// along with helper functions.
		///
		class Guid final
		{
		  public:
			///
			/// Constructor.
			///
			Guid();

			///
			/// Move constructor.
			///
			Guid(Guid&&);

			///
			/// Move assignment operator.
			///
			Guid& operator=(Guid&&);

			///
			/// Copy constructor.
			///
			Guid(const Guid&);

			///
			/// Copy assignment operator.
			///
			Guid& operator=(const Guid&);

			///
			/// Destructor.
			///
			~Guid();

			///
			/// Get the GUID as a string.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& to_string() const;

			///
			/// Check to make sure Guid is not empty.
			///
			/// \return True if guid is invalid or empty, usually from a move.
			///
			[[nodiscard]] bool is_empty() const;

			///
			/// Equality comparison.
			///
			/// \param rhs Right-hand side comparison.
			///
			/// \return True if equal.
			///
			[[nodiscard]] bool operator==(const Guid& rhs);

			///
			/// Inequality comparison.
			///
			/// \param rhs Right-hand side comparison.
			///
			/// \return True if NOT equal.
			///
			[[nodiscard]] bool operator!=(const Guid& rhs);

			///
			/// Comparison operator.
			///
			[[nodiscard]] auto operator<=>(const Guid&) const = default;

		  private:
			///
			/// Guid.
			///
			std::string m_id;
		};
	} // namespace utils
} // namespace galaxy

namespace std
{
	///
	/// Hash specialization template overload for Guid.
	///
	template<>
	struct hash<galaxy::utils::Guid>
	{
		///
		/// Hash specialization function for Guid.
		///
		std::size_t operator()(const galaxy::utils::Guid& guid) const
		{
			return hash<std::string>()(guid.to_string());
		}
	};
} // namespace std

#endif

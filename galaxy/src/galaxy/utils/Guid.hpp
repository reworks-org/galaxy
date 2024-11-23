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
			Guid(Guid&&) noexcept;

			///
			/// Move assignment operator.
			///
			Guid& operator=(Guid&&) noexcept;

			///
			/// Copy constructor.
			///
			Guid(const Guid&) noexcept;

			///
			/// Copy assignment operator.
			///
			Guid& operator=(const Guid&) noexcept;

			///
			/// Destructor.
			///
			~Guid() noexcept;

			///
			/// Get the GUID as a string.
			///
			/// \return Const string reference.
			///
			[[nodiscard]]
			const std::string& to_string() const noexcept;

			///
			/// Check to make sure Guid is not empty.
			///
			/// \return True if guid is invalid or empty, usually from a move.
			///
			[[nodiscard]]
			bool is_empty() const noexcept;

			///
			/// Equality comparison.
			///
			/// \param rhs Right-hand side comparison.
			///
			/// \return True if equal.
			///
			[[nodiscard]]
			bool operator==(const Guid& rhs) noexcept;

			///
			/// Inequality comparison.
			///
			/// \param rhs Right-hand side comparison.
			///
			/// \return True if NOT equal.
			///
			[[nodiscard]]
			bool operator!=(const Guid& rhs) noexcept;

			///
			/// Comparison operator.
			///
			[[nodiscard]]
			auto operator<=>(const Guid&) const noexcept = default;

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

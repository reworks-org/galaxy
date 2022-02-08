///
/// Guid.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_GUID_HPP_
#define GALAXY_CORE_GUID_HPP_

#include <array>
#include <compare>
#include <string>

namespace galaxy
{
	namespace core
	{
		///
		/// Contains a 128bit randomly generated GUID,
		/// along with helper functions.
		///
		class Guid final
		{
		public:
			///
			/// Create a new GUID.
			///
			/// \return RVO object.
			///
			[[nodiscard]] static Guid make() noexcept;

			///
			/// Constructor.
			///
			Guid() noexcept;

			///
			/// Move constructor.
			///
			Guid(Guid&&) noexcept;

			///
			/// Move assignment operator.
			///
			Guid& operator=(Guid&&) noexcept;

			///
			/// Destructor.
			///
			~Guid() noexcept = default;

			///
			/// Get the GUID as a string.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& to_string() const noexcept;

			///
			/// Check to make sure GUID is not empty.
			///
			/// \return True if GUID has not yet been created.
			///
			[[nodiscard]] bool is_empty() const noexcept;

			///
			/// Equality comparison.
			///
			/// \param rhs Right-hand side comparison.
			///
			/// \return True if equal.
			///
			[[nodiscard]] bool operator==(const Guid& rhs) noexcept;

			///
			/// Inequality comparison.
			///
			/// \param rhs Right-hand side comparison.
			///
			/// \return True if NOT equal.
			///
			[[nodiscard]] bool operator!=(const Guid& rhs) noexcept;

			///
			/// Spaceship operator.
			///
			[[nodiscard]] auto operator<=>(const Guid&) const = default;

		private:
			///
			/// Copy constructor.
			///
			Guid(const Guid&) = delete;

			///
			/// Copy assignment operator.
			///
			Guid& operator=(const Guid&) = delete;

		private:
			///
			/// First part.
			///
			std::uint32_t m_first;

			///
			/// Second part.
			///
			std::uint16_t m_second;

			///
			/// Third part.
			///
			std::uint8_t m_third;

			///
			/// Forth part.
			///
			std::uint8_t m_fourth;

			///
			/// Fifth part.
			///
			std::uint64_t m_fifth;

			///
			/// String representation of guid.
			///
			std::string m_string;
		};
	} // namespace core
} // namespace galaxy

namespace std
{
	///
	/// Hash specialization template overload for Guid.
	///
	template<>
	struct hash<galaxy::core::Guid>
	{
		///
		/// Hash specialization function for Guid.
		///
		std::size_t operator()(const galaxy::core::Guid& guid) const
		{
			return hash<std::string>()(guid.to_string());
		}
	};
} // namespace std

#endif
///
/// UUID.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UTILS_UUID_HPP_
#define GALAXY_UTILS_UUID_HPP_

#include <array>
#include <format>

namespace galaxy
{
	///
	/// Contains a 128bit randomly generated UUID,
	/// along with helper functions.
	///
	class UUID final
	{
	public:
		///
		/// Constructor.
		///
		UUID();

		///
		/// Move constructor.
		///
		UUID(UUID&&) noexcept;

		///
		/// Move assignment operator.
		///
		[[maybe_unused]]
		UUID& operator=(UUID&&) noexcept;

		///
		/// Copy constructor.
		///
		UUID(const UUID&) noexcept;

		///
		/// Copy assignment operator.
		///
		[[maybe_unused]]
		UUID& operator=(const UUID&) noexcept;

		///
		/// Destructor.
		///
		~UUID() noexcept;

		///
		/// Get the UUID as a hash.
		///
		/// \return Hash integer.
		///
		[[nodiscard]]
		std::size_t hash() noexcept;

		///
		/// Get the UUID as a string.
		///
		/// \return Const string reference.
		///
		[[nodiscard]]
		const std::string& str() const noexcept;

		///
		/// Equality comparison.
		///
		/// \param rhs Right-hand side comparison.
		///
		/// \return True if equal.
		///
		[[nodiscard]]
		bool operator==(const UUID& rhs) noexcept;

		///
		/// Inequality comparison.
		///
		/// \param rhs Right-hand side comparison.
		///
		/// \return True if NOT equal.
		///
		[[nodiscard]]
		bool operator!=(const UUID& rhs) noexcept;

	private:
		///
		/// UUID.
		///
		std::array<unsigned char, 16> m_uuid;

		///
		/// String representation.
		///
		std::string m_str;
	};
} // namespace galaxy

namespace std
{
	///
	/// Hash specialization template overload for UUID.
	///
	template<>
	struct hash<galaxy::UUID>
	{
		///
		/// Hash specialization function for UUID.
		///
		std::size_t operator()(galaxy::UUID& uuid) const noexcept
		{
			return uuid.hash();
		}
	};
} // namespace std

template<>
struct std::formatter<galaxy::UUID> : std::formatter<std::string>
{
	auto format(const galaxy::UUID& uuid, format_context& ctx) const noexcept
	{
		return std::formatter<std::string>::format(uuid.str(), ctx);
	}
};

#endif

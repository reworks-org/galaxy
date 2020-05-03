///
/// ProtectedArithmetic.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_PROTECTEDARITHMETIC_HPP_
#define PROTOSTAR_PROTECTEDARITHMETIC_HPP_

#include <mutex>
#include <type_traits>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// Protected arithmetic type that has its read/write protected by a lock_guard.
	///
	template<typename Arithmetic>
	class ProtectedAirthmetic final
	{
		///
		/// Makes sure type is actually arithmetic.
		///
		static_assert(std::is_arithmetic<Arithmetic>::value);

	public:
		///
		/// Default constructor.
		///
		ProtectedAirthmetic() noexcept;

		///
		/// Argument constructor.
		///
		/// \param value New value for var.
		///
		explicit ProtectedAirthmetic(const Arithmetic value) noexcept;

		///
		/// Default destructor.
		///
		~ProtectedAirthmetic() noexcept = default;

		///
		/// Set/update the value.
		///
		/// \param value New value for var.
		///
		void set(const Arithmetic value) noexcept;

		///
		/// Get value stored.
		///
		/// \return Mutex protected value.
		///
		const Arithmetic get() noexcept;

	private:
		///
		/// Deleted move constructor.
		///
		ProtectedAirthmetic(ProtectedAirthmetic&&) = delete;

		///
		/// Deleted copy constructor.
		///
		ProtectedAirthmetic(const ProtectedAirthmetic&) = delete;

		///
		/// Deleted move assignment operator.
		///
		ProtectedAirthmetic& operator= (ProtectedAirthmetic&&) = delete;

		///
		/// Deleted copy assignment operator.
		///
		ProtectedAirthmetic& operator= (const ProtectedAirthmetic&) = delete;

	private:
		///
		/// Mutex protecting the variable.
		///
		std::mutex m_mutex;

		///
		/// The variable.
		///
		Arithmetic m_var;
	};

	///
	/// Predefined type for float.
	///
	using ProtectedFloat = ProtectedAirthmetic<float>;

	///
	/// Predefined type for double.
	///
	using ProtectedDouble = ProtectedAirthmetic<double>;

	///
	/// Predefined type for int.
	///
	using ProtectedInt = ProtectedAirthmetic<int>;

	///
	/// Predefined type for unsigned int.
	///
	using ProtectedUInt = ProtectedAirthmetic<unsigned int>;

	///
	/// Predefined type for bool.
	///
	using ProtectedBool = ProtectedAirthmetic<bool>;

	template<typename Arithmetic>
	inline ProtectedAirthmetic<Arithmetic>::ProtectedAirthmetic() noexcept
		:m_var(0)
	{
	}

	template<typename Arithmetic>
	inline ProtectedAirthmetic<Arithmetic>::ProtectedAirthmetic(const Arithmetic value) noexcept
	{
		set(value);
	}

	template<typename Arithmetic>
	inline void ProtectedAirthmetic<Arithmetic>::set(const Arithmetic value) noexcept
	{
		std::lock_guard<std::mutex> l_lock(m_mutex);
		m_var = value;
	}

	template<typename Arithmetic>
	inline const Arithmetic ProtectedAirthmetic<Arithmetic>::get() noexcept
	{
		std::lock_guard<std::mutex> l_lock(m_mutex);
		return m_var;
	}
}

#endif
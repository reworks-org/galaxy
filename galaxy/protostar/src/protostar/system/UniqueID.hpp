///
/// UniqueID.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_UNIQUEID_HPP_
#define PROTOSTAR_UNIQUEID_HPP_

#include <cstddef>
#include <type_traits>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// UniqueID can only be a class.
	///
	template<typename Type>
	concept IsClass = std::is_class<Type>::value;

	///
	/// Generates a unique id for a type for each type of specialization.
	/// And the id is kept as a compile time constant.
	///
	template<IsClass Specialization>
	class UniqueID final
	{
	public:
		///
		/// Default destructor.
		///
		~UniqueID() noexcept = default;

		///
		/// Use this function to retrieve the ID.
		/// Will generate a new id if it is called for the first time.
		///
		/// \return Unique ID for the specialization of that type.
		///
		template<typename Type>
		static const std::size_t get() noexcept;

	private:
		///
		/// Private default constructor.
		///
		UniqueID() noexcept = default;

		///
		/// Internal counter to keep track of allocated ids.
		///
		static inline std::size_t s_counter = 0;
	};

	template<IsClass Specialization>
	template<typename Type>
	inline const std::size_t UniqueID<Specialization>::get() noexcept
	{
		static std::size_t id = s_counter++;
		return id;
	}
}

#endif
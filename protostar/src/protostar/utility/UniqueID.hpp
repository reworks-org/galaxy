///
/// UniqueID.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_UNIQUEID_HPP_
#define PROTOSTAR_UNIQUEID_HPP_

#include <cstddef>

///
/// Core namespace.
///
namespace protostar
{
	///
	/// Generates a unique id for a type for each type of specialization.
	/// And the id is kept as a compile time constant.
	///
	template<typename Specialization>
	class UniqueID final
	{
		///
		/// Specialization should be a struct or class.
		///
		static_assert(std::is_class<Specialization>::value);

	public:
		///
		/// Default destructor.
		///
		~UniqueID() = default;

		///
		/// Use this function to retrieve the ID.
		/// Will generate a new id if it is called for the first time.
		///
		/// \return Std::size_t ID.
		///
		template<typename Type>
		static const std::size_t get();

	private:
		///
		/// Private default constructor.
		///
		UniqueID() = default;

	private:
		///
		/// Internal counter to keep track of allocated ids.
		///
		static inline std::size_t s_counter = 0;
	};

	template<typename Specialization>
	template<typename Type>
	inline const std::size_t UniqueID<Specialization>::get()
	{
		static std::size_t id = s_counter++;
		return id;
	}
}

#endif
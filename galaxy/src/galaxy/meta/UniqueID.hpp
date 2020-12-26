///
/// UniqueID.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_UNIQUEID_HPP_
#define GALAXY_META_UNIQUEID_HPP_

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace meta
	{
		///
		/// Generates a unique id for a type for each type of specialization.
		/// And the id is kept as a compile time constant.
		///
		template<is_class Specialization>
		class UniqueID final
		{
		public:
			///
			/// Copy constructor.
			///
			UniqueID(const UniqueID&) = delete;

			///
			/// Move constructor.
			///
			UniqueID(UniqueID&&) = delete;

			///
			/// Copy assignment operator.
			///
			UniqueID& operator=(const UniqueID&) = delete;

			///
			/// Move assignment operator.
			///
			UniqueID& operator=(UniqueID&&) = delete;

			///
			/// Destructor.
			///
			~UniqueID() = default;

			///
			/// Use this function to retrieve the ID.
			/// Will generate a new id if it is called for the first time.
			///
			/// \return Unique ID for the specialization of that type.
			///
			template<typename Type>
			[[nodiscard]] static const std::size_t get();

		private:
			///
			/// Constructor.
			///
			UniqueID() = default;

			///
			/// Internal counter to keep track of allocated ids.
			///
			static inline std::size_t s_counter = 0;
		};

		template<is_class Specialization>
		template<typename Type>
		[[nodiscard]] inline const std::size_t UniqueID<Specialization>::get()
		{
			static std::size_t id = s_counter++;
			return id;
		}
	} // namespace meta
} // namespace galaxy

#endif
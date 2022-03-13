///
/// UniqueId.hpp
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
		class UniqueId final
		{
		public:
			///
			/// Destructor.
			///
			~UniqueId() noexcept = default;

			///
			/// Use this function to retrieve the ID.
			/// Will generate a new id if it is called for the first time.
			///
			/// \return Unique ID for the specialization of that type.
			///
			template<typename Type>
			[[nodiscard]] static std::size_t get() noexcept;

		private:
			///
			/// Constructor.
			///
			UniqueId() noexcept = default;

			///
			/// Copy constructor.
			///
			UniqueId(const UniqueId&) = delete;

			///
			/// Move constructor.
			///
			UniqueId(UniqueId&&) = delete;

			///
			/// Copy assignment operator.
			///
			UniqueId& operator=(const UniqueId&) = delete;

			///
			/// Move assignment operator.
			///
			UniqueId& operator=(UniqueId&&) = delete;

		private:
			///
			/// Internal counter to keep track of allocated ids.
			///
			inline static std::size_t s_counter = 0;
		};

		template<is_class Specialization>
		template<typename Type>
		[[nodiscard]] inline std::size_t UniqueId<Specialization>::get() noexcept
		{
			static std::size_t id = s_counter++;
			return id;
		}
	} // namespace meta
} // namespace galaxy

#endif
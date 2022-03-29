///
/// StaticId.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_STATICID_HPP_
#define GALAXY_META_STATICID_HPP_

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace meta
	{
		///
		/// Generates a static id. Each id is unique to the specialization.
		/// And the id is kept as a compile time constant.
		///
		template<is_class Specialization>
		class StaticId final
		{
		public:
			///
			/// Destructor.
			///
			~StaticId() noexcept = default;

			///
			/// Use this function to retrieve the ID.
			/// Will generate a new id if it is called for the first time.
			///
			/// \return Unique ID for the specialization of that type.
			///
			[[nodiscard]] static std::size_t get() noexcept;

		private:
			///
			/// Constructor.
			///
			StaticId() noexcept = default;

			///
			/// Copy constructor.
			///
			StaticId(const StaticId&) = delete;

			///
			/// Move constructor.
			///
			StaticId(StaticId&&) = delete;

			///
			/// Copy assignment operator.
			///
			StaticId& operator=(const StaticId&) = delete;

			///
			/// Move assignment operator.
			///
			StaticId& operator=(StaticId&&) = delete;

		private:
			///
			/// Internal counter to keep track of allocated ids.
			///
			inline static std::size_t s_counter = 0;
		};

		template<is_class Specialization>
		[[nodiscard]] inline std::size_t StaticId<Specialization>::get() noexcept
		{
			static std::size_t id = s_counter++;
			return id;
		}
	} // namespace meta
} // namespace galaxy

#endif
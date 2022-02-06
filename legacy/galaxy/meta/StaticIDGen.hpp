///
/// StaticIDGen.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_STATICIDGEN_HPP_
#define GALAXY_META_STATICIDGEN_HPP_

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
		class StaticIDGen final
		{
		public:
			///
			/// Destructor.
			///
			~StaticIDGen() noexcept = default;

			///
			/// Use this function to retrieve the ID.
			/// Will generate a new id if it is called for the first time.
			///
			/// \return Unique ID for the specialization of that type.
			///
			[[nodiscard]] static const std::size_t get() noexcept;

		private:
			///
			/// Constructor.
			///
			StaticIDGen() noexcept = default;

			///
			/// Copy constructor.
			///
			StaticIDGen(const StaticIDGen&) = delete;

			///
			/// Move constructor.
			///
			StaticIDGen(StaticIDGen&&) = delete;

			///
			/// Copy assignment operator.
			///
			StaticIDGen& operator=(const StaticIDGen&) = delete;

			///
			/// Move assignment operator.
			///
			StaticIDGen& operator=(StaticIDGen&&) = delete;

		private:
			///
			/// Internal counter to keep track of allocated ids.
			///
			inline static std::size_t s_counter = 0;
		};

		template<is_class Specialization>
		[[nodiscard]] inline const std::size_t StaticIDGen<Specialization>::get() noexcept
		{
			static std::size_t id = s_counter++;
			return id;
		}
	} // namespace meta
} // namespace galaxy

#endif
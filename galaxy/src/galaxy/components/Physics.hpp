///
/// Physics.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_PHYSICS_HPP_
#define GALAXY_COMPONENTS_PHYSICS_HPP_

#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace components
	{
		///
		/// Physics component.
		///
		class Physics final
		{
		public:
			///
			/// Constructor.
			///
			Physics() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Physics(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Physics(Physics&&) noexcept;

			///
			/// Move assignment operator.
			///
			Physics& operator=(Physics&&) noexcept;

			///
			/// Destructor.
			///
			~Physics();

		private:
			///
			/// Copy constructor.
			///
			Physics(const Physics&) = delete;

			///
			/// Copy assignment operator.
			///
			Physics& operator=(const Physics&) = delete;
		};
	} // namespace components
} // namespace galaxy

#endif
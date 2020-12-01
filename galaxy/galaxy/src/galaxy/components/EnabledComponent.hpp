///
/// EnabledComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ENABLEDCOMPONENT_HPP_
#define GALAXY_ENABLEDCOMPONENT_HPP_

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Flag component used to determine if an entity is "alive" or "dead".
	///
	struct EnabledComponent final
	{
		///
		/// Default constructor.
		///
		EnabledComponent() = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		EnabledComponent(const nlohmann::json& json);
	};
} // namespace galaxy

#endif
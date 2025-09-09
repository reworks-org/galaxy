///
/// EventHandler.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_EVENTHANDLER_HPP_
#define GALAXY_EVENTS_EVENTHANDLER_HPP_

#include <entt/signal/fwd.hpp>

///
/// Forward declaration.
///
union SDL_Event;

namespace galaxy
{
	///
	/// Forward declaration.
	///
	class Window;

	namespace events
	{
		///
		/// Processess an SDL_Event structure and dispatches the relevant event triggers.
		///
		/// \param events Event structure to process.
		/// \param window Window where these events occured.
		/// \param dispatcher Dispatcher to send event triggers to.
		///
		void handler(SDL_Event& events, Window& window, entt::dispatcher& dispatcher);
	} // namespace events
} // namespace galaxy

#endif

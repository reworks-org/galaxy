///
/// EventHandler.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/signal/dispatcher.hpp>
#include <SDL3/SDL_events.h>

#include "galaxy/core/Window.hpp"
#include "galaxy/events/GainedFocus.hpp"
#include "galaxy/events/KeyPress.hpp"
#include "galaxy/events/LostFocus.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowClosed.hpp"
#include "galaxy/events/WindowResized.hpp"
#include "galaxy/logging/Log.hpp"

#include "EventHandler.hpp"

namespace galaxy
{
	namespace events
	{
		void handler(SDL_Event& events, Window& window, entt::dispatcher& dispatcher)
		{
			while (SDL_PollEvent(&events))
			{
				switch (events.type)
				{
					case SDL_EVENT_QUIT:
					case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
					case SDL_EVENT_WINDOW_DESTROYED:
						dispatcher.trigger(WindowClosed {});
						window.close();
						break;

					case SDL_EVENT_LOCALE_CHANGED:
						// TODO: Handle Languages.
						break;

					case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
					case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
						// SDL_GetDisplayContentScale
						// SDL_GetWindowDisplayScale
						// https://wiki.libsdl.org/SDL3/README-highdpi
						break;

					case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
						// SDL_GetWindowSizeInPixels
						break;

					case SDL_EVENT_WINDOW_RESIZED:
					case SDL_EVENT_WINDOW_MAXIMIZED:
					case SDL_EVENT_WINDOW_RESTORED:
					case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
					case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
						{
							const auto size = window.get_pixel_size();

							// clang-format off
							auto wr = WindowResized
							{
								.m_width = size.x,
								.m_height = size.y
							};
							// clang-format on

							dispatcher.trigger(wr);
						}
						break;

					case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
						// TODO
						// HANDLE RESIZE + DISPLAY SCALE + PIXEL SIZE SCALE.
						break;

					case SDL_EVENT_WINDOW_MOUSE_ENTER:
					case SDL_EVENT_WINDOW_FOCUS_GAINED:
						dispatcher.trigger(GainedFocus {});
						break;

					case SDL_EVENT_WINDOW_MOUSE_LEAVE:
					case SDL_EVENT_WINDOW_FOCUS_LOST:
						dispatcher.trigger(LostFocus {});
						break;

					case SDL_EVENT_KEY_DOWN:
						{
							// clang-format off
							auto kd = KeyPress
							{
								.m_scancode = events.key.scancode,
								.m_key = events.key.key,
								.m_mod = events.key.mod,
								.m_raw = events.key.raw,
								.m_repeat = events.key.repeat
							};
							// clang-format on

							dispatcher.trigger(kd);
						}
						break;

					case SDL_EVENT_KEY_UP:
						{
							// clang-format off
							auto ku = KeyPress
							{
								.m_scancode = events.key.scancode,
								.m_key = events.key.key,
								.m_mod = events.key.mod,
								.m_raw = events.key.raw,
								.m_repeat = events.key.repeat
							};
							// clang-format on

							dispatcher.trigger(ku);
						}
						break;

					case SDL_EVENT_TEXT_EDITING:
					case SDL_EVENT_TEXT_INPUT:
					case SDL_EVENT_TEXT_EDITING_CANDIDATES:
						// TODO
						break;

					case SDL_EVENT_MOUSE_MOTION:
						{
							// clang-format off
							auto mm = MouseMoved
							{
								.m_xpos = events.motion.x,
								.m_ypos = events.motion.y,
								.m_xrel = events.motion.xrel,
								.m_yrel = events.motion.yrel
							};
							// clang-format on

							dispatcher.trigger(mm);
						}
						break;

					case SDL_EVENT_MOUSE_BUTTON_DOWN:
						{
							// clang-format off
							auto mp = MousePressed
							{
								.m_xpos = events.button.x,
								.m_ypos = events.button.y,
								.m_clicks = events.button.clicks,
								.m_button = input::int_to_mouse_btn(events.button.button)
							};
							// clang-format on

							dispatcher.trigger(mp);
						}
						break;

					case SDL_EVENT_MOUSE_BUTTON_UP:
						{
							// clang-format off
							auto mr = MouseReleased
							{
								.m_xpos = events.button.x,
								.m_ypos = events.button.y,
								.m_clicks = events.button.clicks,
								.m_button = input::int_to_mouse_btn(events.button.button)
							};
							// clang-format on

							dispatcher.trigger(mr);
						}
						break;

					case SDL_EVENT_MOUSE_WHEEL:
						{
							// clang-format off
							auto mw = MouseWheel
							{
								.m_amount_x = events.wheel.x,
								.m_amount_y = events.wheel.y,
								.m_direction = input::int_to_mousewheel_dir(events.wheel.direction),
								.m_mouse_x = events.wheel.mouse_x,
								.m_mouse_y = events.wheel.mouse_y,
								.m_total_x = events.wheel.integer_x,
								.m_total_y = events.wheel.integer_y
							};
							// clang-format on

							dispatcher.trigger(mw);
						}
						break;

					case SDL_EVENT_DROP_BEGIN:
						/*Drag and drop events
						SDL_EVENT_DROP_FILE = 0x1000, /**< The system requests a file open *
						SDL_EVENT_DROP_TEXT,      /**< text/plain drag-and-drop event *
						SDL_EVENT_DROP_BEGIN,     /**< A new set of drops is beginning (NULL filename) *
						SDL_EVENT_DROP_COMPLETE,  /**< Current set of drops is now complete (NULL filename) *
						SDL_EVENT_DROP_POSITION,  /**< Position while moving over the window */
						// SDL_DropEvent
						break;

					case SDL_EVENT_JOYSTICK_AXIS_MOTION:
					case SDL_EVENT_JOYSTICK_BALL_MOTION:
					case SDL_EVENT_JOYSTICK_HAT_MOTION:
					case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
					case SDL_EVENT_JOYSTICK_BUTTON_UP:
					case SDL_EVENT_JOYSTICK_REMOVED:
					case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
					case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
						// TODO
						GALAXY_LOG(GALAXY_WARN, "Unsupported joystick event detected.");
						break;

					case SDL_EVENT_GAMEPAD_AXIS_MOTION:
					case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
					case SDL_EVENT_GAMEPAD_BUTTON_UP:
					case SDL_EVENT_GAMEPAD_REMOVED:
					case SDL_EVENT_GAMEPAD_REMAPPED:
					case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
					case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
					case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
					case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
					case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
					case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
						// TODO
						GALAXY_LOG(GALAXY_WARN, "Unsupported gamepad event detected.");
						break;

					case SDL_EVENT_FINGER_DOWN:
					case SDL_EVENT_FINGER_UP:
					case SDL_EVENT_FINGER_MOTION:
					case SDL_EVENT_FINGER_CANCELED:
						// TODO
						GALAXY_LOG(GALAXY_WARN, "Unsupported touch event detected.");
						break;

					case SDL_EVENT_PEN_PROXIMITY_IN:
					case SDL_EVENT_PEN_PROXIMITY_OUT:
					case SDL_EVENT_PEN_DOWN:
					case SDL_EVENT_PEN_UP:
					case SDL_EVENT_PEN_BUTTON_DOWN:
					case SDL_EVENT_PEN_BUTTON_UP:
					case SDL_EVENT_PEN_MOTION:
					case SDL_EVENT_PEN_AXIS:
						// TODO
						GALAXY_LOG(GALAXY_WARN, "Unsupported pen event detected.");
						break;

					default:
						GALAXY_LOG(GALAXY_WARN, "Unknown event detected: '{0}'", magic_enum::enum_name(static_cast<SDL_EventType>(events.type)));
						break;
				}
			}
		}
	} // namespace events
} // namespace galaxy

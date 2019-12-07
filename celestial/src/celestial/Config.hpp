///
/// Config.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CONFIG_CELESTIAL_HPP_
#define CONFIG_CELESTIAL_HPP_

#include <memory>

///
/// Defines the template type used in Fonts i.e. Your class that inherits from Font.
///
#define FONT_CLASS decltype(nullptr)

///
/// Defines the template type used in Texture i.e. Your class that inherits from Texture.
///
#define TEXTURE_CLASS decltype(nullptr)

///
/// You need to define a keycode to use for confirming UI stuff like the textbook.
///
#define CONFIRMATION_KEYCODE 0

///
/// This function is used with delta time in order to see time passed.
/// I.e. When u add delta time to a time counter then i.e. IF time_Counter > TIME_UNTT(500) <- 500 milliseconds past for example..
///
#define TIME_COMPARE_FUNC(x) x

#endif
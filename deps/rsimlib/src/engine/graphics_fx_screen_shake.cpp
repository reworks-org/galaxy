/** @file
 * @brief Implements the ScreenShake camera-offset effect.
 */
#include "graphics_fx.h"

#include "renderer.h"

#include <SDL2/SDL.h>

#include <algorithm>
#include <cmath>

namespace sl
{
	ScreenShake::~ScreenShake()
	{
		clear();
	}

	void ScreenShake::trigger(float amplitude, float duration)
	{
		amplitude_ = std::max(0.0f, amplitude);
		duration_ = std::max(0.0f, duration);
		remaining_ = duration_;
	}

	void ScreenShake::update(float delta_seconds)
	{
		if (remaining_ <= 0.0f)
		{
			clear();
			return;
		}
		remaining_ = std::max(0.0f, remaining_ - std::max(0.0f, delta_seconds));
		const float falloff = duration_ > 0.0f ? remaining_ / duration_ : 0.0f;
		const float time = static_cast<float>(SDL_GetTicks()) * 0.01f;
		offset_x_ = std::sin(time * 1.73f) * amplitude_ * falloff;
		offset_y_ = std::cos(time * 2.11f) * amplitude_ * falloff;
		detail::set_screen_offset(offset_x_, offset_y_);
	}

	void ScreenShake::clear()
	{
		remaining_ = 0.0f;
		offset_x_ = 0.0f;
		offset_y_ = 0.0f;
		detail::set_screen_offset(0.0f, 0.0f);
	}

	bool ScreenShake::active() const { return remaining_ > 0.0f; }

} // namespace sl

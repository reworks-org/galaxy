///
/// Generic.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <raylib.hpp>

#include "Generic.hpp"

namespace galaxy
{
	namespace math
	{
		float normalize(const float value, const float start, const float end) noexcept
		{
			return ::Normalize(value, start, end);
		}

		float remap(const float value, const float input_start, const float input_end, const float output_start, const float output_end) noexcept
		{
			return ::Remap(value, input_start, input_end, output_start, output_end);
		}

		float wrap(const float value, const float min, const float max) noexcept
		{
			return ::Wrap(value, min, max);
		}
	} // namespace math
} // namespace galaxy

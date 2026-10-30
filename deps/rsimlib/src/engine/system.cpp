/** @file
 * @brief Implements frame pacing, timing, and core shutdown helpers.
 */

#include "system.h"

#include "async_internal.h"
#include "parallel_record.h"
#include "parallel_render.h"
#include "display.h"
#include "font.h"
#include "renderer.h"

namespace sl
{
	void shutdown()
	{
		display_shutdown();
	}

	void end_frame()
	{
		detail::recycle_transient_instance_buffers();
		detail::pump_async_loads();
	}
} // namespace sl

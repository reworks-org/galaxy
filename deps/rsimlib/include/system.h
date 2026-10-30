#pragma once

#include <cstdint>

namespace sl
{
    /** Shut down the system and release any allocated resources. */
    void shutdown();
    /** Call once per loop iteration; sleeps as needed to hit the target frame rate and records the frame time. */
    void end_frame();
} // namespace sl
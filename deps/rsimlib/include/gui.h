#pragma once

#include "event.h"

namespace sl
{

    /** Create the ImGui context and attach it to the current SDL/GL window. */
    void gui_init();
    /** Forward an SDL event to ImGui's input handling. */
    void gui_handle_event(const Event &event);
    /** Start a new ImGui frame; call before issuing any ImGui widget calls. */
    void gui_new_frame();
    /** Render the ImGui draw data built up since new_frame(). */
    void gui_render();
    /** Destroy the ImGui context; call before the GL context is destroyed. */
    void gui_shutdown();
    /** Show the ImGui demo window. */
    void gui_show_demo_window();
    void gui_show_metrics_window(bool &show_metrics);
} // namespace sl

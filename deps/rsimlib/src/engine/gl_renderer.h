#pragma once

#include "renderer.h"

#include <memory>

namespace sl::detail
{
    std::unique_ptr<Renderer> create_gl_renderer();
}

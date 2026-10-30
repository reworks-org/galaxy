#pragma once

#ifdef _WIN32

#include "renderer.h"

namespace sl::detail
{
    std::unique_ptr<Renderer> create_d3d12_renderer();
}

#endif // _WIN32

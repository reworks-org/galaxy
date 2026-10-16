/** @file
 * @brief Selects and creates the active Renderer backend implementation.
 */
#include "renderer.h"
#include "gl_renderer.h"


namespace sl::detail
{
    std::unique_ptr<Renderer> create_renderer()
    {
		return create_gl_renderer();
    }
}

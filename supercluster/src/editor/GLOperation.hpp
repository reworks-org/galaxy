///
/// GLOperation.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_LAYERS_GLOperation_HPP_
#define SUPERCLUSTER_LAYERS_GLOperation_HPP_

#include <functional>
#include <vector>

namespace sc
{
	using OpenGLOperation      = std::function<void(void)>;
	using OpenGLOperationStack = std::vector<OpenGLOperation>;
} // namespace sc

#endif
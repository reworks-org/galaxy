///
/// GLOperation.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_GLOPERATION_HPP_
#define SUPERCLUSTER_EDITOR_GLOPERATION_HPP_

#include <functional>
#include <vector>

namespace sc
{
	using OpenGLOperation      = std::function<void(void)>;
	using OpenGLOperationStack = std::vector<OpenGLOperation>;
} // namespace sc

#endif
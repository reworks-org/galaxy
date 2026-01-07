///
/// RenderPipeline.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERPIPELINE_HPP_
#define GALAXY_GRAPHICS_RENDERPIPELINE_HPP_

#include <vector>

#include "galaxy/graphics/gl/ShaderBuffer.hpp"

namespace galaxy
{
	///
	/// GPU configuration.
	///
	struct RenderPipeline final
	{
		///
		/// glEnable features.
		///
		std::vector<unsigned int> enabled;

		///
		/// glDisable features.
		///
		std::vector<unsigned int> disabled;

		///
		/// glCullFace.
		///
		unsigned int cull_face;

		///
		/// glDepthFunc.
		///
		unsigned int depth_func;

		///
		/// glBlendEquation.
		///
		unsigned int blend_equation;

		///
		/// glBlendFunc for GL_SRC_ALPHA.
		///
		unsigned int blend_function;

		///
		/// Pipeline Ubershader.
		///
		unsigned int shader;
	};

} // namespace galaxy

#endif

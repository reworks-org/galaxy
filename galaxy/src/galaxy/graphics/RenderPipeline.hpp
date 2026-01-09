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
		unsigned int cull_face = 0;

		///
		/// glDepthFunc.
		///
		unsigned int depth_func = 0;

		///
		/// glBlendEquation.
		///
		unsigned int blend_equation = 0;

		///
		/// glBlendFunc for GL_SRC_ALPHA.
		///
		unsigned int blend_function = 0;

		///
		/// Pipeline Ubershader.
		///
		unsigned int shader = 0;
	};

} // namespace galaxy

#endif

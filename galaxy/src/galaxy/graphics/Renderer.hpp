///
/// Renderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER_HPP_
#define GALAXY_GRAPHICS_RENDERER_HPP_

#include <ankerl/unordered_dense.h>

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/RenderCommand.hpp"
#include "galaxy/graphics/RenderPass.hpp"
#include "galaxy/graphics/RenderPipeline.hpp"
#include "galaxy/graphics/VertexBatch.hpp"

namespace galaxy
{
	///
	/// OpenGL 4.6 Renderer that tries to mimic vulkan pipeline.
	///
	class Renderer final
	{
	public:
		///
		/// Constructor.
		///
		Renderer();

		///
		/// Destructor.
		///
		~Renderer();

		///
		/// Called only in the update loop when we need to refresh data to render.
		///
		void prepare();

		///
		/// \brief Binds framebuffer, viewport, sissor region and clears buffers.
		///
		/// Call this first.
		///
		/// \param pass RenderPass to bind.
		///
		void begin_pass(RenderPass& pass);

		///
		/// \brief Binds the GPU state specified in pipeline.
		///
		/// Call this second.
		///
		/// \param pipeline RenderPipeline to bind.
		///
		void bind_pipeline(RenderPipeline& pipeline);

		///
		/// \brief Push a renderable object.
		///
		/// Try not to push too many non-quads, only quads are batched.
		///
		/// \param cmd RenderCmd to draw.
		///
		void push_cmd(RenderCmd& cmd) noexcept;

		///
		/// Renders everything configured in this pass.
		///
		/// \param camera Projection and view matrix to render this pass with.
		///
		void end_pass(Camera& camera);

	private:
		///
		/// Quad vertex batch.
		///
		VertexBatch m_quads;

		///
		/// RenderCmd storage.
		///
		std::vector<RenderCmd> m_commands;

		///
		/// Currently active pass.
		///
		RenderPass* m_pass;

		///
		/// Currently active pipeline.
		///
		RenderPipeline* m_pipeline;

		///
		/// Renderable uniforms buffer.
		///
		ShaderBuffer m_uniform_storage;

		///
		/// Camera buffer.
		///
		ShaderBuffer m_camera_storage;

		///
		/// Renderable uniform data storage.
		///
		std::vector<RenderData> m_uniform_data;

		///
		/// Uniform data current max index.
		///
		unsigned int m_uniform_data_index;
	};
} // namespace galaxy

#endif

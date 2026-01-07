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
	class Renderer final
	{
	public:
		///
		///
		///
		Renderer();

		///
		///
		///
		~Renderer();

		///
		/// Called only in the update loop when we need to refresh data to render.
		///
		void prepare();

		///
		///
		///
		void begin_pass(RenderPass& pass);

		///
		///
		///
		void bind_pipeline(RenderPipeline& pipeline);

		///
		/// \brief
		///
		/// Try not to push too many non-quads, only quads are batched.
		///
		void push_cmd(RenderCmd&& cmd);

		///
		///
		///
		void end_pass(Camera& camera);

	private:
		///
		/// All quads get batched.
		///
		VertexBatch m_quads;

		///
		/// RenderCommands.
		///
		std::vector<RenderCmd> m_commands;

		///
		///
		///
		RenderPass* m_pass;

		///
		///
		///
		RenderPipeline* m_pipeline;

		///
		/// Shader uniform data.
		///
		ShaderBuffer m_uniform_storage;

		ShaderBuffer m_camera_storage;

		std::vector<RenderData> m_uniform_data;
	};
} // namespace galaxy

#endif

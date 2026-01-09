///
/// Renderer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/Settings.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "Renderer.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26813)
#endif

namespace galaxy
{
	Renderer::Renderer()
		: m_quads {Settings::max_quads(), 4, galaxy::graphics::gen_default_indices()}
		, m_pass {nullptr}
		, m_pipeline {nullptr}
	{
		m_camera_storage.set_index(GLBufferBinding::CAMERA);
		m_uniform_storage.set_index(GLBufferBinding::UNIFORMS);
	}

	Renderer::~Renderer()
	{
		m_pass     = nullptr;
		m_pipeline = nullptr;
	}

	void Renderer::prepare()
	{
		m_quads.prepare();

		m_commands.clear();
		m_uniform_data.clear();

		m_pass     = nullptr;
		m_pipeline = nullptr;
	}

	void Renderer::begin_pass(RenderPass& pass)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pass.target);
		glViewport(pass.viewport.x, pass.viewport.y, pass.viewport.z, pass.viewport.w);
		glScissor(pass.scissor.x, pass.scissor.y, pass.scissor.z, pass.scissor.w);

		glClearColor(pass.clear_col.x, pass.clear_col.y, pass.clear_col.z, pass.clear_col.w);
		glClearDepth(pass.clear_depth);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		m_pass = &pass;
	}

	void Renderer::bind_pipeline(RenderPipeline& pipeline)
	{
		for (const auto& feature : pipeline.enabled)
		{
			glEnable(feature);
		}

		for (const auto& feature : pipeline.disabled)
		{
			glDisable(feature);
		}

		glCullFace(pipeline.cull_face);
		glDepthFunc(pipeline.depth_func);
		glBlendEquation(pipeline.blend_equation);
		glBlendFunc(GL_SRC_ALPHA, pipeline.blend_function);

		glUseProgram(pipeline.shader);
		glActiveTexture(GL_TEXTURE0);

		// There is no texture sampler object to bind because we use bindless textures.

		m_pipeline = &pipeline;
	}

	void Renderer::push_cmd(RenderCmd&& cmd)
	{
		// Add uniform data first.
		m_uniform_data.emplace_back(std::move(cmd.data));

		// Our index will always be the last inserted element at the time.
		const auto index = static_cast<unsigned int>(m_uniform_data.size()) - 1u;

		// update uniform data location.
		for (auto& vert : cmd.vertices)
		{
			vert.m_index = index;
		}

		// We want to batch quads and just draw the rest regularly.
		if (cmd.type == GLRenderMode::QUADS)
		{
			m_quads.push(cmd.vertices);
		}
		else
		{
			m_commands.emplace_back(cmd);
		}
	}

	void Renderer::end_pass(Camera& camera)
	{
		m_camera_storage.bind();
		m_camera_storage.buffer(1, &camera.get_data());

		m_uniform_storage.bind();
		m_uniform_storage.buffer(static_cast<unsigned int>(m_uniform_data.size()), m_uniform_data.data());

		m_quads.flush();
		m_quads.bind();

		glDrawElements(GL_TRIANGLES, m_quads.count(), GL_UNSIGNED_INT, m_quads.offset());

		for (const auto& cmd : m_commands)
		{
			cmd.vao->bind();
			glDrawElements(static_cast<unsigned int>(cmd.type), cmd.vao->count(), GL_UNSIGNED_INT, cmd.vao->offset());
		}
	}
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

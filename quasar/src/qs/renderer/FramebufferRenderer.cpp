///
/// FramebufferRenderer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/core/Window.hpp"
#include "qs/core/Shader.hpp"

#include "FramebufferRenderer.hpp"

///
/// Core namespace.
///
namespace qs
{
	FramebufferRenderer::FramebufferRenderer(qs::Window* window) noexcept
		:m_window(window)
	{
		m_framebuffer.create(m_window->getWidth(), m_window->getHeight());
		m_sprite.load(m_framebuffer.getGLTexture(), m_framebuffer.getWidth(), m_framebuffer.getHeight());
		m_sprite.create(qs::BufferType::STATIC);
	}

	void FramebufferRenderer::begin() noexcept
	{
		m_framebuffer.bind();
	}

	void FramebufferRenderer::submit(qs::Sprite& sprite, qs::Shader& shader) noexcept
	{
		shader.setUniform("u_projection", m_framebuffer.getProjection());
		shader.setUniform<float>("u_width", static_cast<float>(sprite.getWidth()));
		shader.setUniform<float>("u_height", static_cast<float>(sprite.getHeight()));

		sprite.bind();

		glDrawElements(GL_TRIANGLES, sprite.getCount(), GL_UNSIGNED_INT, nullptr);

		sprite.unbind();
	}

	void FramebufferRenderer::end() noexcept
	{
		m_framebuffer.unbind(*m_window);
	}

	void FramebufferRenderer::render(qs::Shader& postEffect) noexcept
	{
		postEffect.setUniform<float>("u_width", static_cast<float>(m_sprite.getWidth()));
		postEffect.setUniform<float>("u_height", static_cast<float>(m_sprite.getHeight()));

		m_sprite.bind();

		glDrawElements(GL_TRIANGLES, m_sprite.getCount(), GL_UNSIGNED_INT, nullptr);

		m_sprite.unbind();
	}
}
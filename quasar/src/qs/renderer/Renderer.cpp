///
/// Renderer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/core/Shader.hpp"
#include "qs/graphics/Sprite.hpp"
#include "qs/core/RenderTexture.hpp"

#include "Renderer.hpp"

///
/// Core namespace.
///
namespace qs
{
	void Renderer::drawPoint(qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawLine(qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawTriangle(qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawQuad(qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawCircle(qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawPolygon(qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawSprite(qs::Sprite& sprite, qs::Shader& shader) noexcept
	{
		shader.setUniform<float>("u_width", sprite.getWidth());
		shader.setUniform<float>("u_height", sprite.getHeight());

		sprite.bind();
		
		glDrawElements(GL_TRIANGLES, sprite.getCount(), GL_UNSIGNED_INT, nullptr);

		sprite.unbind();
	}

	void Renderer::drawSpriteToTexture(qs::Sprite& sprite, qs::RenderTexture& rt, qs::Shader& shader) noexcept
	{
		shader.setUniform("u_projection", rt.getProjection());
		shader.setUniform<float>("u_width", sprite.getWidth());
		shader.setUniform<float>("u_height", sprite.getHeight());

		sprite.bind();

		glDrawElements(GL_TRIANGLES, sprite.getCount(), GL_UNSIGNED_INT, nullptr);

		sprite.unbind();
	}
}
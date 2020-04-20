///
/// Renderer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/text/Text.hpp"
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
		shader.setUniform<float>("u_width", static_cast<float>(sprite.getWidth()));
		shader.setUniform<float>("u_height", static_cast<float>(sprite.getHeight()));

		sprite.bind();
		
		glDrawElements(GL_TRIANGLES, sprite.getCount(), GL_UNSIGNED_INT, nullptr);

		sprite.unbind();
	}

	void Renderer::drawVAToTexture(qs::VertexArray& va, qs::IndexBuffer& ib, qs::RenderTexture& rt, qs::Shader& shader) noexcept
	{
		shader.setUniform("u_projection", rt.getProjection());

		va.bind();

		glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);

		va.unbind();
	}

	void Renderer::drawSpriteToTexture(qs::Sprite& sprite, qs::RenderTexture& rt, qs::Shader& shader) noexcept
	{
		shader.setUniform("u_projection", rt.getProjection());
		shader.setUniform<float>("u_width", static_cast<float>(sprite.getWidth()));
		shader.setUniform<float>("u_height", static_cast<float>(sprite.getHeight()));

		sprite.bind();

		glDrawElements(GL_TRIANGLES, sprite.getCount(), GL_UNSIGNED_INT, nullptr);

		sprite.unbind();
	}

	void Renderer::drawText(qs::Text& text, qs::Shader& shader) noexcept
	{
		drawSprite(text.asSprite(), shader);
	}
}
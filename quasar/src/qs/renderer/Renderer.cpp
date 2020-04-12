///
/// Renderer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/core/Shader.hpp"
#include "qs/graphics/Sprite.hpp"
//#include "qs/core/RenderTexture.hpp"

#include "Renderer.hpp"

///
/// Core namespace.
///
namespace qs
{
	void Renderer::drawPoint(qs::VertexArray& va, qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawLine(qs::VertexArray& va, qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawTriangle(qs::VertexArray& va) noexcept
	{
		va.bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawQuad(qs::VertexArray& va) noexcept
	{
		va.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawCircle(qs::VertexArray& va, qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawPolygon(qs::VertexArray& va, qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawSprite(qs::Sprite& sprite, qs::Shader& shader) noexcept
	{
		sprite.bind();
		
		glDrawElements(GL_TRIANGLES, sprite.getCount(), GL_UNSIGNED_INT, nullptr);

		sprite.unbind();
	}

	/*void Renderer::drawSpriteToTexture(qs::Sprite& sprite, qs::RenderTexture& rt, qs::Shader& shader) noexcept
	{
		shader.setUniform("u_transform", sprite.getTransformation());
		shader.setUniform("u_projection", rt.getProjection());
		sprite.bind();

		glDrawElements(GL_TRIANGLES, sprite.getCount(), GL_UNSIGNED_INT, nullptr);

		sprite.unbind();
	}*/
}
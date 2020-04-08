///
/// Renderer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/core/Shader.hpp"
#include "qs/graphics/Sprite2D.hpp"
#include "qs/graphics/BatchSprite2D.hpp"

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

	void Renderer::drawTriangle(qs::VertexArray& va, qs::Shader& shader) noexcept
	{
		shader.use();
		va.bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawQuad(qs::VertexArray& va, qs::Shader& shader) noexcept
	{
		shader.use();
		va.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawCircle(qs::VertexArray& va, qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawPolygon(qs::VertexArray& va, qs::Shader& shader) noexcept
	{
	}

	void Renderer::drawSprite2D(qs::Sprite2D& sprite, qs::Shader& shader) noexcept
	{
		shader.setUniform("u_transform", sprite.getTransformation());
		sprite.activate();

		glDrawElements(GL_TRIANGLES, sprite.getIBO().count(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawBatchSprite(qs::BatchSprite2D* spritebatch, qs::Shader& shader)
	{
		shader.setUniform("u_transform", spritebatch->getTransformArray());
		spritebatch->activate();

		glDrawElements(GL_TRIANGLES, spritebatch->count(), GL_UNSIGNED_INT, nullptr);
	}
}
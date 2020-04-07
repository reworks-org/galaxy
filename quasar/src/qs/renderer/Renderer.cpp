///
/// Renderer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "Renderer.hpp"

///
/// Core namespace.
///
namespace qs
{
	Renderer::Renderer() noexcept
	{
	}

	Renderer::~Renderer() noexcept
	{
	}

	void Renderer::drawLine() noexcept
	{
	}

	void Renderer::drawTriangle() noexcept
	{
	}

	void Renderer::drawQuad() noexcept
	{
	}

	void Renderer::drawCircle() noexcept
	{
	}

	void Renderer::drawPolygon() noexcept
	{
	}

	void Renderer::drawSprite2D(qs::Sprite2D& sprite, qs::Shader& shader) noexcept
	{
		shader.setUniform("u_transform", sprite.getTransformation());
		sprite.activate();

		glDrawElements(GL_TRIANGLES, sprite.getIBO().count(), GL_UNSIGNED_INT, nullptr);
	}
}
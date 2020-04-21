///
/// Renderer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/text/Text.hpp"
#include "qs/core/Shader.hpp"
#include "qs/graphics/Point.hpp"
#include "qs/graphics/Line.hpp"
#include "qs/graphics/Sprite.hpp"
#include "qs/graphics/Circle.hpp"
#include "qs/core/RenderTexture.hpp"

#include "Renderer.hpp"

///
/// Core namespace.
///
namespace qs
{
	void Renderer::drawPoint(qs::Point& point, qs::Shader& shader) noexcept
	{
		point.bind();
		shader.setUniform("u_point_size", point.getSize());

		glDrawArrays(GL_POINTS, 0, 1);

		point.unbind();
	}

	void Renderer::drawLine(qs::Line& line) noexcept
	{
		line.bind();

		glDrawArrays(GL_LINES, 0, 2);

		line.unbind();
	}

	void Renderer::drawCircle(qs::Circle& circle) noexcept
	{
		circle.bind();

		glDrawArrays(GL_LINE_LOOP, 0, circle.getCount());

		circle.unbind();
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
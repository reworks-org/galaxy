///
/// Renderer.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/mat4x4.hpp>

#include "qs/core/Shader.hpp"
#include "qs/sprite/Sprite.hpp"
#include "qs/graphics/Line.hpp"
#include "qs/graphics/Point.hpp"
#include "qs/text/Character.hpp"
#include "qs/graphics/Circle.hpp" 
#include "qs/sprite/SpriteBatch.hpp"
#include "qs/sprite/AnimatedSprite.hpp"
#include "qs/texture/RenderTexture.hpp"

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
	}

	void Renderer::drawLine(qs::Line& line) noexcept
	{
		line.bind();

		glDrawArrays(GL_LINES, 0, 2);
	}

	void Renderer::drawCircle(qs::Circle& circle) noexcept
	{
		circle.bind();

		glDrawArrays(GL_LINE_LOOP, 0, circle.getCount());
	}

	void Renderer::drawSprite(qs::Sprite& sprite, qs::Shader& shader) noexcept
	{
		sprite.bind();

		shader.setUniform("u_transform", sprite.getTransformation());
		shader.setUniform("u_opacity", sprite.getOpacity());
		shader.setUniform<float>("u_width", sprite.getWidth());
		shader.setUniform<float>("u_height", sprite.getHeight());

		glDrawElements(GL_TRIANGLES, sprite.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawAnimatedSprite(qs::AnimatedSprite& sprite, qs::Shader& shader) noexcept
	{
		sprite.bind();
		sprite.update();

		shader.setUniform("u_transform", sprite.getTransformation());
		shader.setUniform("u_opacity", sprite.getOpacity());
		shader.setUniform<float>("u_width", sprite.getWidth());
		shader.setUniform<float>("u_height", sprite.getHeight());

		glDrawElements(GL_TRIANGLES, sprite.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawSpriteBatch(qs::SpriteBatch& spritebatch, qs::Shader& shader) noexcept
	{
		spritebatch.bind();

		shader.setUniform<float>("u_width", spritebatch.getWidth());
		shader.setUniform<float>("u_height", spritebatch.getHeight());

		glDrawElements(GL_TRIANGLES, spritebatch.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawSpriteToTexture(qs::Sprite* sprite, qs::RenderTexture& target, qs::Shader& shader) noexcept
	{
		shader.setUniform("u_projection", target.getProjection());

		sprite->bind();
		shader.setUniform("u_transform", sprite->getTransformation());
		shader.setUniform<float>("u_width", sprite->getWidth());
		shader.setUniform<float>("u_height", sprite->getHeight());

		glDrawElements(GL_TRIANGLES, sprite->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::drawCharacter(qs::Character& chr, qs::RenderTexture& target, qs::Shader& shader) noexcept
	{
		chr.bind();
		shader.setUniform("u_projection", target.getProjection());

		glDrawElements(GL_TRIANGLES, chr.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	/*
|	void Renderer::drawText(qs::Text& text, qs::Shader& shader) noexcept
	{
		drawSprite(text.asSprite(), shader);
	}

	void Renderer::drawScene(qs::SpriteBatch& spritebatch, qs::Camera& camera, qs::LightSource& ls) noexcept
	{
		spritebatch.bind();
		spritebatch.update();
		ls.m_shader.bind();

		ls.m_shader.setUniform("u_light_pos", glm::vec3(ls.m_pos.x, camera.getHeight() - ls.m_pos.y, ls.m_zLevel));
		ls.m_shader.setUniform("u_light_colour", ls.m_lightColour);
		ls.m_shader.setUniform("u_ambient_colour", ls.m_ambientColour);
		ls.m_shader.setUniform("u_falloff", ls.m_falloff);
		ls.m_shader.setUniform("u_width", static_cast<float>(spritebatch.getWidth()));
		ls.m_shader.setUniform("u_height", static_cast<float>(spritebatch.getHeight()));
		ls.m_shader.setUniform<glm::mat4>("u_cameraProj", camera.getProj());
		ls.m_shader.setUniform<glm::mat4>("u_cameraView", camera.getTransformation());
		ls.m_shader.setUniform("u_window_resolution", camera.getWidth(), camera.getHeight());

		glDrawElements(GL_TRIANGLES, spritebatch.getCount(), GL_UNSIGNED_INT, nullptr);
	}
	*/
}
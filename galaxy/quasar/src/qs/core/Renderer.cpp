///
/// Renderer.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/mat4x4.hpp>

#include "qs/text/Text.hpp"
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
	void Renderer::draw_point(qs::Point& point, qs::Shader& shader) noexcept
	{
		point.bind();
		shader.set_uniform("u_point_size", point.size());

		glDrawArrays(GL_POINTS, 0, 1);
	}

	void Renderer::draw_line(qs::Line& line) noexcept
	{
		line.bind();

		glDrawArrays(GL_LINES, 0, 2);
	}

	void Renderer::draw_circle(qs::Circle& circle) noexcept
	{
		circle.bind();

		glDrawArrays(GL_LINE_LOOP, 0, circle.count());
	}

	void Renderer::draw_sprite(qs::Sprite& sprite, qs::Shader& shader) noexcept
	{
		sprite.bind();

		shader.set_uniform("u_transform", sprite.get_transform());
		shader.set_uniform("u_opacity", sprite.opacity());
		shader.set_uniform<float>("u_width", static_cast<float>(sprite.width()));
		shader.set_uniform<float>("u_height", static_cast<float>(sprite.height()));

		glDrawElements(GL_TRIANGLES, sprite.count(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::draw_animated_sprite(qs::AnimatedSprite& sprite, qs::Shader& shader) noexcept
	{
		sprite.bind();
		sprite.update();

		shader.set_uniform("u_transform", sprite.get_transform());
		shader.set_uniform("u_opacity", sprite.opacity());
		shader.set_uniform<float>("u_width", static_cast<float>(sprite.width()));
		shader.set_uniform<float>("u_height", static_cast<float>(sprite.height()));

		glDrawElements(GL_TRIANGLES, sprite.count(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::draw_sprite_batch(qs::SpriteBatch& sprite_batch, qs::Shader& shader) noexcept
	{
		sprite_batch.bind();

		shader.set_uniform<float>("u_width", static_cast<float>(sprite_batch.width()));
		shader.set_uniform<float>("u_height", static_cast<float>(sprite_batch.height()));

		glDrawElements(GL_TRIANGLES, sprite_batch.count(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::draw_sprite_to_texture(qs::Sprite* sprite, qs::RenderTexture& target, qs::Shader& shader) noexcept
	{
		shader.set_uniform("u_projection", target.get_projection());

		sprite->bind();
		shader.set_uniform("u_transform", sprite->get_transform());
		shader.set_uniform<float>("u_width", static_cast<float>(sprite->width()));
		shader.set_uniform<float>("u_height", static_cast<float>(sprite->height()));

		glDrawElements(GL_TRIANGLES, sprite->count(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::draw_character(qs::Character* chr, qs::RenderTexture& target, qs::Shader& shader) noexcept
	{
		chr->bind();
		shader.set_uniform("u_projection", target.get_projection());

		glDrawElements(GL_TRIANGLES, chr->count(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::draw_text(qs::Text& text, qs::Shader& shader) noexcept
	{
		text.bind();

		shader.set_uniform("u_transform", text.get_transform());
		shader.set_uniform("u_opacity", text.opacity());

		glDrawElements(GL_TRIANGLES, text.count(), GL_UNSIGNED_INT, nullptr);
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

		ls.m_shader.set_uniform("u_light_pos", glm::vec3(ls.m_pos.x, camera.getHeight() - ls.m_pos.y, ls.m_zLevel));
		ls.m_shader.set_uniform("u_light_colour", ls.m_lightColour);
		ls.m_shader.set_uniform("u_ambient_colour", ls.m_ambientColour);
		ls.m_shader.set_uniform("u_falloff", ls.m_falloff);
		ls.m_shader.set_uniform("u_width", static_cast<float>(spritebatch.getWidth()));
		ls.m_shader.set_uniform("u_height", static_cast<float>(spritebatch.getHeight()));
		ls.m_shader.set_uniform<glm::mat4>("u_cameraProj", camera.getProj());
		ls.m_shader.set_uniform<glm::mat4>("u_cameraView", camera.getTransformation());
		ls.m_shader.set_uniform("u_window_resolution", camera.getWidth(), camera.getHeight());

		glDrawElements(GL_TRIANGLES, spritebatch.getCount(), GL_UNSIGNED_INT, nullptr);
	}
	*/
} // namespace qs
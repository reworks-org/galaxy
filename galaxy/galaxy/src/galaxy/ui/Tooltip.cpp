///
/// Tooltip.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>

#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "Tooltip.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Tooltip::Tooltip(Theme* theme) noexcept
		:m_x(0.0f), m_y(0.0f), m_textStr(""), m_texture(""), m_theme(theme)
	{
		m_theme->getBatch()->add(&m_sprite);
	}

	Tooltip::~Tooltip() noexcept
	{
		m_theme = nullptr;
	}

	void Tooltip::onMove(const protostar::MouseMovedEvent& e) noexcept
	{
		m_x = static_cast<float>(e.m_x + 12);
		m_y = static_cast<float>(e.m_y + 12);
	}

	void Tooltip::render(qs::Camera& camera, std::string_view textShader) noexcept
	{
		auto* shader = m_theme->get<ShaderBook>(textShader);
		
		shader->bind();
		shader->setUniform<glm::mat4>("u_cameraProj", camera.getProj());
		shader->setUniform<glm::mat4>("u_cameraView", camera.getTransformation());

		m_theme->getRenderer()->drawText(m_text, *shader);
	}

	void Tooltip::setText(const std::string& text) noexcept
	{
		m_textStr = text;
	}

	void Tooltip::setColour(const protostar::Colour& colour) noexcept
	{
		m_colour = colour;
	}

	void Tooltip::setTexture(const std::string& texture) noexcept
	{
		m_texture = texture;
	}

	void Tooltip::create(std::string_view font) noexcept
	{
		m_sprite.create(m_theme->getAtlas()->getTexQuad(m_texture), 1);
		m_sprite.setPos(m_x, m_y);

		m_text.load(m_textStr, m_theme->get<galaxy::FontBook>(font), m_colour);
		m_text.create();
	}

	void Tooltip::updateText(const std::string& update) noexcept
	{
		m_text.updateText(update);
	}
}
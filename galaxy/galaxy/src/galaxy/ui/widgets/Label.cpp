///
/// Label.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Shader.hpp>

#include "Label.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Label::Label(const float x, const float y, const std::string& text, const std::string& font, const pr::Colour& col, galaxy::UITheme* theme) noexcept
		:Widget()
	{
		m_theme = theme;
		m_text.load(text, m_theme->getFont(font), col);
		m_text.create(*m_theme->getWindow(), *m_theme->getRenderer(), *m_theme->getTextShader());
		m_text.asSprite().move(x, y);
	}

	Label::~Label()
	{
	}

	void Label::updateText(const std::string& text)
	{
		m_text.updateText(text, *m_theme->getWindow(), *m_theme->getRenderer(), *m_theme->getTextShader());
	}

	void Label::activate() noexcept
	{
		m_text.asSprite().bind();
	}

	void Label::deactivate() noexcept
	{
		m_text.asSprite().unbind();
	}

	void Label::update(pr::ProtectedDouble* dt) noexcept
	{
	}

	void Label::render(qs::Shader& shader) noexcept
	{
		shader.setUniform<glm::mat4>("u_transform", m_text.asSprite().getTransformation());
		shader.setUniform<float>("u_width", m_text.asSprite().getWidth());
		shader.setUniform<float>("u_height", m_text.asSprite().getHeight());

		glDrawElements(GL_TRIANGLES, m_text.asSprite().getCount(), GL_UNSIGNED_INT, nullptr);
	}
}
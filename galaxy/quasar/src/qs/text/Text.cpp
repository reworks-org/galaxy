///
/// Text.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "qs/core/Window.hpp"
#include "qs/core/Shader.hpp"
#include "qs/renderer/Renderer.hpp"
#include "qs/vertex/VertexArray.hpp"
#include "qs/vertex/VertexLayout.hpp"

#include "Text.hpp"

///
/// Core namespace.
///
namespace qs
{
	Text::Text() noexcept
        :m_text(""), m_font(nullptr)
	{
	}

    Text::Text(const std::string& text, qs::Font* font, protostar::Colour& col) noexcept
        :m_text(""), m_font(nullptr)
    {
        load(text, font, col);
    }

    void Text::load(const std::string& text, qs::Font* font, const protostar::Colour& col) noexcept
    {
        m_text = text;
        m_font = font;
        m_colour = col;

        m_texture.create(m_font->getTextWidth(m_text), m_font->getHeight());
        m_texture.updateProjection(0.0f, static_cast<float>(m_texture.getWidth()), 0.0f, static_cast<float>(m_texture.getHeight()));

        m_sprite.load(m_texture.getGLTexture(), m_texture.getWidth(), m_texture.getHeight());
        m_sprite.create<qs::BufferTypeDynamic>();
    }

    void Text::create(qs::Window& window, qs::Renderer& renderer, qs::Shader& shader) noexcept
    {
        m_texture.bind();

        float advX = 0;
        qs::VertexStorage vs;
        vs.reserve(m_text.length() * 4);
        for (auto& c : m_text)
        {
            auto* chr = &m_font->getChars()[c];

            float x = advX + chr->m_bearingX;
            float y = static_cast<float>(0 - (chr->getHeight() - chr->m_bearingY));
            float w = static_cast<float>(chr->getWidth());
            float h = static_cast<float>(chr->getHeight());
            
            vs.clear();
            vs.emplace_back(qs::Vertex{ x, y, m_colour, 0.0f, 1.0f });
            vs.emplace_back(qs::Vertex{ x + w, y, m_colour, 1.0f, 1.0f });
            vs.emplace_back(qs::Vertex{ x + w, y + h, m_colour, 1.0f, 0.0f });
            vs.emplace_back(qs::Vertex{ x, y + h, m_colour, 0.0f, 0.0f });

            glNamedBufferSubData(chr->getVBO().getID(), 0, vs.size() * sizeof(qs::Vertex), vs.data());
            renderer.drawCharacter(*chr, m_texture, shader);

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            advX += (chr->m_advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
        }

        m_texture.unbind(window);
    }

    void Text::updateText(const std::string& text, qs::Window& window, qs::Renderer& renderer, qs::Shader& shader) noexcept
    {
        m_text = text;
        create(window, renderer, shader);
    }

    qs::Sprite& Text::asSprite() noexcept
    {
        return m_sprite;
    }

    qs::RenderTexture& Text::getTexture() noexcept
    {
        return m_texture;
    }
}
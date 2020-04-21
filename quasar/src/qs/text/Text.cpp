///
/// Text.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
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
        :m_text("")
	{
	}

    Text::Text(const std::string& text, qs::Font& font, protostar::Colour& col) noexcept
        :m_text(text), m_font(font), m_colour(col)
    {
    }

    void Text::load(const std::string& text, qs::Font& font, protostar::Colour& col)
    {
        m_text = text;
        m_font = std::move(font);
        m_colour = std::move(col);
    }

    void Text::create(qs::Window& window, qs::Renderer& renderer, qs::Shader& shader)
    {
        m_texture.create(m_font.getTextWidth(m_text), m_font.getTextHeight(m_text));
        m_texture.updateProjection(0.0f, m_texture.getWidth(), 0.0f, m_texture.getHeight());
        m_texture.bind();

        qs::VertexBuffer vb;
        qs::VertexStorage vs = {
            qs::Vertex{ 0.0f, 0.0f, m_colour, 0.0f, 0.0f },
            qs::Vertex{ 0.0f, 0.0f, m_colour, 0.0f, 1.0f },
            qs::Vertex{ 0.0f, 0.0f, m_colour, 1.0f, 1.0f },
            qs::Vertex{ 0.0f, 0.0f, m_colour, 0.0f, 0.0f },
            qs::Vertex{ 0.0f, 0.0f, m_colour, 1.0f, 1.0f },
            qs::Vertex{ 0.0f, 0.0f, m_colour, 1.0f, 0.f }
        };
        vb.create<qs::DynamicBufferType>(vs);

        qs::IndexBuffer ib;
        ib.create<qs::StaticBufferType>(
        {
            0, 1, 2, 3, 4, 5
        });

        qs::VertexLayout layout;
        layout.add<qs::PositionVAType>(2);
        layout.add<qs::ColourVAType>(4);
        layout.add<qs::TexelVAType>(2);

        qs::VertexArray ar;
        ar.create(vb, ib, layout);

        float advX = 0;
        for (auto& c : m_text)
        {
            auto& map = m_font.getChars();
            auto chr = map[c];

            float x = advX + chr.m_bearingX;
            float y = 0 - (chr.m_height - chr.m_bearingY);
            float w = chr.m_width;
            float h = chr.m_height;
            
            vs.clear();
            vs.push_back(qs::Vertex{ x, y + h, m_colour, 0.0f, 0.0f });
            vs.push_back(qs::Vertex{ x, y, m_colour, 0.0f, 1.0f });
            vs.push_back(qs::Vertex{ x + w, y, m_colour, 1.0f, 1.0f });
            vs.push_back(qs::Vertex{ x, y + h, m_colour, 0.0f, 0.0f });
            vs.push_back(qs::Vertex{ x + w, y, m_colour, 1.0f, 1.0f });
            vs.push_back(qs::Vertex{ x + w, y + h, m_colour, 1.0f, 0.f });

            glNamedBufferSubData(vb.getID(), 0, vs.size() * sizeof(qs::Vertex), vs.data());

            glBindTexture(GL_TEXTURE_2D, chr.m_id);
            renderer.drawVAToTexture(ar, ib, m_texture, shader);

            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            advX += (chr.m_advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64)
        }

        m_texture.unbind(window);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_sprite.load(m_texture.getGLTexture(), m_texture.getWidth(), m_texture.getHeight());
        m_sprite.create(qs::BufferType::DYNAMIC);
        m_sprite.move(0.0f, 0.0f);
        m_sprite.applyTransforms();
    }

    void Text::updateText(const std::string& text) noexcept
    {
        m_text = text;
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
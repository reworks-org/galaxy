///
/// Text.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "qs/core/Window.hpp"
#include "qs/core/Shader.hpp"
#include "qs/vertex/VertexArray.hpp"
#include "qs/vertex/VertexLayout.hpp"

#include "Text.hpp"

///
/// Core namespace.
///
namespace qs
{
	Text::Text() noexcept
        :m_text(""), m_font(nullptr), m_opacity(1.0f)
	{
	}

    void Text::load(const std::string& text, qs::Font* font, const protostar::Colour& col) noexcept
    {
        m_text = text;
        m_font = font;
        m_colour = col;
    }

    void Text::create() noexcept
    {
        IndexStorage is;
        VertexStorage<qs::SpriteVertex> vs;

        unsigned int count = 0;
        for (auto& c : m_text)
        {
            for (auto& vertex : m_font->getChar(c)->getVertexs())
            {
                vs.push_back(vertex);
            }

            is.push_back(0 + count);
            is.push_back(1 + count);
            is.push_back(3 + count);
            is.push_back(1 + count);
            is.push_back(2 + count);
            is.push_back(3 + count);

            count += 4;
        }

        m_vertexBuffer.create<qs::SpriteVertex, qs::BufferTypeDynamic>(vs);
        m_indexBuffer.create<qs::BufferTypeStatic>(is);

        m_layout.add<qs::SpriteVertex, qs::VATypePosition>(2);
        m_layout.add<qs::SpriteVertex, qs::VATypeTexel>(2);
        m_layout.add<qs::SpriteVertex, qs::VATypeOpacity>(1);

        m_vertexArray.create<qs::SpriteVertex>(m_vertexBuffer, m_indexBuffer, m_layout);
    }

    void Text::updateText(const std::string& text) noexcept
    {
        m_text = text;

        IndexStorage is;
        VertexStorage<qs::SpriteVertex> vs;

        unsigned int count = 0;
        for (auto& c : m_text)
        {
            for (auto& vertex : m_font->getChar(c)->getVertexs())
            {
                vs.push_back(vertex);
            }

            is.push_back(0 + count);
            is.push_back(1 + count);
            is.push_back(3 + count);
            is.push_back(1 + count);
            is.push_back(2 + count);
            is.push_back(3 + count);

            count += 4;
        }

        m_vertexBuffer.create<qs::SpriteVertex, qs::BufferTypeDynamic>(vs);
        m_indexBuffer.create<qs::BufferTypeStatic>(is);
    }

    void Text::setOpacity(float opacity) noexcept
    {
        m_opacity = opacity;
    }

    const float Text::getOpacity() const noexcept
    {
        return m_opacity;
    }

    void Text::bind() noexcept
    {
        m_vertexArray.bind();
        glBindTexture(GL_TEXTURE_2D, m_font->getTexture()->getGLTexture());
    }

    void Text::unbind() noexcept
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        m_vertexArray.bind();
    }
}
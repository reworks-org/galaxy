///
/// Button.cpp
/// starlight
///
/// Created by reworks on 19/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Button.hpp"

namespace sl
{
	Button::Button(const int x, const int y, const std::array<std::string, 3>& textures)
		:Widget({x, y, 0, 0}), m_callback(nullptr)
	{
		// Register events.
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<Button, &Button::receivePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseReleasedEvent>().connect<Button, &Button::receiveRelease>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<Button, &Button::recieveMoved>(this);

		// Load each bitmap from the array and check for errors.
		for (auto i = 0; i < 3; ++i)
		{
			m_textures[i] = al_load_bitmap(textures[i].c_str());
			if (!m_textures[i])
			{
				LOG_S(FATAL) << "Failed to load texture: " << textures[i] << " Errno: " << al_get_errno();
			}
		}

		// Set dimensions.
		m_bounds.m_width = al_get_bitmap_width(m_textures[0]);
		m_bounds.m_height = al_get_bitmap_height(m_textures[0]);
	}

	Button::Button(const int x, const int y, const std::string& text, const std::string& font, const std::array<ALLEGRO_COLOR, 3>& colors)
		:Widget({ x, y, 0, 0 }), m_callback(nullptr)
	{	
		// Register events.
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<Button, &Button::receivePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseReleasedEvent>().connect<Button, &Button::receiveRelease>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<Button, &Button::recieveMoved>(this);

		// Make sure font is valid.
		ALLEGRO_FONT* ttf = Locator::fontBook->get(font);
		if (!ttf)
		{
			LOG_S(FATAL) << "Could not find font: " << font;
		}
		else
		{
			// Find correct button size.
			m_bounds.m_width = al_get_text_width(ttf, text.c_str());
			m_bounds.m_height = al_get_font_line_height(ttf);

			for (auto i = 0; i < 3; ++i)
			{
				// Create bitmap, draw text to it, then store it. It does not need to be freed because it is freed in the destructor.
				ALLEGRO_BITMAP* bitmap = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
				al_set_target_bitmap(bitmap);
				al_clear_to_color(al_map_rgba(0, 0, 0, 0));

				al_draw_text(ttf, colors[i], 0, 0, ALLEGRO_ALIGN_LEFT, text.c_str());

				al_flip_display();
				m_textures[i] = bitmap;
			}

			al_set_target_backbuffer(Locator::window->getDisplay());
		}
	}

	Button::Button(const sol::table& table)
		:Widget({ 0, 0, 0, 0 }), m_callback(nullptr)
	{
		// Register events.
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<Button, &Button::receivePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseReleasedEvent>().connect<Button, &Button::receiveRelease>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<Button, &Button::recieveMoved>(this);

		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Get texture data.
		std::array<std::string, 3> textures =
		{
			table.get<std::string>("defaultTexture"),
			table.get<std::string>("pressedTexture"),
			table.get<std::string>("hoverTexture")
		};

		// Load each bitmap from the array and check for errors.
		for (auto i = 0; i < 3; ++i)
		{
			m_textures[i] = al_load_bitmap(textures[i].c_str());
			if (!m_textures[i])
			{
				LOG_S(FATAL) << "Failed to load texture: " << textures[i] << " Errno: " << al_get_errno();
			}
		}

		// Set dimensions.
		m_bounds.m_width = al_get_bitmap_width(m_textures[0]);
		m_bounds.m_height = al_get_bitmap_height(m_textures[0]);
	}

	Button::~Button()
	{
		for (ALLEGRO_BITMAP* texture : m_textures)
		{
			if (texture)
			{
				al_destroy_bitmap(texture);
			}
		}
	}

	void Button::update(const double dt)
	{
		if (m_isVisible)
		{
			if (m_state == Button::State::PRESSED)
			{
				m_callback();
			}
		}
	}

	void Button::render()
	{
		if (m_isVisible)
		{
			// Simply render depending on button state.
			switch (m_state)
			{
			case Button::State::DEFAULT:
				al_draw_bitmap(m_textures[0], m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);
				break;

			case Button::State::PRESSED:
				al_draw_bitmap(m_textures[1], m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);
				break;

			case Button::State::HOVER:
				al_draw_bitmap(m_textures[2], m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);

				if (m_tooltip)
				{
					m_tooltip->draw();
				}
				break;
			}
		}
	}

	void Button::receivePress(const sl::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			// If the mouse cursor is greater than the x axis but less than the total width of the button, and
			// Less than the height of the cursor, but greather than the y of the cursor take its height.

			int topleft = m_bounds.m_x + m_offsetX;
			int topright = topleft + m_bounds.m_width;
			int top = m_bounds.m_y + m_offsetY;
			int bottom = top + m_bounds.m_height;

			if (((e.m_x >= topleft) && (e.m_x <= topright) && (e.m_y >= top) && (e.m_y <= bottom)) && e.m_button == 1)
			{
				m_state = Button::State::PRESSED;
			}
		}
	}

	void Button::receiveRelease(const sl::MouseReleasedEvent& e)
	{
		if (m_isVisible)
		{
			// If the mouse cursor is greater than the x axis but less than the total width of the button, and
			// Less than the height of the cursor, but greather than the y of the cursor take its height.

			int topleft = m_bounds.m_x + m_offsetX;
			int topright = topleft + m_bounds.m_width;
			int top = m_bounds.m_y + m_offsetY;
			int bottom = top + m_bounds.m_height;

			if ((e.m_x >= topleft) && (e.m_x <= topright) && (e.m_y >= top) && (e.m_y <= bottom))
			{
				m_state = Button::State::HOVER;
			}
			else
			{
				m_state = Button::State::DEFAULT;
			}
		}
	}

	void Button::recieveMoved(const sl::MouseMovedEvent& e)
	{
		if (m_isVisible)
		{
			// If the mouse cursor is greater than the x axis but less than the total width of the button, and
			// Less than the height of the cursor, but greather than the y of the cursor take its height.

			int topleft = m_bounds.m_x + m_offsetX;
			int topright = topleft + m_bounds.m_width;
			int top = m_bounds.m_y + m_offsetY;
			int bottom = top + m_bounds.m_height;

			if ((e.m_x >= topleft) && (e.m_x <= topright) && (e.m_y >= top) && (e.m_y <= bottom))
			{
				m_state = Button::State::HOVER;
			}
			else
			{
				m_state = Button::State::DEFAULT;
			}
		}
	}

	void Button::registerCallback(const std::function<void()>& callback)
	{
		m_callback = callback;
	}
}
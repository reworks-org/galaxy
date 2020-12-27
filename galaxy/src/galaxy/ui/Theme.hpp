///
/// UITheme.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_UITHEME_HPP_
#define GALAXY_UI_UITHEME_HPP_

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/TextureAtlas.hpp"
#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/ShaderBook.hpp"

namespace galaxy
{
	namespace graphics
	{
		class Window;
	} // namespace graphics

	namespace ui
	{
		///
		/// Defines the theme used by the GUI.
		///
		class UITheme final
		{
		public:
			///
			/// Constructor.
			///
			UITheme();

			///
			/// Copy constructor.
			///
			UITheme(const UITheme&) = delete;

			///
			/// Copy assignment operator.
			///
			UITheme& operator=(const UITheme&) = delete;

			///
			/// Destructor.
			///
			~UITheme() = default;

			///
			/// The textureatlas for the GUI.
			///
			TextureAtlas m_atlas;

			///
			/// Fonts.
			///
			res::FontBook* m_fonts;

			///
			/// Shaders.
			///
			res::ShaderBook* m_shaders;

			///
			/// Window pointer.
			///
			graphics::Window* m_window;

			///
			/// Renderer pointer.
			///
			Renderer* m_renderer;

			///
			/// Font colour.
			///
			graphics::Colour m_font_col;
		};
	} // namespace ui
} // namespace galaxy

#endif
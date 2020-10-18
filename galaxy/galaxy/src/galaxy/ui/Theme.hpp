///
/// UITheme.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UITHEME_HPP_
#define GALAXY_UITHEME_HPP_

#include <qs/graphics/TextureAtlas.hpp>

#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/ShaderBook.hpp"

namespace qs
{
	class Window;
	class Renderer;
} // namespace qs

///
/// Core namespace.
///
namespace galaxy
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
		/// Move constructor.
		///
		UITheme(UITheme&&);

		///
		/// Copy assignment operator.
		///
		UITheme& operator=(const UITheme&) = delete;

		///
		/// Move assignment operator.
		///
		UITheme& operator=(UITheme&&);

		///
		/// Destructor.
		///
		~UITheme() = default;

		///
		/// The textureatlas for the GUI.
		///
		qs::TextureAtlas m_atlas;

		///
		/// Fonts.
		///
		galaxy::FontBook* m_fonts;

		///
		/// Shaders.
		///
		galaxy::ShaderBook* m_shaders;

		///
		/// Window pointer.
		///
		qs::Window* m_window;

		///
		/// Renderer pointer.
		///
		qs::Renderer* m_renderer;

		///
		/// Font colour.
		///
		pr::Colour m_font_col;
	};
} // namespace galaxy

#endif
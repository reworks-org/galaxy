///
/// Theme.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_THEME_HPP_
#define GALAXY_UI_THEME_HPP_

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// Defines the theme used by the GUI.
		///
		class Theme final
		{
		public:
			///
			/// Constructor.
			///
			Theme() noexcept;

			///
			/// Move constructor.
			///
			Theme(Theme&&) noexcept;

			///
			/// Move assignment operator.
			///
			Theme& operator=(Theme&&) noexcept;

			///
			/// Destructor.
			///
			~Theme() noexcept = default;

		public:
			///
			/// Font colour.
			///
			graphics::Colour m_font_col;

			///
			/// Camera view for GUI.
			///
			graphics::Camera m_camera;

			///
			/// Spritebatch.
			///
			graphics::SpriteBatch m_sb;

		private:
			///
			/// Copy constructor.
			///
			Theme(const Theme&) = delete;

			///
			/// Copy assignment operator.
			///
			Theme& operator=(const Theme&) = delete;
		};
	} // namespace ui
} // namespace galaxy

#endif
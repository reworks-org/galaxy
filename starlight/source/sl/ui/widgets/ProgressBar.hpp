///
/// ProgressBar.hpp
/// starlight
///
/// Created by reworks on 16/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PROGRESSBAR_HPP_
#define STARLIGHT_PROGRESSBAR_HPP_

#include "sl/math/Vector4.hpp"

#include "sl/ui/Widget.hpp"

namespace sl
{
	///
	/// UI bar that is used to track progress of something.
	///
	class ProgressBar final : public Widget
	{
	public:
		///
		/// Primitives constructor.
		///
		/// \param bounds x,y,w,h of progress bar rectangle, specifically the outline.
		/// \param container Colour of the progress bar background.
		/// \param bar Colour of the progress bar itself.
		///
		ProgressBar(const sl::Rect<int>& bounds, const ALLEGRO_COLOR container, const ALLEGRO_COLOR bar);

		///
		/// Texture constructor.
		///
		/// \param vec4 w = x-pos relative to panel.
		///				x = y-pos relative to panel.
		///				y = x-pos relative to container texture.
		///				z = y-pos relative to container texture.
		/// \param container Texture of the progress bar container in theme to use.
		/// \param bar Seperate texture for the progress bar itself, not its outline in theme to use.
		/// \param theme Theme of the widget to use.
		///
		ProgressBar(const sl::Vector4<int>& vec4, const std::string& container, const std::string& bar, UITheme* theme);

		///
		/// Lua Constructor.
		///
		/// \param table sol::table to create widget from.
		/// \param theme Theme of the widget to use.
		///
		ProgressBar(const sol::table& table, UITheme* theme);

		///
		/// Destructor.
		///
		~ProgressBar() noexcept;

		///
		/// \brief Allows for ProgressBar to recieve MouseMovedEvents. Automatically registered with entt.
		///
		/// This is to be used with entt's dispatcher (sl::Locator::dispatcher).
		///
		/// \param e MouseMovedEvent object.
		///
		void recieve(const sl::MouseMovedEvent& e);

		///
		/// Update the widget.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt) override;

		///
		/// Render the widget.
		///
		void render() override;

		///
		/// Set progress of bar.
		///
		/// \param progress Progress is from 0.0f to 1.0f i.e. 0.54f is 54%.
		///
		void setProgress(const float progress);

		///
		/// Get current progress of bar.
		///
		/// \return const float of progress.
		///
		const float getProgress() const;

		///
		/// Returns current progress of bar in a percentage.
		///
		/// \return const float progress as pertentage.
		///
		const float getPercentage() const;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		ProgressBar() = delete;

	private:
		///
		/// Bounds for the bar texture.
		///
		sl::Rect<int> m_barBounds;

		///
		/// Current progress percentage of this bar. From 0.0f - 1.0f. I.e. 0.54f is 54%.
		///
		float m_progress;

		///
		/// Outline/background texture.
		///
		ALLEGRO_BITMAP* m_container;

		///
		/// Progress bar texture.
		///
		ALLEGRO_BITMAP* m_bar;
	};
}

#endif
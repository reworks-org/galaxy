///
/// ProgressBar.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PROGRESSBAR_HPP_
#define GALAXY_PROGRESSBAR_HPP_

#include <protostar/shapes/Rect.hpp>
#include <protostar/graphics/Colour.hpp>

#include "galaxy/ui/ui/Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// UI bar that is used to track progress of something.
	///
	class ProgressBar final : public Widget
	{
	public:
		///
		/// Texture constructor.
		///
		/// \param containerBarCoords   x = x-pos of container relative to panel.
		///								y = y-pos of container relative to panel.
		///								w = x-pos of progress bar relative to container texture.
		///								h = y-pos of progress bar relative to container texture.
		/// \param container Texture of the progress bar container in theme to use.
		/// \param bar Seperate texture for the progress bar itself, not its outline in theme to use.
		/// \param theme Theme of the widget to use.
		///
		ProgressBar(const protostar::Rect<int>& containerBarCoords, const std::string& container, const std::string& bar, UITheme* theme);

		///
		/// Primitives constructor.
		///
		/// \param bounds x,y,w,h of progress bar rectangle, specifically the outline.
		/// \param container Colour of the progress bar background.
		/// \param bar Colour of the progress bar itself.
		/// \param loader ResourceLoader to use.
		///
		ProgressBar(const protostar::Rect<int>& bounds, const protostar::Colour& container, const protostar::Colour& bar, galaxy::ResourceLoader* loader);
		
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
		void recieve(const protostar::MouseMovedEvent& e);

		///
		/// Update the widget.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt) override;

		///
		/// Render the widget.
		///
		void render(galaxy::Renderer* renderer) override;

		///
		/// Set progress of bar.
		///
		/// \param progress Progress is from 0.0f to 1.0f i.e. 0.54f is 54%.
		///
		void setProgress(const int progress);

		///
		/// Get current progress of bar.
		///
		/// \return const int of progress.
		///
		const float getProgress() const;

		///
		/// Returns current progress of bar in a percentage.
		///
		/// \return const int progress as pertentage.
		///
		const float getPercentage() const;

		///
		/// \brief Set the offset of the widget from the panel. Called for you in the Panel::add widget function.
		///
		/// It should look like this:
		/// m_bounds.m_x += x;
		/// m_bounds.m_y += y;
		///
		/// \param x x-pos of the panel.
		/// \param y y-pos of the panel.
		///
		void setOffset(const int x, const int y) override;

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
		protostar::Rect<int> m_barBounds;

		///
		/// Current progress percentage of this bar. From 0.0f - 1.0f. I.e. 0.54f is 54%.
		///
		float m_progress;

		///
		/// Outline/background texture.
		///
		TexturePtr m_container;

		///
		/// Progress bar texture.
		///
		TexturePtr m_bar;
	};
}

#endif
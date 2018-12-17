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
		/// \param foreCol Colour of the progress bar itself.
		/// \param backCol Colour of the progress bar background.
		///
		ProgressBar(const sl::Rect<int>& bounds, const ALLEGRO_COLOR foreCol, const ALLEGRO_COLOR backCol);

		///
		/// Image constructor.
		///
		/// \param bounds x,y,w,h of progress bar rectangle, specifically the outline.
		/// \param barBounds x,y,w,h Of the interal bar, in order to position it correctly within an outline texture.
		/// \param image Image of the progress bar container.
		/// \param col Colour of the progress bar.
		///
		ProgressBar(const sl::Rect<int>& bounds, const sl::Rect<int> barBounds, const std::string& image, const ALLEGRO_COLOR col);

		///
		/// Image constructor.
		///
		/// \param bounds x,y,w,h of progress bar rectangle, specifically the outline.
		/// \param barBounds x,y,w,h Of the interal bar, in order to position it correctly within an outline texture.
		/// \param image Image of the progress bar container.
		/// \param barImage Seperate texture for the progress bar itself, not its outline.
		///
		ProgressBar(const sl::Rect<int>& bounds, const sl::Rect<int> barBounds, const std::string& image, const std::string& barImage);

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
		void update() override;

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
		ALLEGRO_BITMAP* m_background;

		///
		/// Progress bar texture.
		///
		ALLEGRO_BITMAP* m_bar;
	};
}

#endif
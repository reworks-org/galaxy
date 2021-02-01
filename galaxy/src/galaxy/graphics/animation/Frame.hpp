///
/// Frame.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ANIMATION_FRAME_HPP_
#define GALAXY_GRAPHICS_ANIMATION_FRAME_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/graphics/Rect.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A single frame of an animation.
		///
		struct Frame final
		{
			///
			/// Constructor.
			///
			Frame() noexcept;

			///
			/// Argument constructor.
			///
			/// \param region Texture region on the texture atlas.
			/// \param time_per_frame Total time to display this frame for.
			///
			Frame(const graphics::fRect& region, const double time_per_frame) noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON object.
			///
			Frame(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~Frame() noexcept = default;

			///
			/// Region on the texture atlas.
			///
			graphics::fRect m_region;

			///
			/// Time to spend on this frame in milliseconds.
			///
			double m_time_per_frame;
		};
	} // namespace graphics
} // namespace galaxy

#endif
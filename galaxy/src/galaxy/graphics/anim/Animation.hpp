///
///
/// Animation.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ANIM_ANIMATION_HPP_
#define GALAXY_GRAPHICS_ANIM_ANIMATION_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/graphics/anim/Frame.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace graphics
	{

		// want to load a series of frames based on a texture. each frame should have drawable rect offset + time per frame

		///
		/// 2D animation information.
		///
		class Animation final
		{
		  public:
			///
			/// Constructor.
			///
			Animation();

			///
			/// Move constructor.
			///
			Animation(Animation&&);

			///
			/// Move assignment operator.
			///
			Animation& operator=(Animation&&);

			///
			/// Copy constructor.
			///
			Animation(const Animation&);

			///
			/// Copy assignment operator.
			///
			Animation& operator=(const Animation&);

			///
			/// Destructor.
			///
			~Animation();

			///
			/// Load a json file from VFS and set frames from that.
			///
			/// \param file File in VFS. JSON file must be an array.
			///
			/// \return True if successful.
			///
			[[nodiscard]] bool load(const std::string& file);

			///
			/// Loads frames from json object.
			///
			/// \param json Must be an array of json objects.
			///
			void set_frames(const nlohmann::json& json);

			///
			/// Set from an existing vector of frames.
			///
			/// \param frames List of frames to use for this animation.
			///
			void set_frames(const meta::vector<Frame>& frames);

			///
			/// Advance to the next frame.
			///
			void next();

			///
			/// Go back to the previous frame.
			///
			void prev();

			///
			/// Sets animation back to beginning.
			///
			void restart();

			///
			/// Get currently active frame.
			///
			/// \return Reference to active frame.
			///
			[[nodiscard]] Frame& current();

			///
			/// Get total frames.
			///
			/// \return std::size_t.
			///
			[[nodiscard]] std::size_t total() const;

			///
			/// Get current frame index.
			///
			/// \return std::size_t.
			///
			[[nodiscard]] std::size_t index() const;

			///
			/// Get the frames.
			///
			/// \return Reference to frame list.
			///
			[[nodiscard]] meta::vector<Frame>& frames();

		  public:
			///
			/// Speed of the animation.
			/// Multiplier, so 1.0f is regular speed.
			///
			double m_speed;

			///
			/// Animation name.
			///
			std::string m_name;

		  private:
			///
			/// Index of the current active frame.
			///
			std::size_t m_index;

			///
			/// Frames belonging to this animation.
			///
			meta::vector<Frame> m_frames;
		};
	} // namespace graphics
} // namespace galaxy

#endif

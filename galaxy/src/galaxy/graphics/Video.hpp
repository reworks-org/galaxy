///
/// Video.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VIDEO_HPP_
#define GALAXY_GRAPHICS_VIDEO_HPP_

#include <string>

#include <raylib.hpp>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Raylib support for videos.
		///
		class Video final : public rl::MediaStream
		{
		public:
			///
			/// Constructor.
			///
			Video() noexcept;

			///
			/// Move constructor.
			///
			Video(Video&&) noexcept;

			///
			/// Move assignment operator.
			///
			[[maybe_unused]]
			Video& operator=(Video&&) noexcept;

			///
			/// Destructor.
			///
			~Video() noexcept;

			///
			/// Load a MediaStream from a file.
			///
			/// \param file_name Path to the movie file.
			///
			void load(const std::string& file_name) noexcept;

			///
			/// Load a MediaStream from a file with flags.
			///
			/// \param file_name Path to the movie file.
			/// \param flags Combination of MediaLoadFlag values.
			///
			void load_ex(const std::string& file_name, const rl::MediaLoadFlag flags) noexcept;

			///
			/// Check if a MediaStream is valid (loaded and initialized).
			///
			/// \return True if media is valid; false otherwise.
			///
			[[nodiscard]]
			bool valid() const noexcept;

			///
			/// Retrieve properties of the loaded media.
			///
			/// \return Filled MediaProperties structure on success; empty structure on failure.
			///
			[[nodiscard]]
			ray::MediaProperties get_properties() const noexcept;

			///
			/// Update a MediaStream.
			///
			/// \return True on success; false otherwise.
			///
			[[maybe_unused]]
			bool update() noexcept;

			///
			/// Update a MediaStream with a specified delta time.
			///
			/// \param delta_time Time in seconds since the last update.
			///
			/// \return True on success; false otherwise.
			///
			[[maybe_unused]]
			bool update_ex(const double delta_time) noexcept;

			///
			/// Get the state of a MediaStream.
			///
			/// \return Current state; MEDIA_STATE_INVALID on failure.
			///
			[[nodiscard]]
			ray::MediaState get_state() const noexcept;

			///
			/// Set the state of a MediaStream (play, pause, or stop).
			///
			/// \param new_state Desired state.
			///
			/// \return The new state on success; MEDIA_STATE_INVALID on failure.
			///
			[[maybe_unused]]
			int set_state(const ray::MediaState new_state) noexcept;

			///
			/// Get the playback position of a MediaStream in seconds.
			///
			[[nodiscard]]
			double get_position() const noexcept;

			///
			/// Set the playback position of a MediaStream.
			///
			/// \param time_sec Desired position in seconds.
			///
			/// \return True on success; false otherwise.
			///
			[[maybe_unused]]
			bool set_position(const double time_sec) noexcept;

			///
			/// Enable or disable loop playback for a MediaStream.
			///
			/// \param loop_play True to enable looping; false to disable.
			///
			/// \return True on success; false otherwise.
			///
			[[maybe_unused]]
			bool set_looping(const bool loop_play) noexcept;

			///
			/// Set a global configuration property.
			///
			/// \param flag One of MediaConfigFlag values.
			/// \param value New property value.
			///
			/// \return 0 on success; -1 on failure.
			///
			[[maybe_unused]]
			int set_flag(const ray::MediaConfigFlag flag, const int value) noexcept;

			///
			/// Get a global configuration property.
			///
			/// \param flag One of MediaConfigFlag values.
			///
			/// \return Property value; negative on failure.
			///
			[[nodiscard]]
			int get_flag(const ray::MediaConfigFlag flag) const noexcept;

			///
			/// Unload a MediaStream and free its associated memory.
			///
			void unload() noexcept;

		private:
			///
			/// Copy constructor.
			///
			Video(const Video&) = delete;

			///
			/// Copy assignment operator.
			///
			Video& operator=(const Video&) = delete;
		};
	} // namespace graphics
} // namespace galaxy

#endif

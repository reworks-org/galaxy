///
/// AudioEngine.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MEDIA_AUDIOENGINE_HPP_
#define GALAXY_MEDIA_AUDIOENGINE_HPP_

#include <miniaudio.h>

namespace galaxy
{
	namespace media
	{
		///
		/// MiniAudio high level API engine.
		///
		class AudioEngine final
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param listener_count Amount of listeners to spacialize audio around.
			///
			AudioEngine(const int listener_count);

			///
			/// Destructor.
			///
			~AudioEngine();

			///
			/// Stop all playing audio.
			///
			void stop();

			///
			/// \brief Set a listeners position.
			///
			/// Coordinate system is where positive X points right, positive Y points up and negative Z points forward.
			///
			/// \param id Listener index / id.
			/// \param x X Coord.
			/// \param y Y Coord.
			/// \param z Z Coord.
			///
			void set_listener_position(const unsigned int id, const float x, const float y, const float z);

			///
			/// \brief Set a listeners direction.
			///
			/// Direction is a forward vector.
			///
			/// \param id Listener index / id.
			/// \param x X Coord.
			/// \param y Y Coord.
			/// \param z Z Coord.
			///
			void set_listener_direction(const unsigned int id, const float x, const float y, const float z);

			///
			/// \brief Set listener world up vector.
			///
			/// World up is an up vector.
			///
			/// \param id Listener index / id.
			/// \param x X Coord.
			/// \param y Y Coord.
			/// \param z Z Coord.
			///
			void set_listener_world_up(const unsigned int id, const float x, const float y, const float z);

			///
			/// \brief Directional attenuation.
			///
			/// The listener can have a cone the controls how sound is attenuated based on the listener's direction.
			/// When a sound is between the inner and outer cones, it will be attenuated between 1 and the cone's outer gain.
			/// When a sound is inside the inner code, no directional attenuation is applied. When the sound is in between the inner and outer cones, the
			/// attenuation will be interpolated between 1 and the outer gain.
			///
			/// \param id Listener index / id.
			/// \param inner_angle In radians.
			/// \param outer_angle In radians.
			/// \param outer_gain Value attenuation will be set to when the sound is outside of the outer cone.
			///
			void set_listener_cone(const unsigned int id, const float inner_angle, const float outer_angle, const float outer_gain);

			///
			/// Toggle a listener.
			///
			/// \param id Listener index / id.
			/// \param enable True to enable, false to disable.
			///
			void toggle_listener(const unsigned int id, const bool enable);

			///
			/// Sets master sound effect volume.
			///
			/// \param volume Scales linearly. 0 is muted. 1 is default. 1+ is gain.
			///
			void set_sfx_volume(const float volume);

			///
			/// Sets master music volume.
			///
			/// \param volume Scales linearly. 0 is muted. 1 is default. 1+ is gain.
			///
			void set_music_volume(const float volume);

			///
			/// Sets master dialogue volume.
			///
			/// \param volume Scales linearly. 0 is muted. 1 is default. 1+ is gain.
			///
			void set_dialogue_volume(const float volume);

			///
			/// Get SFX engine.
			///
			/// \return Pointer to ma_engine structure.
			///
			[[nodiscard]] ma_engine* get_sfx_engine();

			///
			/// Get music engine.
			///
			/// \return Pointer to ma_engine structure.
			///
			[[nodiscard]] ma_engine* get_music_engine();

			///
			/// Get dialogue engine.
			///
			/// \return Pointer to ma_engine structure.
			///
			[[nodiscard]] ma_engine* get_dialogue_engine();

		private:
			///
			/// Constructor.
			///
			AudioEngine() = delete;

			///
			/// Copy constructor.
			///
			AudioEngine(const AudioEngine&) = delete;

			///
			/// Move constructor.
			///
			AudioEngine(AudioEngine&&) = delete;

			///
			/// Copy assignment operator.
			///
			AudioEngine& operator=(const AudioEngine&) = delete;

			///
			/// Move assignment operator.
			///
			AudioEngine& operator=(AudioEngine&&) = delete;

		private:
			///
			/// Handles sound effects.
			///
			ma_engine m_sfx_engine;

			///
			/// Handles music.
			///
			ma_engine m_music_engine;

			///
			/// Handles dialogue sounds.
			///
			ma_engine m_dialogue_engine;

			///
			/// Log structure.
			///
			ma_log m_log;

			///
			/// Callback for when logging is done.
			///
			ma_log_callback m_callback;
		};
	} // namespace media
} // namespace galaxy

#endif
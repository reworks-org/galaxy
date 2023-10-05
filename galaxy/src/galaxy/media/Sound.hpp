///
/// Sound.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MEDIA_SOUND_HPP_
#define GALAXY_MEDIA_SOUND_HPP_

#include <string_view>

#include <miniaudio.h>

#include "galaxy/media/SoundType.hpp"

namespace galaxy
{
	namespace media
	{
		///
		/// An audio file to play.
		///
		class Sound final
		{
		public:
			///
			/// Constructor.
			///
			Sound();

			///
			/// \brief Argument constructor.
			///
			/// Calls load().
			///
			/// \param type Classification of the sound.
			/// \param file File to load.
			///
			Sound(SoundType type, std::string_view file);

			///
			/// Destructor.
			///
			~Sound();

			///
			/// Load a sound from disk.
			///
			/// \param type Classification of the sound.
			/// \param file File to load.
			///
			void load(SoundType type, std::string_view file);

			///
			/// Start playing the sound.
			///
			void play();

			///
			/// Pause the sound.
			///
			void pause();

			///
			/// Stop playing the sound.
			///
			void stop();

			///
			/// Fade sound in.
			///
			/// \param ms Time to fade in over, defaulting to 1000 milliseconds.
			///
			void fade_in(const ma_uint64 ms = 1000);

			///
			/// Fade sound out.
			///
			/// \param ms Time to take fading out, defaulting to 1000 milliseconds.
			///
			void fade_out(const ma_uint64 ms = 1000);

			///
			/// \brief Configure a callback that will be fired	when the sound reaches the end.
			///
			/// Note that the callback is fired from the audio thread which means you cannot be uninitializing sound from the callback.
			///
			/// \param callback Function to use when finishing sound.
			/// \param user_data Any userdata you want to pass to the function.
			///
			void on_finish(ma_sound_end_proc callback, void* user_data);

			///
			/// Set if sound should loop.
			///
			/// \param loop True to loop, false to disable looping.
			///
			void set_looping(const bool loop);

			///
			/// Control individual volume of this sound.
			///
			/// \param volume Scales linearly. 0 is muted. 1 is default. 1+ is gain.
			///
			void set_volume(const float volume);

			///
			/// \brief Sound pan.
			///
			/// Setting the pan to 0 will result in an unpanned sound. Setting it to -1 will shift everything to the left, whereas +1 will shift it to the
			/// right.
			///
			/// \param pan Clamped between -1 to 1.
			///
			void set_pan(const float pan);

			///
			/// Set the mode to pan the sound with.
			///
			/// \param mode Stereo panner for movement.
			///
			void set_pan_mode(ma_pan_mode mode);

			///
			/// \brief Sound pitch.
			///
			/// Clamped to 0.1f.
			///
			/// \param pitch The pitch must be greater than 0.
			///
			void set_pitch(const float pitch);

			///
			/// Should sound always be spatialized relative to a specific listener.
			///
			/// \param id Listener index / id.
			///
			void set_pinned_listener(const unsigned int id);

			///
			/// Position relative to a listener.
			///
			/// \param positioning Position relative to listener.
			///
			void set_positioning(ma_positioning positioning);

			///
			/// \brief Set a listeners position.
			///
			/// Coordinate system is where positive X points right, positive Y points up and negative Z points forward.
			///
			/// \param x X Coord.
			/// \param y Y Coord.
			/// \param z Z Coord.
			///
			void set_position(const float x, const float y, const float z);

			///
			/// \brief Set a listeners direction.
			///
			/// Direction is a forward vector.
			///
			/// \param x Forward x.
			/// \param y Forward y.
			/// \param z Forward z.
			///
			void set_direction(const float x, const float y, const float z);

			///
			/// \brief Directional attenuation.
			///
			/// When a sound is between the inner and outer cones, it will be attenuated between 1 and the cone's outer gain.
			/// When a sound is inside the inner code, no directional attenuation is applied. When the sound is in between the inner and outer cones, the
			/// attenuation will be interpolated between 1 and the outer gain.
			///
			/// \param inner_angle In radians.
			/// \param outer_angle In radians.
			/// \param outer_gain Value attenuation will be set to when the sound is outside of the outer cone.
			///
			void set_cone(const float inner_angle, const float outer_angle, const float outer_gain);

			///
			/// The velocity of a sound is used for doppler effect.
			///
			/// \param x X velocity.
			/// \param y Y velocity.
			/// \param z Z velocity.
			///
			void set_velocity(const float x, const float y, const float z);

			///
			/// Set sound attenuation.
			///
			/// \param model Attenuation model.
			///
			void set_attenuation_model(ma_attenuation_model model);

			///
			/// Enable or disable spacialization.
			///
			/// \param enable True to enable, false to disable.
			///
			void set_spatialization_enabled(const bool enable);

			///
			/// Set sound rolloff.
			///
			/// \param rolloff How quickly a sound rolls off as it moves away from the listener.
			///
			void set_rolloff(const float rolloff);

			///
			/// Set min gain.
			///
			/// \param gain Min gain to apply from spacialization.
			///
			void set_min_gain(const float gain);

			///
			/// Set max gain.
			///
			/// \param gain Max gain to apply from spacialization.
			///
			void set_max_gain(const float gain);

			///
			/// Set min distance.
			///
			/// \param distance Min distance for the attenuation calculation.
			///
			void set_min_distance(const float distance);

			///
			/// Set max distance.
			///
			/// \param distance Max distance for the attenuation calculation.
			///
			void set_max_distance(const float distance);

			///
			/// Set doppler effect.
			///
			/// \param factor Spacialization doppler factor.
			///
			void set_doppler_factor(const float factor);

			///
			/// Set the directional attenuation factor for the sound.
			///
			/// \param factor Used for spacialization. Set to 0 to disable.
			///
			void set_directional_attenuation_factor(const float factor);

			///
			/// Set the start time of the sound in milliseconds relative to the absolute global time.
			///
			/// \param ms The start time in milliseconds.
			///
			void set_start_time_in_milliseconds(const ma_uint64 ms);

			/// Set the stop time of the sound in milliseconds relative to the absolute global time.
			///
			/// \param ms The stop time in milliseconds.
			///
			void set_stop_time_in_milliseconds(const ma_uint64 ms);

			///
			/// Get the volume of the sound.
			///
			/// \return The volume of the sound.
			///
			[[nodiscard]] float get_volume() const;

			///
			/// Get the pan value of the sound.
			///
			/// \return The pan value of the sound.
			///
			[[nodiscard]] float get_pan() const;

			///
			/// Get the pan mode of the sound.
			///
			/// \return The pan mode of the sound.
			///
			[[nodiscard]] ma_pan_mode get_pan_mode() const;

			///
			/// Get the pitch of the sound.
			///
			/// \return The pitch of the sound.
			///
			[[nodiscard]] float get_pitch() const;

			///
			/// Get the position of the sound.
			///
			/// \return The position of the sound.
			///
			[[nodiscard]] ma_vec3f get_position() const;

			///
			/// Get the direction of the sound.
			///
			/// \return The direction of the sound.
			///
			[[nodiscard]] ma_vec3f get_direction() const;

			///
			/// Get the velocity of the sound.
			///
			/// \return The velocity of the sound.
			///
			[[nodiscard]] ma_vec3f get_velocity() const;

			///
			/// Get directional attenuation cone.
			///
			/// \param inner_angle Pointer to store the inner angle of the sound cone in radians.
			/// \param outer_angle Pointer to store the outer angle of the sound cone in radians.
			/// \param outer_gain Pointer to store the outer gain of the sound cone.
			///
			void get_cone(float* inner_angle, float* outer_angle, float* outer_gain) const;

			///
			/// Get the attenuation model of the sound.
			///
			/// \return The attenuation model of the sound.
			///
			[[nodiscard]] ma_attenuation_model get_attenuation_model() const;

			///
			/// Get the positioning of the sound.
			///
			/// \return The positioning of the sound.
			///
			[[nodiscard]] ma_positioning get_positioning() const;

			///
			/// Get the pinned listener index for the sound.
			///
			/// \return The pinned listener index for the sound.
			///
			[[nodiscard]] unsigned int get_pinned_listener_index() const;

			///
			/// Get the listener index for the sound.
			///
			/// \return The listener index for the sound.
			///
			[[nodiscard]] unsigned int get_listener_index() const;

			///
			/// Get the direction to the listener for the sound.
			///
			/// \return The direction to the listener for the sound.
			///
			[[nodiscard]] ma_vec3f get_direction_to_listener() const;

			///
			/// Get the rolloff of the sound.
			///
			/// \return The rolloff of the sound.
			///
			[[nodiscard]] float get_rolloff() const;

			///
			/// Get the minimum gain of the sound.
			///
			/// \return The minimum gain of the sound.
			///
			[[nodiscard]] float get_min_gain() const;

			///
			/// Get the maximum gain of the sound.
			///
			/// \return The maximum gain of the sound.
			///
			[[nodiscard]] float get_max_gain() const;

			///
			/// Get the minimum distance of the sound.
			///
			/// \return The minimum distance of the sound.
			///
			[[nodiscard]] float get_min_distance() const;

			///
			/// Get the Doppler factor of the sound.
			///
			/// \return The Doppler factor of the sound.
			///
			[[nodiscard]] float get_doppler_factor() const;

			///
			/// Get the maximum distance of the sound.
			///
			/// \return The maximum distance of the sound.
			///
			[[nodiscard]] float get_max_distance() const;

			///
			/// Get the directional attenuation factor of the sound.
			///
			/// \return The directional attenuation factor of the sound.
			///
			[[nodiscard]] float get_directional_attenuation_factor() const;

			///
			/// Get the current fade volume of the sound.
			///
			/// \return The current fade volume of the sound.
			///
			[[nodiscard]] float get_current_fade_volume() const;

			///
			/// Get the cursor position of the sound in seconds.
			///
			/// \return Float in seconds.
			///
			[[nodiscard]] float get_cursor_in_seconds();

			///
			/// Get the length of the sound in seconds.
			///
			/// \return Float in seconds.
			///
			[[nodiscard]] float get_length_in_seconds();

			///
			/// Does this sound have spatialization enabled.
			///
			/// \return True if spatialization is enabled.
			///
			[[nodiscard]] bool is_spatialization_enabled() const;

			///
			/// Check if sound is playing.
			///
			/// \return Bool.
			///
			[[nodiscard]] bool is_playing() const;

			///
			/// Check if sound is looping.
			///
			/// \return Bool.
			///
			[[nodiscard]] bool is_looping() const;

			///
			/// Check if sound has finished playing.
			///
			/// \return Bool.
			///
			[[nodiscard]] bool is_finished() const;

		private:
			///
			/// Miniaudio sound structure.
			///
			ma_sound m_sound;
		};
	} // namespace media
} // namespace galaxy

#endif
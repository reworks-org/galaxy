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
			/// Sound type.
			///
			enum class Type : int
			{
				///
				/// A sound effect. Is not streamed.
				///
				SFX = 0,

				///
				/// Music. Is streamed.
				///
				MUSIC = 1,

				///
				/// Dialogue. Is streamed.
				///
				DIALOGUE = 2
			};

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
			Sound(Sound::Type type, std::string_view file);

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
			void load(Sound::Type type, std::string_view file);

			///
			/// Enable or disable spacialization.
			///
			/// \param enable True to enable, false to disable.
			///
			void toggle_spatialization(const bool enable);

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
			/// \brief Sound pan.
			///
			/// Setting the pan to 0 will result in an unpanned sound. Setting it to -1 will shift everything to the left, whereas +1 will shift it to the
			/// right.
			///
			/// \param pan Clamped between -1 to 1.
			///
			void set_pan(const float pan);

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
			/// The listener can have a cone the controls how sound is attenuated based on the listener's direction.
			/// When a sound is between the inner and outer cones, it will be attenuated between 1 and the cone's outer gain.
			/// When a sound is inside the inner code, no directional attenuation is applied. When the sound is in between the inner and outer cones, the
			/// attenuation will be interpolated between 1 and the outer gain.
			///
			/// \param inner_angle In radians.
			/// \param outer_angle In radians.
			/// \param outer_gain Value attenuation will be set to when the sound is outside of the outer cone.
			///
			void set_cone(float inner_angle, const float outer_angle, const float outer_gain);

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
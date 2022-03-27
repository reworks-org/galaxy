///
/// Sound.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_SOUND_HPP_
#define GALAXY_AUDIO_SOUND_HPP_

#include "galaxy/audio/SourceManipulator.hpp"
#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace audio
	{
		///
		/// \brief Short length ogg-vorbis audio.
		///
		/// Contains a Buffer and Source.
		///
		class Sound final : public Buffer, public SourceManipulator, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Sound() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Sound(const nlohmann::json& json);

			///
			/// Destructor.
			///
			virtual ~Sound();

			///
			/// \brief Play sound.
			///
			/// Plays music from beginning or pause point.
			///
			void play() noexcept override;

			///
			/// Pause sound.
			///
			void pause() noexcept override;

			///
			/// \brief Stop sound.
			///
			/// Starts again from beginning.
			///
			void stop() noexcept override;

			///
			/// Load a file from disk.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			/// \return False if load failed.
			///
			[[maybe_unused]] bool load(std::string_view file);

			///
			/// \brief Should the sound repeat upon reaching the end.
			///
			/// \param looping True to repeat.
			///
			void set_looping(const bool looping) noexcept override;

			///
			/// Get audio looping state.
			///
			/// \return Const bool.
			///
			[[nodiscard]] bool get_looping() noexcept override;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Move constructor.
			///
			Sound(Sound&&) = delete;

			///
			/// Move assignment operator.
			///
			Sound& operator=(Sound&&) = delete;

			///
			/// Copy constructor.
			///
			Sound(const Sound&) = delete;

			///
			/// Copy assignment operator.
			///
			Sound& operator=(const Sound&) = delete;
		};
	} // namespace audio
} // namespace galaxy

#endif
///
/// Music.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_MUSIC_HPP_
#define GALAXY_AUDIO_MUSIC_HPP_

#include <thread>

#include <nlohmann/json_fwd.hpp>

#include "galaxy/audio/SourceManipulator.hpp"
#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace audio
	{
		///
		/// \brief Streamed audio source.
		///
		class Music final : public BufferStream, public SourceManipulator, public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Music() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Music(const nlohmann::json& json);

			///
			/// Destructor.
			///
			virtual ~Music();

			///
			/// \brief Play music.
			///
			/// Plays music from beginning or pause point.
			///
			void play() override;

			///
			/// Pause music.
			///
			void pause() override;

			///
			/// \brief Stop music.
			///
			/// Starts again from beginning.
			///
			void stop() override;

			///
			/// Load a file to stream from disk.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			/// \return False if load failed.
			///
			[[maybe_unused]] const bool load(std::string_view file);

			///
			/// \brief Should the music repeat upon reaching the end.
			///
			/// \param looping True to repeat.
			///
			void set_looping(const bool looping) override;

			///
			/// Get audio looping state.
			///
			/// \return Const bool.
			///
			[[nodiscard]] const bool get_looping() override;

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
			Music(Music&&) = delete;

			///
			/// Move assignment operator.
			///
			Music& operator=(Music&&) = delete;

			///
			/// Copy constructor.
			///
			Music(const Music&) = delete;

			///
			/// Copy assignment operator.
			///
			Music& operator=(const Music&) = delete;

			///
			/// \brief Update stream buffers as it plays.
			///
			/// Internal use only.
			///
			void update();

			///
			/// Destroy music data.
			///
			void destroy();

		private:
			///
			/// Looping flag.
			///
			bool m_looping;

			///
			/// Thread to process music updates on.
			///
			std::jthread m_thread;

			///
			/// Thread control.
			///
			std::atomic_bool m_running;
		};
	} // namespace audio
} // namespace galaxy

#endif
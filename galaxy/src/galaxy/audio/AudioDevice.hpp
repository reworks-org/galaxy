///
/// AudioDevice.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_DEVICE_HPP_
#define GALAXY_AUDIO_DEVICE_HPP_

namespace galaxy
{
	namespace audio
	{
		namespace device
		{
			///
			/// Initialize audio device and context.
			///
			void init() noexcept;

			///
			/// Close the audio device and context.
			///
			void close() noexcept;

			///
			/// Check if audio device has been initialized successfully.
			///
			/// \return True if the audio device is ready, false otherwise.
			///
			[[nodiscard]]
			bool is_audio_device_ready() noexcept;

			///
			/// Set master volume (listener).
			///
			/// \param volume The master volume to set.
			///
			void set_master_volume(const float volume) noexcept;

			///
			/// Get master volume (listener).
			///
			/// \return The current master volume.
			///
			[[nodiscard]]
			float get_master_volume() noexcept;

			///
			/// Attach audio stream processor to stream, receives the samples as 'float'.
			///
			/// \param stream The audio stream.
			/// \param processor The audio callback processor.
			///
			void attach_audio_stream_processor(const AudioStream& stream, const AudioCallback& processor) noexcept;

			///
			/// Detach audio stream processor from stream.
			///
			/// \param stream The audio stream.
			/// \param processor The audio callback processor.
			///
			void detach_audio_stream_processor(const AudioStream& stream, const AudioCallback& processor) noexcept;

			///
			/// Attach audio stream processor to the entire audio pipeline, receives the samples as 'float'.
			///
			/// \param processor The audio callback processor.
			///
			void attach_audio_mixed_processor(const AudioCallback& processor) noexcept;

			///
			/// Detach audio stream processor from the entire audio pipeline.
			///
			/// \param processor The audio callback processor.
			///
			void detach_audio_mixed_processor(const AudioCallback& processor) noexcept;
		} // namespace device
	} // namespace audio
} // namespace galaxy

#endif

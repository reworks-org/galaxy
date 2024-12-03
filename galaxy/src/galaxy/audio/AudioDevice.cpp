///
/// AudioDevice.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <raylib.hpp>

#include "AudioDevice.hpp"

namespace galaxy
{
	namespace audio
	{
		namespace device
		{
			void init() noexcept
			{
				::InitAudioDevice();
			}

			void close() noexcept
			{
				::CloseAudioDevice();
			}

			bool is_audio_device_ready() noexcept
			{
				return ::IsAudioDeviceReady();
			}

			void set_master_volume(const float volume) noexcept
			{
				::SetMasterVolume(volume);
			}

			float get_master_volume() noexcept
			{
				return ::GetMasterVolume();
			}

			void attach_audio_stream_processor(const AudioStream& stream, const AudioCallback& processor) noexcept
			{
				::AttachAudioStreamProcessor(stream, processor);
			}

			void detach_audio_stream_processor(const AudioStream& stream, const AudioCallback& processor) noexcept
			{
				::DetachAudioStreamProcessor(stream, processor);
			}

			void attach_audio_mixed_processor(const AudioCallback& processor) noexcept
			{
				::AttachAudioMixedProcessor(processor);
			}

			void detach_audio_mixed_processor(const AudioCallback& processor) noexcept
			{
				::DetachAudioMixedProcessor(processor);
			}
		} // namespace device
	} // namespace audio
} // namespace galaxy

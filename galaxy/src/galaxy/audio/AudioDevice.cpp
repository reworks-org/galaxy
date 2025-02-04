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
				rl::InitAudioDevice();
			}

			void close() noexcept
			{
				rl::CloseAudioDevice();
			}

			bool is_audio_device_ready() noexcept
			{
				return rl::IsAudioDeviceReady();
			}

			void set_master_volume(const float volume) noexcept
			{
				rl::SetMasterVolume(volume);
			}

			float get_master_volume() noexcept
			{
				return rl::GetMasterVolume();
			}

			void attach_audio_stream_processor(const rl::AudioStream& stream, const rl::AudioCallback& processor) noexcept
			{
				rl::AttachAudioStreamProcessor(stream, processor);
			}

			void detach_audio_stream_processor(const rl::AudioStream& stream, const rl::AudioCallback& processor) noexcept
			{
				rl::DetachAudioStreamProcessor(stream, processor);
			}

			void attach_audio_mixed_processor(const rl::AudioCallback& processor) noexcept
			{
				rl::AttachAudioMixedProcessor(processor);
			}

			void detach_audio_mixed_processor(const rl::AudioCallback& processor) noexcept
			{
				rl::DetachAudioMixedProcessor(processor);
			}
		} // namespace device
	} // namespace audio
} // namespace galaxy

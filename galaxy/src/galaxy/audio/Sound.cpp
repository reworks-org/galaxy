///
/// Sound.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>

#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Sound.hpp"

namespace galaxy
{
	namespace audio
	{
		Sound::Sound() noexcept
		{
			stream.buffer     = nullptr;
			stream.processor  = nullptr;
			stream.channels   = 0u;
			stream.sampleRate = 0u;
			stream.sampleSize = 0u;
			frameCount        = 0u;
		}

		Sound::Sound(Sound&& s) noexcept
		{
			this->unload();

			this->stream.buffer     = s.stream.buffer;
			this->stream.processor  = s.stream.processor;
			this->stream.channels   = s.stream.channels;
			this->stream.sampleRate = s.stream.sampleRate;
			this->stream.sampleSize = s.stream.sampleSize;
			this->frameCount        = s.frameCount;

			s.stream.buffer    = nullptr;
			s.stream.processor = nullptr;
		}

		Sound& Sound::operator=(Sound&& s) noexcept
		{
			if (this != &s)
			{
				this->unload();

				this->stream.buffer     = s.stream.buffer;
				this->stream.processor  = s.stream.processor;
				this->stream.channels   = s.stream.channels;
				this->stream.sampleRate = s.stream.sampleRate;
				this->stream.sampleSize = s.stream.sampleSize;
				this->frameCount        = s.frameCount;

				s.stream.buffer    = nullptr;
				s.stream.processor = nullptr;
			}

			return *this;
		}

		Sound::~Sound() noexcept
		{
			unload();
		}

		void Sound::load(const std::string& file_name)
		{
			auto& fs = entt::locator<fs::VirtualFileSystem>::value();

			const auto ext  = fs.get_file_extension(file_name);
			const auto data = fs.read_binary(file_name);

			const auto wave = ::LoadWaveFromMemory(ext.c_str(), data.data(), data.size());
			const auto s    = ::LoadSoundFromWave(wave);
			::UnloadWave(wave);

			stream.buffer     = s.stream.buffer;
			stream.processor  = s.stream.processor;
			stream.channels   = s.stream.channels;
			stream.sampleRate = s.stream.sampleRate;
			stream.sampleSize = s.stream.sampleSize;
			frameCount        = s.frameCount;
		}

		void Sound::unload() noexcept
		{
			if (valid())
			{
				::UnloadSound(*this);

				stream.buffer    = nullptr;
				stream.processor = nullptr;
			}
		}

		bool Sound::valid() const noexcept
		{
			return ::IsSoundValid(*this);
		}

		void Sound::update(const void* data, const int sample_count) noexcept
		{
			::UpdateSound(*this, data, sample_count);
		}

		void Sound::play() noexcept
		{
			::PlaySound(*this);
		}

		void Sound::stop() noexcept
		{
			::StopSound(*this);
		}

		void Sound::pause() noexcept
		{
			::PauseSound(*this);
		}

		void Sound::resume() noexcept
		{
			::ResumeSound(*this);
		}

		bool Sound::is_playing() const noexcept
		{
			return ::IsSoundPlaying(*this);
		}

		void Sound::set_volume(const float volume) noexcept
		{
			::SetSoundVolume(*this, volume);
		}

		void Sound::set_pitch(const float pitch) noexcept
		{
			::SetSoundPitch(*this, pitch);
		}

		void Sound::set_pan(const float pan) noexcept
		{
			::SetSoundPan(*this, pan);
		}
	} // namespace audio
} // namespace galaxy

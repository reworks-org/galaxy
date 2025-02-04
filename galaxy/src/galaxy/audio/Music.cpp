///
/// Music.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>

#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Music.hpp"

namespace galaxy
{
	namespace audio
	{
		Music::Music() noexcept
		{
			stream.buffer     = nullptr;
			stream.processor  = nullptr;
			stream.channels   = 0u;
			stream.sampleRate = 0u;
			stream.sampleSize = 0u;
			frameCount        = 0u;
			looping           = false;
			ctxType           = 0;
			ctxData           = nullptr;
		}

		Music::Music(Music&& s) noexcept
		{
			this->unload();

			this->stream.buffer     = s.stream.buffer;
			this->stream.processor  = s.stream.processor;
			this->stream.channels   = s.stream.channels;
			this->stream.sampleRate = s.stream.sampleRate;
			this->stream.sampleSize = s.stream.sampleSize;
			this->frameCount        = s.frameCount;
			this->looping           = s.looping;
			this->ctxType           = s.ctxType;
			this->ctxData           = s.ctxData;

			s.stream.buffer    = nullptr;
			s.stream.processor = nullptr;
			s.ctxData          = nullptr;
		}

		Music& Music::operator=(Music&& s) noexcept
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
				this->looping           = s.looping;
				this->ctxType           = s.ctxType;
				this->ctxData           = s.ctxData;

				s.stream.buffer    = nullptr;
				s.stream.processor = nullptr;
				s.ctxData          = nullptr;
			}

			return *this;
		}

		Music::~Music() noexcept
		{
			unload();
		}

		void Music::load(const std::string& file_name)
		{
			auto& fs = entt::locator<fs::VirtualFileSystem>::value();

			const auto ext  = fs.get_file_extension(file_name);
			const auto data = fs.read_binary(file_name);

			const auto s = ::LoadMusicStreamFromMemory(ext.c_str(), data.data(), data.size());

			stream.buffer     = s.stream.buffer;
			stream.processor  = s.stream.processor;
			stream.channels   = s.stream.channels;
			stream.sampleRate = s.stream.sampleRate;
			stream.sampleSize = s.stream.sampleSize;
			frameCount        = s.frameCount;
			looping           = s.looping;
			ctxType           = s.ctxType;
			ctxData           = s.ctxData;
		}

		void Music::unload() noexcept
		{
			if (valid())
			{
				::UnloadMusicStream(*this);

				stream.buffer    = nullptr;
				stream.processor = nullptr;
				ctxData          = nullptr;
			}
		}

		bool Music::valid() const noexcept
		{
			return ::IsMusicValid(*this);
		}

		void Music::update() noexcept
		{
			::UpdateMusicStream(*this);
		}

		void Music::play() noexcept
		{
			::PlayMusicStream(*this);
		}

		void Music::stop() noexcept
		{
			::StopMusicStream(*this);
		}

		void Music::pause() noexcept
		{
			::PauseMusicStream(*this);
		}

		void Music::resume() noexcept
		{
			::ResumeMusicStream(*this);
		}

		bool Music::is_playing() const noexcept
		{
			return ::IsMusicStreamPlaying(*this);
		}

		void Music::seek(const float position) noexcept
		{
			::SeekMusicStream(*this, position);
		}

		void Music::set_volume(const float volume) noexcept
		{
			::SetMusicVolume(*this, volume);
		}

		void Music::set_pitch(const float pitch) noexcept
		{
			::SetMusicPitch(*this, pitch);
		}

		void Music::set_pan(const float pan) noexcept
		{
			::SetMusicPan(*this, pan);
		}

		float Music::get_time_length() noexcept
		{
			return ::GetMusicTimeLength(*this);
		}

		float Music::get_time_played() noexcept
		{
			return ::GetMusicTimePlayed(*this);
		}
	} // namespace audio
} // namespace galaxy

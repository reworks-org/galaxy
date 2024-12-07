///
/// Video.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Video.hpp"

namespace galaxy
{
	namespace graphics
	{
		Video::Video() noexcept
		{
			ctx = nullptr;

			videoTexture.format  = 0;
			videoTexture.height  = 0;
			videoTexture.id      = 0u;
			videoTexture.mipmaps = 1;
			videoTexture.width   = 0;

			audioStream.buffer     = nullptr;
			audioStream.processor  = nullptr;
			audioStream.channels   = 0u;
			audioStream.sampleRate = 0u;
			audioStream.sampleSize = 0u;
		}

		Video::Video(Video&& v) noexcept
		{
			unload();

			this->ctx = v.ctx;

			this->videoTexture.format  = v.videoTexture.format;
			this->videoTexture.height  = v.videoTexture.height;
			this->videoTexture.id      = v.videoTexture.id;
			this->videoTexture.mipmaps = v.videoTexture.mipmaps;
			this->videoTexture.width   = v.videoTexture.width;

			this->audioStream.buffer     = v.audioStream.buffer;
			this->audioStream.processor  = v.audioStream.processor;
			this->audioStream.channels   = v.audioStream.channels;
			this->audioStream.sampleRate = v.audioStream.sampleRate;
			this->audioStream.sampleSize = v.audioStream.sampleSize;

			v.ctx                   = nullptr;
			v.audioStream.buffer    = nullptr;
			v.audioStream.processor = nullptr;
		}

		Video& Video::operator=(Video&& v) noexcept
		{
			if (this != &v)
			{
				this->ctx = v.ctx;

				this->videoTexture.format  = v.videoTexture.format;
				this->videoTexture.height  = v.videoTexture.height;
				this->videoTexture.id      = v.videoTexture.id;
				this->videoTexture.mipmaps = v.videoTexture.mipmaps;
				this->videoTexture.width   = v.videoTexture.width;

				this->audioStream.buffer     = v.audioStream.buffer;
				this->audioStream.processor  = v.audioStream.processor;
				this->audioStream.channels   = v.audioStream.channels;
				this->audioStream.sampleRate = v.audioStream.sampleRate;
				this->audioStream.sampleSize = v.audioStream.sampleSize;

				v.ctx                   = nullptr;
				v.audioStream.buffer    = nullptr;
				v.audioStream.processor = nullptr;
			}

			return *this;
		}

		Video::~Video() noexcept
		{
			unload();
		}

		void Video::load(const std::string& file_name) noexcept
		{
			load_ex(file_name, MEDIA_LOAD_AV);
		}

		void Video::load_ex(const std::string& file_name, const ray::MediaLoadFlag flags) noexcept
		{
			const auto v = ::LoadMediaEx(file_name.c_str(), flags);

			ctx = v.ctx;

			videoTexture.format  = v.videoTexture.format;
			videoTexture.height  = v.videoTexture.height;
			videoTexture.id      = v.videoTexture.id;
			videoTexture.mipmaps = v.videoTexture.mipmaps;
			videoTexture.width   = v.videoTexture.width;

			audioStream.buffer     = v.audioStream.buffer;
			audioStream.processor  = v.audioStream.processor;
			audioStream.channels   = v.audioStream.channels;
			audioStream.sampleRate = v.audioStream.sampleRate;
			audioStream.sampleSize = v.audioStream.sampleSize;
		}

		bool Video::valid() const noexcept
		{
			return ::IsMediaValid(*this);
		}

		ray::MediaProperties Video::get_properties() const noexcept
		{
			return ::GetMediaProperties(*this);
		}

		bool Video::update() noexcept
		{
			return ::UpdateMedia(this);
		}

		bool Video::update_ex(const double delta_time) noexcept
		{
			return ::UpdateMediaEx(this, delta_time);
		}

		ray::MediaState Video::get_state() const noexcept
		{
			return static_cast<ray::MediaState>(::GetMediaState(*this));
		}

		double Video::get_position() const noexcept
		{
			return ::GetMediaPosition(*this);
		}

		bool Video::set_position(const double time_sec) noexcept
		{
			return ::SetMediaPosition(*this, time_sec);
		}

		bool Video::set_looping(const bool loop_play) noexcept
		{
			return ::SetMediaLooping(*this, loop_play);
		}

		int Video::set_flag(const ray::MediaConfigFlag flag, const int value) noexcept
		{
			return ::SetMediaFlag(static_cast<int>(flag), value);
		}

		int Video::get_flag(const ray::MediaConfigFlag flag) const noexcept
		{
			return ::GetMediaFlag(static_cast<int>(flag));
		}

		void Video::unload() noexcept
		{
			if (valid())
			{
				::UnloadMedia(this);
				ctx = nullptr;
			}
		}
	} // namespace graphics
} // namespace galaxy

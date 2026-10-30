#include "video.h"
#include "draw.h"
#include "error.h"

#define PL_MPEG_IMPLEMENTATION
#include <pl_mpeg.h>

#include <SDL3/SDL.h>

#include <algorithm>

namespace sl
{
	struct Video
	{
		plm_t*           plm          = nullptr;
		Bitmap*          bitmap       = nullptr;
		SDL_AudioStream* audio_stream = nullptr; // SDL3 uses streams instead of raw device IDs
		float            volume       = 1.0f;
		bool             wants_audio  = false;
		bool             audio_ready  = false;
	};

	namespace
	{
		void video_frame_callback(plm_t*, plm_frame_t* frame, void* user)
		{
			Video*    video  = static_cast<Video*>(user);
			Bitmap*   bitmap = video->bitmap;
			const int stride = bitmap->width * 4;

			plm_frame_to_rgba(frame, bitmap->pixels.data(), stride);
			bitmap->ram_dirty = true;
			upload_bitmap(bitmap);
		}

		void video_audio_callback(plm_t*, plm_samples_t* samples, void* user)
		{
			Video* video = static_cast<Video*>(user);

			if (!video->audio_stream)
				return;

			constexpr int sample_count = PLM_AUDIO_SAMPLES_PER_FRAME * 2;

			if (video->volume >= 0.999f)
			{
				SDL_PutAudioStreamData(video->audio_stream, samples->interleaved, sample_count * sizeof(float));
				return;
			}
                
			float scaled[sample_count];
			for (int i = 0; i < sample_count; ++i)
				scaled[i] = samples->interleaved[i] * video->volume;

			SDL_PutAudioStreamData(video->audio_stream, scaled, sizeof(scaled));
		}

		// Opening the SDL audio device is deferred until the first update_video() call
		// (rather than done eagerly in open_video()) because set_gfx_mode() unconditionally
		// calls SDL_Quit() during its own setup, which would otherwise invalidate an audio
		// device opened beforehand (a common ordering need: open the video first to size the
		// window from its dimensions, then set_gfx_mode(), then start the update loop).
		void ensure_audio_ready(Video* video)
		{
			if (video->audio_ready || !video->wants_audio)
				return;

			video->audio_ready = true;

			SDL_AudioSpec want {};
			want.freq     = plm_get_samplerate(video->plm);
			want.format   = SDL_AUDIO_F32; // SDL3 replaces AUDIO_F32SYS with SDL_AUDIO_F32
			want.channels = 2;
			// want.samples was removed from SDL_AudioSpec in SDL3

			// Open the default playback device directly into a managed stream
			video->audio_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want, nullptr, nullptr);

			if (video->audio_stream)
			{
				// SDL3 manages buffers internally. We reuse 4096.0 from the old want.samples here.
				plm_set_audio_lead_time(video->plm, 4096.0 / want.freq);
				plm_set_audio_decode_callback(video->plm, video_audio_callback, video);

				// Devices open in a paused state. Get the device bound to the stream and resume it.
				SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(video->audio_stream));
			}
		}

        Video *create_video(plm_t *plm, bool audio)
        {
            if (!plm)
                return nullptr;

            // The MPEG-PS system header's declared stream counts are unreliable on many
            // real-world files; probing actual packets is how pl_mpeg's own examples find
            // streams reliably (without this, num_video/audio_streams can read as 0 and no
            // decoder ever gets created, silently decoding nothing).
            plm_probe(plm, 5000 * 1024);

            if (!plm_has_headers(plm))
            {
                plm_destroy(plm);
                return nullptr;
            }

            Video *video = new Video();
            video->plm = plm;
            video->bitmap = create_bitmap(plm_get_width(plm), plm_get_height(plm));

            // plm_frame_to_rgba() never touches the alpha byte, so without this every
            // decoded frame renders as fully transparent over a zero-initialised bitmap.
            if (video->bitmap)
            {
                for (std::size_t i = 3; i < video->bitmap->pixels.size(); i += 4)
                    video->bitmap->pixels[i] = 255;
            }

            plm_set_video_decode_callback(plm, video_frame_callback, video);

            video->wants_audio = audio && plm_get_num_audio_streams(plm) > 0;
            if (!video->wants_audio)
                plm_set_audio_enabled(plm, 0);

            return video;
        }
    } // namespace

    const char *video_backend_name()
    {
        return "pl_mpeg";
    }

    Video *open_video(const std::string &path, bool audio)
    {
        if (path.find("://") != std::string::npos)
        {
            detail::set_error("URL playback requires the FFmpeg video backend (SIMLIB_VIDEO=ffmpeg)");
            return nullptr;
        }

        Video *video = create_video(plm_create_with_filename(path.c_str()), audio);
        if (!video)
            detail::set_error("Unable to open MPEG1 video: " + path);
        return video;
    }

    Video *open_video_from_memory(const std::uint8_t *data, std::size_t size, bool audio)
    {
        Video *video = create_video(plm_create_with_memory(const_cast<std::uint8_t *>(data), static_cast<std::size_t>(size), 0), audio);
        if (!video)
            detail::set_error("Unable to open MPEG1 video from memory");
        return video;
    }

    void destroy_video(Video* video)
	{
		if (!video)
			return;

		if (video->audio_stream)
			SDL_DestroyAudioStream(video->audio_stream);

		if (video->bitmap)
			destroy_bitmap(video->bitmap);

		if (video->plm)
			plm_destroy(video->plm);

		delete video;
	}

    void update_video(Video *video, double dt_seconds)
    {
        if (!video || !video->plm)
            return;

        ensure_audio_ready(video);
        plm_decode(video->plm, dt_seconds);
    }

    Bitmap *video_bitmap(Video *video)
    {
        return video ? video->bitmap : nullptr;
    }

    int video_width(Video *video)
    {
        return video ? plm_get_width(video->plm) : 0;
    }

    int video_height(Video *video)
    {
        return video ? plm_get_height(video->plm) : 0;
    }

    double video_duration(Video *video)
    {
        return video ? plm_get_duration(video->plm) : 0.0;
    }

    double video_position(Video *video)
    {
        return video ? plm_get_time(video->plm) : 0.0;
    }

    bool seek_video(Video *video, double seconds)
    {
        if (!video)
            return false;

        const double duration = plm_get_duration(video->plm);
        seconds = std::max(0.0, duration > 0.0 ? std::min(seconds, duration) : seconds);
        return plm_seek(video->plm, seconds, 1) != 0;
    }

    bool video_has_audio(Video *video)
    {
        return video && video->wants_audio;
    }

    bool video_has_ended(Video *video)
    {
        return video ? plm_has_ended(video->plm) != 0 : true;
    }

    void set_video_loop(Video *video, bool loop)
    {
        if (video)
            plm_set_loop(video->plm, loop ? 1 : 0);
    }

    void set_video_volume(Video *video, float volume)
    {
        if (video)
            video->volume = std::clamp(volume, 0.0f, 1.0f);
    }

} // namespace sl

#include "video.h"
#include "draw.h"
#include "error.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/error.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

#include <SDL2/SDL.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <deque>
#include <vector>

namespace sl
{
    namespace
    {
        constexpr double audio_lead_seconds = 0.25;
        constexpr int io_timeout_seconds = 10;
        constexpr int max_packets_per_update = 4096;

        struct QueuedFrame
        {
            AVFrame *frame = nullptr;
            double pts = 0.0;
        };

        struct MemorySource
        {
            const std::uint8_t *data = nullptr;
            std::size_t size = 0;
            std::size_t position = 0;
        };

        std::string av_error_text(int code)
        {
            char buffer[AV_ERROR_MAX_STRING_SIZE]{};
            av_strerror(code, buffer, sizeof(buffer));
            return buffer;
        }

        int read_memory(void *opaque, std::uint8_t *buffer, int size)
        {
            MemorySource *source = static_cast<MemorySource *>(opaque);
            const std::size_t remaining = source->size - source->position;
            if (remaining == 0)
                return AVERROR_EOF;

            const std::size_t count = std::min<std::size_t>(remaining, static_cast<std::size_t>(size));
            std::memcpy(buffer, source->data + source->position, count);
            source->position += count;
            return static_cast<int>(count);
        }

        std::int64_t seek_memory(void *opaque, std::int64_t offset, int whence)
        {
            MemorySource *source = static_cast<MemorySource *>(opaque);

            if (whence & AVSEEK_SIZE)
                return static_cast<std::int64_t>(source->size);

            std::int64_t base = 0;
            switch (whence & ~AVSEEK_FORCE)
            {
            case SEEK_SET:
                base = 0;
                break;
            case SEEK_CUR:
                base = static_cast<std::int64_t>(source->position);
                break;
            case SEEK_END:
                base = static_cast<std::int64_t>(source->size);
                break;
            default:
                return -1;
            }

            const std::int64_t target = base + offset;
            if (target < 0 || target > static_cast<std::int64_t>(source->size))
                return -1;

            source->position = static_cast<std::size_t>(target);
            return target;
        }

        using SteadyClock = std::chrono::steady_clock;
    } // namespace

    struct Video
    {
        AVFormatContext *format = nullptr;
        AVIOContext *io = nullptr;
        MemorySource memory;
        AVCodecContext *video_codec = nullptr;
        AVCodecContext *audio_codec = nullptr;
        SwsContext *sws = nullptr;
        SwrContext *swr = nullptr;
        AVPacket *packet = nullptr;
        int video_stream = -1;
        int audio_stream = -1;
        double start_offset = 0.0;
        double duration = 0.0;
        bool seekable = false;

        Bitmap *bitmap = nullptr;
        std::deque<QueuedFrame> queue;
        AVFrame *ready = nullptr;
        double last_video_pts = 0.0;
        double frame_duration = 1.0 / 30.0;

        SDL_AudioDeviceID audio_device = 0;
        float volume = 1.0f;
        bool wants_audio = false;
        bool audio_ready = false;
        double audio_end = 0.0;
        std::vector<float> audio_buffer;

        double clock = 0.0;
        bool demux_eof = false;
        bool video_done = false;
        bool ended = false;
        bool loop = false;

        SteadyClock::time_point io_deadline = SteadyClock::time_point::max();
    };

    namespace
    {
        int interrupt_callback(void *opaque)
        {
            const Video *video = static_cast<const Video *>(opaque);
            return SteadyClock::now() > video->io_deadline ? 1 : 0;
        }

        void arm_io_deadline(Video *video)
        {
            video->io_deadline = SteadyClock::now() + std::chrono::seconds(io_timeout_seconds);
        }

        void disarm_io_deadline(Video *video)
        {
            video->io_deadline = SteadyClock::time_point::max();
        }

        double stream_seconds(const Video *video, int stream_index, std::int64_t pts)
        {
            const AVStream *stream = video->format->streams[stream_index];
            return static_cast<double>(pts) * av_q2d(stream->time_base) - video->start_offset;
        }

        void clear_video_queue(Video *video)
        {
            for (QueuedFrame &queued : video->queue)
                av_frame_free(&queued.frame);
            video->queue.clear();
            av_frame_free(&video->ready);
        }

        void destroy_ffmpeg_video(Video *video)
        {
            if (!video)
                return;

            if (video->audio_device)
                SDL_CloseAudioDevice(video->audio_device);

            clear_video_queue(video);
            sws_freeContext(video->sws);
            swr_free(&video->swr);
            avcodec_free_context(&video->video_codec);
            avcodec_free_context(&video->audio_codec);
            av_packet_free(&video->packet);
            avformat_close_input(&video->format);

            if (video->io)
            {
                av_freep(&video->io->buffer);
                avio_context_free(&video->io);
            }

            if (video->bitmap)
                destroy_bitmap(video->bitmap);

            delete video;
        }

        AVCodecContext *open_decoder(const AVStream *stream, bool threaded)
        {
            const AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
            if (!codec)
                return nullptr;

            AVCodecContext *context = avcodec_alloc_context3(codec);
            if (!context)
                return nullptr;

            if (avcodec_parameters_to_context(context, stream->codecpar) < 0)
            {
                avcodec_free_context(&context);
                return nullptr;
            }

            if (threaded)
                context->thread_count = 0;

            if (avcodec_open2(context, codec, nullptr) < 0)
            {
                avcodec_free_context(&context);
                return nullptr;
            }

            return context;
        }

        bool open_audio_converter(Video *video)
        {
            AVChannelLayout out_layout{};
            av_channel_layout_default(&out_layout, 2);

            AVChannelLayout in_layout{};
            if (av_channel_layout_copy(&in_layout, &video->audio_codec->ch_layout) < 0)
                return false;
            if (in_layout.order == AV_CHANNEL_ORDER_UNSPEC)
                av_channel_layout_default(&in_layout, std::max(1, in_layout.nb_channels));

            const int result = swr_alloc_set_opts2(&video->swr, &out_layout, AV_SAMPLE_FMT_FLT,
                                                   video->audio_codec->sample_rate, &in_layout,
                                                   video->audio_codec->sample_fmt,
                                                   video->audio_codec->sample_rate, 0, nullptr);
            av_channel_layout_uninit(&in_layout);
            av_channel_layout_uninit(&out_layout);

            if (result < 0 || swr_init(video->swr) < 0)
            {
                swr_free(&video->swr);
                return false;
            }
            return true;
        }

        // Opening the SDL audio device is deferred until the first update_video() call because
        // set_gfx_mode() calls SDL_Quit() during setup, which would invalidate a device opened
        // beforehand (open the video first to size the window, then set_gfx_mode(), then update).
        void ensure_audio_ready(Video *video)
        {
            if (video->audio_ready || !video->wants_audio)
                return;

            video->audio_ready = true;

            SDL_AudioSpec want{};
            want.freq = video->audio_codec->sample_rate;
            want.format = AUDIO_F32SYS;
            want.channels = 2;
            want.samples = 4096;

            video->audio_device = SDL_OpenAudioDevice(nullptr, 0, &want, nullptr, 0);
            if (video->audio_device)
                SDL_PauseAudioDevice(video->audio_device, 0);
        }

        void queue_audio_frame(Video *video, const AVFrame *frame)
        {
            const double frame_start = frame->best_effort_timestamp != AV_NOPTS_VALUE
                                           ? stream_seconds(video, video->audio_stream, frame->best_effort_timestamp)
                                           : video->audio_end;
            const double frame_end = frame_start + static_cast<double>(frame->nb_samples) / frame->sample_rate;
            video->audio_end = std::max(video->audio_end, frame_end);

            // After a seek the decoder may emit audio that ends before the new position.
            if (!video->audio_device || !video->swr || frame_end <= video->clock)
                return;

            const int capacity = swr_get_out_samples(video->swr, frame->nb_samples);
            if (capacity <= 0)
                return;

            video->audio_buffer.resize(static_cast<std::size_t>(capacity) * 2);
            std::uint8_t *out = reinterpret_cast<std::uint8_t *>(video->audio_buffer.data());
            const int converted = swr_convert(video->swr, &out, capacity,
                                              const_cast<const std::uint8_t **>(frame->extended_data),
                                              frame->nb_samples);
            if (converted <= 0)
                return;

            const std::size_t sample_count = static_cast<std::size_t>(converted) * 2;
            if (video->volume < 0.999f)
            {
                for (std::size_t i = 0; i < sample_count; ++i)
                    video->audio_buffer[i] *= video->volume;
            }

            SDL_QueueAudio(video->audio_device, video->audio_buffer.data(),
                           static_cast<Uint32>(sample_count * sizeof(float)));
        }

        // Pass nullptr to flush the decoder at end of stream.
        void decode_audio_packet(Video *video, const AVPacket *packet)
        {
            if (avcodec_send_packet(video->audio_codec, packet) < 0)
                return;

            AVFrame *frame = av_frame_alloc();
            while (avcodec_receive_frame(video->audio_codec, frame) >= 0)
                queue_audio_frame(video, frame);
            av_frame_free(&frame);
        }

        // Pass nullptr to flush the decoder at end of stream.
        void decode_video_packet(Video *video, const AVPacket *packet)
        {
            if (avcodec_send_packet(video->video_codec, packet) < 0 && packet)
                return;

            while (true)
            {
                AVFrame *frame = av_frame_alloc();
                const int result = avcodec_receive_frame(video->video_codec, frame);
                if (result < 0)
                {
                    av_frame_free(&frame);
                    if (result == AVERROR_EOF)
                        video->video_done = true;
                    return;
                }

                double pts = frame->best_effort_timestamp != AV_NOPTS_VALUE
                                 ? stream_seconds(video, video->video_stream, frame->best_effort_timestamp)
                                 : video->last_video_pts + video->frame_duration;
                video->last_video_pts = pts;
                video->queue.push_back({frame, pts});
            }
        }

        void hit_end_of_stream(Video *video)
        {
            video->demux_eof = true;
            decode_video_packet(video, nullptr);
            if (video->audio_codec)
                decode_audio_packet(video, nullptr);
        }

        // Moves frames that are due into `ready`, leaving only future frames queued.
        bool take_due_frames(Video *video)
        {
            bool got = false;
            while (!video->queue.empty() && video->queue.front().pts <= video->clock)
            {
                av_frame_free(&video->ready);
                video->ready = video->queue.front().frame;
                video->queue.pop_front();
                got = true;
            }
            return got;
        }

        void present_ready_frame(Video *video)
        {
            if (!video->ready || !video->bitmap)
                return;

            const AVFrame *frame = video->ready;
            video->sws = sws_getCachedContext(video->sws, frame->width, frame->height,
                                              static_cast<AVPixelFormat>(frame->format),
                                              video->bitmap->width, video->bitmap->height, AV_PIX_FMT_RGBA,
                                              SWS_BILINEAR, nullptr, nullptr, nullptr);
            if (!video->sws)
                return;

            std::uint8_t *dst[4] = {video->bitmap->pixels.data(), nullptr, nullptr, nullptr};
            const int dst_stride[4] = {video->bitmap->width * 4, 0, 0, 0};
            sws_scale(video->sws, frame->data, frame->linesize, 0, frame->height, dst, dst_stride);

            video->bitmap->ram_dirty = true;
            upload_bitmap(video->bitmap);
            av_frame_free(&video->ready);
        }

        bool seek_to(Video *video, double seconds)
        {
            if (!video->seekable)
                return false;

            if (video->duration > 0.0)
                seconds = std::min(seconds, video->duration);
            seconds = std::max(0.0, seconds);

            const std::int64_t target = static_cast<std::int64_t>((seconds + video->start_offset) * AV_TIME_BASE);
            arm_io_deadline(video);
            const int result = av_seek_frame(video->format, -1, target, AVSEEK_FLAG_BACKWARD);
            disarm_io_deadline(video);
            if (result < 0)
                return false;

            avcodec_flush_buffers(video->video_codec);
            if (video->audio_codec)
                avcodec_flush_buffers(video->audio_codec);
            if (video->swr)
            {
                // Discard samples buffered inside the resampler.
                std::vector<float> scratch(static_cast<std::size_t>(swr_get_out_samples(video->swr, 0)) * 2 + 16);
                std::uint8_t *out = reinterpret_cast<std::uint8_t *>(scratch.data());
                swr_convert(video->swr, &out, static_cast<int>(scratch.size() / 2), nullptr, 0);
            }
            if (video->audio_device)
                SDL_ClearQueuedAudio(video->audio_device);

            clear_video_queue(video);
            video->clock = seconds;
            video->audio_end = seconds;
            video->last_video_pts = seconds;
            video->demux_eof = false;
            video->video_done = false;
            video->ended = false;
            return true;
        }

        void pump(Video *video)
        {
            for (int packets = 0; packets < max_packets_per_update; ++packets)
            {
                take_due_frames(video);

                const bool need_video = video->queue.empty() && !video->video_done;
                const bool need_audio = video->audio_codec && video->audio_device &&
                                        video->audio_end < video->clock + audio_lead_seconds && !video->demux_eof;
                if ((!need_video && !need_audio) || video->demux_eof)
                    break;

                arm_io_deadline(video);
                const int result = av_read_frame(video->format, video->packet);
                disarm_io_deadline(video);

                if (result < 0)
                {
                    hit_end_of_stream(video);
                    take_due_frames(video);
                    break;
                }

                if (video->packet->stream_index == video->video_stream)
                    decode_video_packet(video, video->packet);
                else if (video->packet->stream_index == video->audio_stream && video->audio_device)
                    decode_audio_packet(video, video->packet);

                av_packet_unref(video->packet);
            }
        }

        Video *finish_open(Video *video, bool audio, const std::string &what)
        {
            const int find_result = avformat_find_stream_info(video->format, nullptr);
            if (find_result < 0)
            {
                detail::set_error("Unable to read stream info for " + what + ": " + av_error_text(find_result));
                destroy_ffmpeg_video(video);
                return nullptr;
            }

            video->video_stream = av_find_best_stream(video->format, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
            if (video->video_stream < 0)
            {
                detail::set_error("No video stream found in " + what);
                destroy_ffmpeg_video(video);
                return nullptr;
            }

            video->video_codec = open_decoder(video->format->streams[video->video_stream], true);
            if (!video->video_codec || video->video_codec->width <= 0 || video->video_codec->height <= 0)
            {
                detail::set_error("Unable to open a video decoder for " + what);
                destroy_ffmpeg_video(video);
                return nullptr;
            }

            if (audio)
            {
                video->audio_stream = av_find_best_stream(video->format, AVMEDIA_TYPE_AUDIO, -1, video->video_stream,
                                                          nullptr, 0);
                if (video->audio_stream >= 0)
                {
                    video->audio_codec = open_decoder(video->format->streams[video->audio_stream], false);
                    if (video->audio_codec && open_audio_converter(video))
                        video->wants_audio = true;
                    else
                        avcodec_free_context(&video->audio_codec);
                }
            }

            const AVRational rate = av_guess_frame_rate(video->format, video->format->streams[video->video_stream],
                                                        nullptr);
            if (rate.num > 0 && rate.den > 0)
                video->frame_duration = 1.0 / av_q2d(rate);

            video->start_offset = video->format->start_time != AV_NOPTS_VALUE
                                      ? static_cast<double>(video->format->start_time) / AV_TIME_BASE
                                      : 0.0;
            video->duration = video->format->duration != AV_NOPTS_VALUE
                                  ? static_cast<double>(video->format->duration) / AV_TIME_BASE
                                  : 0.0;
            video->seekable = video->format->pb && (video->format->pb->seekable & AVIO_SEEKABLE_NORMAL) != 0;

            video->bitmap = create_bitmap(video->video_codec->width, video->video_codec->height);
            if (!video->bitmap)
            {
                detail::set_error("Unable to allocate the video bitmap for " + what);
                destroy_ffmpeg_video(video);
                return nullptr;
            }

            // sws_scale to RGBA writes opaque alpha, but start fully opaque so the first
            // presented frame never shows a transparent bitmap.
            for (std::size_t i = 3; i < video->bitmap->pixels.size(); i += 4)
                video->bitmap->pixels[i] = 255;

            video->packet = av_packet_alloc();
            return video;
        }
    } // namespace

    const char *video_backend_name()
    {
        return "FFmpeg";
    }

    Video *open_video(const std::string &path, bool audio)
    {
        Video *video = new Video();
        video->format = avformat_alloc_context();
        if (!video->format)
        {
            delete video;
            detail::set_error("Unable to allocate an FFmpeg format context");
            return nullptr;
        }

        video->format->interrupt_callback.callback = interrupt_callback;
        video->format->interrupt_callback.opaque = video;
        arm_io_deadline(video);

        AVDictionary *options = nullptr;
        av_dict_set(&options, "reconnect", "1", 0);
        av_dict_set(&options, "reconnect_streamed", "1", 0);

        AVFormatContext *format = video->format;
        const int result = avformat_open_input(&format, path.c_str(), nullptr, &options);
        av_dict_free(&options);
        disarm_io_deadline(video);
        video->format = format;

        if (result < 0)
        {
            detail::set_error("Unable to open " + path + ": " + av_error_text(result));
            video->format = nullptr;
            delete video;
            return nullptr;
        }

        arm_io_deadline(video);
        Video *opened = finish_open(video, audio, path);
        if (opened)
            disarm_io_deadline(opened);
        return opened;
    }

    Video *open_video_from_memory(const std::uint8_t *data, std::size_t size, bool audio)
    {
        if (!data || size == 0)
        {
            detail::set_error("Cannot open a video from an empty buffer");
            return nullptr;
        }

        Video *video = new Video();
        video->memory = {data, size, 0};

        constexpr int io_buffer_size = 32768;
        unsigned char *buffer = static_cast<unsigned char *>(av_malloc(io_buffer_size));
        video->io = buffer ? avio_alloc_context(buffer, io_buffer_size, 0, &video->memory, read_memory, nullptr,
                                                 seek_memory)
                           : nullptr;
        video->format = video->io ? avformat_alloc_context() : nullptr;
        if (!video->format)
        {
            if (video->io)
            {
                av_freep(&video->io->buffer);
                avio_context_free(&video->io);
            }
            else
            {
                av_free(buffer);
            }
            delete video;
            detail::set_error("Unable to allocate FFmpeg memory input");
            return nullptr;
        }

        video->format->pb = video->io;
        video->format->flags |= AVFMT_FLAG_CUSTOM_IO;

        AVFormatContext *format = video->format;
        const int result = avformat_open_input(&format, nullptr, nullptr, nullptr);
        video->format = format;
        if (result < 0)
        {
            detail::set_error("Unable to open video from memory: " + av_error_text(result));
            video->format = nullptr;
            av_freep(&video->io->buffer);
            avio_context_free(&video->io);
            delete video;
            return nullptr;
        }

        return finish_open(video, audio, "memory buffer");
    }

    void destroy_video(Video *video)
    {
        destroy_ffmpeg_video(video);
    }

    void update_video(Video *video, double dt_seconds)
    {
        if (!video || video->ended)
            return;

        ensure_audio_ready(video);
        video->clock += std::max(0.0, dt_seconds);
        pump(video);
        take_due_frames(video);
        present_ready_frame(video);

        if (video->video_done && video->queue.empty())
        {
            if (video->loop && seek_to(video, 0.0))
                return;
            video->ended = true;
        }
    }

    Bitmap *video_bitmap(Video *video)
    {
        return video ? video->bitmap : nullptr;
    }

    int video_width(Video *video)
    {
        return video ? video->video_codec->width : 0;
    }

    int video_height(Video *video)
    {
        return video ? video->video_codec->height : 0;
    }

    double video_duration(Video *video)
    {
        return video ? video->duration : 0.0;
    }

    double video_position(Video *video)
    {
        return video ? video->clock : 0.0;
    }

    bool seek_video(Video *video, double seconds)
    {
        return video && seek_to(video, seconds);
    }

    bool video_has_audio(Video *video)
    {
        return video && video->wants_audio;
    }

    bool video_has_ended(Video *video)
    {
        return video ? (video->ended && !video->loop) : true;
    }

    void set_video_loop(Video *video, bool loop)
    {
        if (video)
            video->loop = loop;
    }

    void set_video_volume(Video *video, float volume)
    {
        if (video)
            video->volume = std::clamp(volume, 0.0f, 1.0f);
    }

} // namespace sl

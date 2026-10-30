#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace sl
{
    struct Bitmap;

    /**
     * Opaque handle to an open video (with optional audio) decoding into a bitmap.
     * Supported formats depend on the build-time SIMLIB_VIDEO option: "plmpeg" decodes MPEG1/MP2
     * program streams only, "ffmpeg" decodes anything FFmpeg supports, "none" disables video.
     */
    struct Video;

    /** Name of the compiled-in video backend: "pl_mpeg", "FFmpeg" or "none". */
    const char *video_backend_name();

    /**
     * Open a video file for playback. Pass audio=false to skip audio decoding/output.
     * With the FFmpeg backend path may also be a URL (http, https, rtsp, ...); other backends reject URLs.
     * Returns nullptr and sets last_error() on failure.
     */
    Video *open_video(const std::string &path, bool audio = true);
    /** Open a video from a memory buffer; the caller retains ownership of data and must keep it alive. */
    Video *open_video_from_memory(const std::uint8_t *data, std::size_t size, bool audio = true);
    /** Destroy a video and release its decode buffers and audio device. */
    void destroy_video(Video *video);

    /** Advance decoding by elapsed seconds; updates video_bitmap() and queues audio as needed. */
    void update_video(Video *video, double dt_seconds);

    /** The RGBA8 bitmap that receives decoded frames, sized to the video's display resolution. */
    Bitmap *video_bitmap(Video *video);

    /** Display width/height of the video stream. */
    int video_width(Video *video);
    int video_height(Video *video);
    /** Duration of the video in seconds, or 0 if unknown. */
    double video_duration(Video *video);
    /** Current playback position in seconds. */
    double video_position(Video *video);
    /** Seek to a position in seconds. Returns false if the source is not seekable (e.g. a live stream). */
    bool seek_video(Video *video, double seconds);
    /** Whether the video has an audio track that will be played. */
    bool video_has_audio(Video *video);
    /** Whether playback has reached the end. Always false while looping is enabled. */
    bool video_has_ended(Video *video);
    /** Enable or disable looping playback. Default false. */
    void set_video_loop(Video *video, bool loop);
    /** Set playback volume in the 0-1 range. No effect if the video has no audio track. */
    void set_video_volume(Video *video, float volume);

} // namespace sl

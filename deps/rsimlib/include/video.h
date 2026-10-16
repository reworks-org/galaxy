#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace sl
{
    struct Bitmap;

    /** Opaque handle to an open MPEG1 video (with optional MP2 audio) decoding into a bitmap. */
    struct Video;

    /** Open an MPEG-PS (.mpg) file for playback. Pass audio=false to skip audio decoding/output. */
    Video *open_video(const std::string &path, bool audio = true);
    /** Open an MPEG-PS video from a memory buffer; the caller retains ownership of data. */
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
    /** Whether playback has reached the end. Always false while looping is enabled. */
    bool video_has_ended(Video *video);
    /** Enable or disable looping playback. Default false. */
    void set_video_loop(Video *video, bool loop);
    /** Set playback volume in the 0-1 range. No effect if the video has no audio track. */
    void set_video_volume(Video *video, float volume);

} // namespace sl

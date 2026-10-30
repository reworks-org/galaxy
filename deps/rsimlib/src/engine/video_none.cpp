#include "video.h"
#include "error.h"

namespace sl
{
    const char *video_backend_name()
    {
        return "none";
    }

    Video *open_video(const std::string &, bool)
    {
        detail::set_error("Video support is not compiled in (SIMLIB_VIDEO=none)");
        return nullptr;
    }

    Video *open_video_from_memory(const std::uint8_t *, std::size_t, bool)
    {
        detail::set_error("Video support is not compiled in (SIMLIB_VIDEO=none)");
        return nullptr;
    }

    void destroy_video(Video *) {}
    void update_video(Video *, double) {}
    Bitmap *video_bitmap(Video *) { return nullptr; }
    int video_width(Video *) { return 0; }
    int video_height(Video *) { return 0; }
    double video_duration(Video *) { return 0.0; }
    double video_position(Video *) { return 0.0; }
    bool seek_video(Video *, double) { return false; }
    bool video_has_audio(Video *) { return false; }
    bool video_has_ended(Video *) { return true; }
    void set_video_loop(Video *, bool) {}
    void set_video_volume(Video *, float) {}
} // namespace sl

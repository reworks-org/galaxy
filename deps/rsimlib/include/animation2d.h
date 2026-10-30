#pragma once

#include "atlas.h"
#include "draw.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace sl
{
    /** One frame of a sprite sheet: a source rectangle in the sheet bitmap and how long it is shown. */
    struct SpriteFrame
    {
        int x = 0, y = 0, width = 0, height = 0;
        /** Display time in seconds. */
        float duration = 0.1f;
        /** Name of the frame in the source file, if any. */
        std::string name;
    };

    /** Order in which a clip steps through its frames. */
    enum class PlayDirection
    {
        forward,
        reverse,
        ping_pong
    };

    /** A named run of frames (inclusive range into SpriteSheet::frames) with playback settings. */
    struct AnimationClip
    {
        std::string name;
        int first_frame = 0;
        int last_frame = 0;
        PlayDirection direction = PlayDirection::forward;
        bool loop = true;
        /** Playback speed multiplier. */
        float speed = 1.0f;
        /** Events fired when the animation enters a sheet frame index. */
        std::vector<std::pair<int, std::string>> events;
    };

    /** Frames and named clips for one sprite sheet bitmap. The sheet does not own the bitmap. */
    struct SpriteSheet
    {
        Bitmap *bitmap = nullptr;
        std::vector<SpriteFrame> frames;
        std::map<std::string, AnimationClip> clips;

        /** Add a clip over frames [first, last]; returns false when the range is invalid. */
        bool add_clip(const std::string &name, int first, int last, bool loop = true,
                      PlayDirection direction = PlayDirection::forward, float speed = 1.0f)
        {
            if (first < 0 || last < first || last >= static_cast<int>(frames.size()))
                return false;
            AnimationClip clip;
            clip.name = name;
            clip.first_frame = first;
            clip.last_frame = last;
            clip.loop = loop;
            clip.direction = direction;
            clip.speed = speed;
            clips[name] = clip;
            return true;
        }
        /** Attach an event to a clip; it fires whenever playback enters that sheet frame. */
        bool add_event(const std::string &clip, int frame, const std::string &event)
        {
            const auto found = clips.find(clip);
            if (found == clips.end())
                return false;
            found->second.events.emplace_back(frame, event);
            return true;
        }
        const AnimationClip *find_clip(const std::string &name) const
        {
            const auto found = clips.find(name);
            return found == clips.end() ? nullptr : &found->second;
        }
    };

    /** Build a sheet from an Atlas grid, one frame per tile, all with the same duration (seconds). */
    inline SpriteSheet sprite_sheet_from_atlas(const Atlas &atlas, float frame_duration = 0.1f)
    {
        SpriteSheet sheet;
        sheet.bitmap = atlas.bitmap;
        for (int tile = 0; tile < atlas.tile_count; ++tile)
        {
            SpriteFrame frame;
            if (!atlas_tile_rect(atlas, tile, frame.x, frame.y, frame.width, frame.height))
                continue;
            frame.duration = frame_duration;
            sheet.frames.push_back(frame);
        }
        return sheet;
    }

    /**
     * Parse an Aseprite or TexturePacker JSON export into frames and clips. Both the array and the hash form of
     * "frames" are accepted. Aseprite "frameTags" become clips (forward, reverse and pingpong directions); the
     * bitmap is left for the caller to set. Returns false and fills error when the JSON is unusable.
     */
    inline bool parse_sprite_sheet_json(const std::string &text, SpriteSheet &sheet, std::string *error = nullptr)
    {
        auto fail = [&](const char *message)
        {
            if (error)
                *error = message;
            return false;
        };
        nlohmann::ordered_json document = nlohmann::ordered_json::parse(text, nullptr, false);
        if (document.is_discarded() || !document.is_object() || !document.contains("frames"))
            return fail("Not a sprite sheet JSON file: no \"frames\" member");

        sheet.frames.clear();
        sheet.clips.clear();
        auto read_frame = [&](const nlohmann::ordered_json &entry, const std::string &name) -> bool
        {
            const auto rect = entry.find("frame");
            if (rect == entry.end() || !rect->is_object())
                return false;
            SpriteFrame frame;
            frame.x = rect->value("x", 0);
            frame.y = rect->value("y", 0);
            frame.width = rect->value("w", 0);
            frame.height = rect->value("h", 0);
            frame.duration = entry.value("duration", 100) / 1000.0f;
            frame.name = entry.contains("filename") && entry["filename"].is_string() ? entry["filename"].get<std::string>() : name;
            sheet.frames.push_back(frame);
            return true;
        };
        const nlohmann::ordered_json &frames = document["frames"];
        if (frames.is_array())
        {
            for (const auto &entry : frames)
                if (!read_frame(entry, std::string()))
                    return fail("A frame is missing its \"frame\" rectangle");
        }
        else if (frames.is_object())
        {
            // ordered_json keeps file order, which is the frame order in Aseprite's hash form.
            for (auto it = frames.begin(); it != frames.end(); ++it)
                if (!read_frame(it.value(), it.key()))
                    return fail("A frame is missing its \"frame\" rectangle");
        }
        else
            return fail("\"frames\" must be an array or an object");

        if (document.contains("meta") && document["meta"].contains("frameTags"))
        {
            for (const auto &tag : document["meta"]["frameTags"])
            {
                const int from = tag.value("from", 0);
                const int to = tag.value("to", 0);
                const std::string direction = tag.value("direction", "forward");
                const PlayDirection play = direction == "reverse"    ? PlayDirection::reverse
                                           : direction == "pingpong" ? PlayDirection::ping_pong
                                                                     : PlayDirection::forward;
                sheet.add_clip(tag.value("name", std::string()), from, to, true, play);
            }
        }
        return true;
    }

    /** Read an Aseprite/TexturePacker JSON file; see parse_sprite_sheet_json(). */
    inline bool load_sprite_sheet_json(const std::string &path, SpriteSheet &sheet, std::string *error = nullptr)
    {
        std::ifstream file(path);
        if (!file)
        {
            if (error)
                *error = "Unable to open " + path;
            return false;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return parse_sprite_sheet_json(buffer.str(), sheet, error);
    }

    /** Plays the clips of a SpriteSheet. Call update() each frame with the elapsed seconds, then draw(). */
    class Animator
    {
    public:
        explicit Animator(const SpriteSheet *sheet = nullptr) : sheet_(sheet) {}

        void set_sheet(const SpriteSheet *sheet)
        {
            sheet_ = sheet;
            clip_ = nullptr;
        }

        /** Called with the event name when playback enters a frame that has an event. */
        std::function<void(const std::string &)> on_event;
        /** Called once when a non-looping clip reaches its last frame's end. */
        std::function<void(const std::string &)> on_finished;

        /** Start a clip. Does nothing when it is already playing unless restart is true. Returns false for unknown clips. */
        bool play(const std::string &name, bool restart = false)
        {
            const AnimationClip *clip = sheet_ ? sheet_->find_clip(name) : nullptr;
            if (!clip)
                return false;
            if (clip == clip_ && !restart && !finished_)
                return true;
            clip_ = clip;
            build_sequence();
            position_ = 0;
            elapsed_ = 0.0f;
            finished_ = false;
            paused_ = false;
            fire_events();
            return true;
        }

        void pause() { paused_ = true; }
        void resume() { paused_ = false; }
        bool paused() const { return paused_; }
        /** True when a non-looping clip has finished (it holds its last frame). */
        bool finished() const { return finished_; }
        /** Name of the current clip, or an empty string. */
        const std::string &clip_name() const
        {
            static const std::string none;
            return clip_ ? clip_->name : none;
        }
        /** Sheet frame index currently shown, or -1 when no clip is playing. */
        int frame_index() const { return sequence_.empty() ? -1 : sequence_[position_]; }
        const SpriteFrame *frame() const
        {
            const int index = frame_index();
            return index >= 0 && sheet_ && index < static_cast<int>(sheet_->frames.size()) ? &sheet_->frames[index] : nullptr;
        }
        /** Jump to the n-th step of the current clip's sequence. */
        void set_position(int step)
        {
            if (!sequence_.empty())
            {
                position_ = std::clamp(step, 0, static_cast<int>(sequence_.size()) - 1);
                elapsed_ = 0.0f;
            }
        }

        void update(float seconds)
        {
            if (!clip_ || sequence_.empty() || paused_ || finished_ || seconds <= 0.0f)
                return;
            elapsed_ += seconds * clip_->speed;
            // The cap guards against zero-length frames spinning forever.
            for (int guard = 0; guard < 1000; ++guard)
            {
                const SpriteFrame *current = frame();
                const float duration = current ? std::max(current->duration, 0.0001f) : 0.1f;
                if (elapsed_ < duration)
                    break;
                elapsed_ -= duration;
                if (position_ + 1 < static_cast<int>(sequence_.size()))
                    ++position_;
                else if (clip_->loop)
                    position_ = 0;
                else
                {
                    finished_ = true;
                    elapsed_ = 0.0f;
                    if (on_finished)
                        on_finished(clip_->name);
                    return;
                }
                fire_events();
            }
        }

        /** Fill params' source rectangle from the current frame and draw it. Position, scale, flips, etc. come from params. */
        void draw(Bitmap *bitmap, SpriteParams params) const
        {
            const SpriteFrame *current = frame();
            if (!current)
                return;
            params.source_x = current->x;
            params.source_y = current->y;
            params.source_width = current->width;
            params.source_height = current->height;
            draw_sprite_ex(bitmap ? bitmap : sheet_->bitmap, params);
        }

    private:
        void build_sequence()
        {
            sequence_.clear();
            const int first = clip_->first_frame;
            const int last = clip_->last_frame;
            if (clip_->direction == PlayDirection::reverse)
            {
                for (int i = last; i >= first; --i)
                    sequence_.push_back(i);
            }
            else
            {
                for (int i = first; i <= last; ++i)
                    sequence_.push_back(i);
                if (clip_->direction == PlayDirection::ping_pong)
                    for (int i = last - 1; i > first; --i)
                        sequence_.push_back(i);
            }
        }
        void fire_events()
        {
            if (!on_event || !clip_)
                return;
            const int index = frame_index();
            for (const auto &entry : clip_->events)
                if (entry.first == index)
                    on_event(entry.second);
        }

        const SpriteSheet *sheet_ = nullptr;
        const AnimationClip *clip_ = nullptr;
        std::vector<int> sequence_;
        int position_ = 0;
        float elapsed_ = 0.0f;
        bool paused_ = false;
        bool finished_ = false;
    };
}

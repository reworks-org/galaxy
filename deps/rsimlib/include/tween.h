#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

namespace sl
{
    /** Easing curves for tweens. "in" starts slow, "out" ends slow, "in_out" does both. */
    enum class Ease
    {
        linear,
        quad_in,
        quad_out,
        quad_in_out,
        cubic_in,
        cubic_out,
        cubic_in_out,
        sine_in,
        sine_out,
        sine_in_out,
        expo_in,
        expo_out,
        expo_in_out,
        back_in,
        back_out,
        back_in_out,
        elastic_out,
        bounce_out
    };

    /** Apply an easing curve to progress t in [0, 1]. The result is 0 at t = 0 and 1 at t = 1 (it may overshoot between). */
    inline float ease(Ease type, float t)
    {
        t = std::clamp(t, 0.0f, 1.0f);
        constexpr float pi = 3.14159265358979f;
        constexpr float back = 1.70158f;
        switch (type)
        {
        case Ease::linear:
            return t;
        case Ease::quad_in:
            return t * t;
        case Ease::quad_out:
            return 1.0f - (1.0f - t) * (1.0f - t);
        case Ease::quad_in_out:
            return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) * 0.5f;
        case Ease::cubic_in:
            return t * t * t;
        case Ease::cubic_out:
            return 1.0f - std::pow(1.0f - t, 3.0f);
        case Ease::cubic_in_out:
            return t < 0.5f ? 4.0f * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) * 0.5f;
        case Ease::sine_in:
            return 1.0f - std::cos(t * pi * 0.5f);
        case Ease::sine_out:
            return std::sin(t * pi * 0.5f);
        case Ease::sine_in_out:
            return -(std::cos(pi * t) - 1.0f) * 0.5f;
        case Ease::expo_in:
            return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
        case Ease::expo_out:
            return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
        case Ease::expo_in_out:
            if (t == 0.0f || t == 1.0f)
                return t;
            return t < 0.5f ? std::pow(2.0f, 20.0f * t - 10.0f) * 0.5f : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) * 0.5f;
        case Ease::back_in:
            return (back + 1.0f) * t * t * t - back * t * t;
        case Ease::back_out:
            return 1.0f + (back + 1.0f) * std::pow(t - 1.0f, 3.0f) + back * std::pow(t - 1.0f, 2.0f);
        case Ease::back_in_out:
        {
            const float c = back * 1.525f;
            return t < 0.5f ? (std::pow(2.0f * t, 2.0f) * ((c + 1.0f) * 2.0f * t - c)) * 0.5f
                            : (std::pow(2.0f * t - 2.0f, 2.0f) * ((c + 1.0f) * (t * 2.0f - 2.0f) + c) + 2.0f) * 0.5f;
        }
        case Ease::elastic_out:
            if (t == 0.0f || t == 1.0f)
                return t;
            return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * (2.0f * pi / 3.0f)) + 1.0f;
        case Ease::bounce_out:
        {
            const float n = 7.5625f, d = 2.75f;
            if (t < 1.0f / d)
                return n * t * t;
            if (t < 2.0f / d)
            {
                t -= 1.5f / d;
                return n * t * t + 0.75f;
            }
            if (t < 2.5f / d)
            {
                t -= 2.25f / d;
                return n * t * t + 0.9375f;
            }
            t -= 2.625f / d;
            return n * t * t + 0.984375f;
        }
        }
        return t;
    }

    /** Animates a value from one number to another over time. Call update() each frame and read value(). */
    struct Tween
    {
        float from = 0.0f;
        float to = 1.0f;
        /** Length in seconds. A zero or negative duration finishes immediately. */
        float duration = 1.0f;
        Ease easing = Ease::linear;
        /** Wait this many seconds before starting. */
        float delay = 0.0f;
        /** Run back and forth forever instead of finishing. */
        bool ping_pong = false;
        /** Restart from the beginning forever instead of finishing. */
        bool loop = false;

        /** Advance time. Returns true once the tween has finished (never for looping tweens). */
        bool update(float dt_seconds)
        {
            if (finished_)
                return true;
            elapsed_ += dt_seconds;
            if (elapsed_ >= delay + duration && duration > 0.0f && (loop || ping_pong))
            {
                const float local = elapsed_ - delay;
                elapsed_ = delay + std::fmod(local, ping_pong ? duration * 2.0f : duration);
            }
            else if (elapsed_ >= delay + duration)
            {
                finished_ = true;
            }
            return finished_;
        }

        /** Current value. */
        float value() const
        {
            if (duration <= 0.0f)
                return to;
            float progress = std::max(elapsed_ - delay, 0.0f) / duration;
            if (ping_pong && progress > 1.0f)
                progress = 2.0f - progress;
            if (finished_)
                progress = 1.0f;
            return from + (to - from) * ease(easing, progress);
        }

        bool finished() const { return finished_; }
        void reset()
        {
            elapsed_ = 0.0f;
            finished_ = false;
        }

    private:
        float elapsed_ = 0.0f;
        bool finished_ = false;
    };

    /** Runs callbacks after a delay, once or repeatedly. Call update() each frame. */
    class Scheduler
    {
    public:
        /** Call fn after delay seconds. Returns an id for cancel(). */
        int after(float delay_seconds, std::function<void()> fn)
        {
            entries_.push_back({next_id_, delay_seconds, 0.0f, std::move(fn)});
            return next_id_++;
        }
        /** Call fn every interval seconds until cancelled. Returns an id for cancel(). */
        int every(float interval_seconds, std::function<void()> fn)
        {
            entries_.push_back({next_id_, interval_seconds, std::max(interval_seconds, 0.0001f), std::move(fn)});
            return next_id_++;
        }
        /** Cancel a scheduled callback. Returns whether it was found. */
        bool cancel(int id)
        {
            const auto found = std::find_if(entries_.begin(), entries_.end(), [id](const Entry &e) { return e.id == id; });
            if (found == entries_.end())
                return false;
            found->cancelled = true;
            return true;
        }
        /** Advance time and run due callbacks. Callbacks may schedule or cancel others. */
        void update(float dt_seconds)
        {
            std::vector<Entry> due;
            for (Entry &entry : entries_)
            {
                if (entry.cancelled)
                    continue;
                entry.remaining -= dt_seconds;
                if (entry.remaining <= 0.0f)
                    due.push_back(entry);
            }
            for (Entry &entry : entries_)
            {
                if (!entry.cancelled && entry.remaining <= 0.0f)
                {
                    if (entry.interval > 0.0f)
                        entry.remaining += entry.interval;
                    else
                        entry.cancelled = true;
                }
            }
            entries_.erase(std::remove_if(entries_.begin(), entries_.end(), [](const Entry &e) { return e.cancelled; }), entries_.end());
            for (Entry &entry : due)
                entry.fn();
        }
        /** Number of callbacks still pending. */
        int pending() const
        {
            return static_cast<int>(std::count_if(entries_.begin(), entries_.end(), [](const Entry &e) { return !e.cancelled; }));
        }

    private:
        struct Entry
        {
            int id;
            float remaining;
            float interval;
            std::function<void()> fn;
            bool cancelled = false;
        };
        std::vector<Entry> entries_;
        int next_id_ = 1;
    };
} // namespace sl

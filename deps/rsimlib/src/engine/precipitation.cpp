/** @file
 * @brief Implements 2D and 3D precipitation, the lens overlay and the weather sound.
 */

#include "precipitation.h"

#include "audio_synth.h"
#include "display.h"

#include <algorithm>
#include <cmath>

namespace sl
{
    namespace
    {
        constexpr float pi = 3.14159265358979323846f;

        float lerpf(float a, float b, float t) { return a + (b - a) * t; }
        float clamp01(float v) { return std::clamp(v, 0.0f, 1.0f); }

        Uint8 lerp_channel(Uint8 a, Uint8 b, float t)
        {
            return static_cast<Uint8>(std::clamp(lerpf(static_cast<float>(a), static_cast<float>(b), t), 0.0f, 255.0f));
        }

        // Truncation would stall a slow transition one unit short of its target, so always move at least one step.
        Uint8 step_channel(Uint8 a, Uint8 b, float t)
        {
            if (a == b)
                return b;
            const float moved = lerpf(static_cast<float>(a), static_cast<float>(b), t);
            const int step = b > a ? std::max(1, static_cast<int>(moved) - a) : std::min(-1, static_cast<int>(std::ceil(moved)) - a);
            return static_cast<Uint8>(std::clamp(static_cast<int>(a) + step, std::min<int>(a, b), std::max<int>(a, b)));
        }

        Colour lerp_colour(Colour a, Colour b, float t)
        {
            return {lerp_channel(a.red, b.red, t), lerp_channel(a.green, b.green, t), lerp_channel(a.blue, b.blue, t),
                    lerp_channel(a.alpha, b.alpha, t)};
        }

        Colour step_colour(Colour a, Colour b, float t)
        {
            return {step_channel(a.red, b.red, t), step_channel(a.green, b.green, t), step_channel(a.blue, b.blue, t),
                    step_channel(a.alpha, b.alpha, t)};
        }

        Colour with_alpha(Colour c, float alpha)
        {
            c.alpha = static_cast<Uint8>(std::clamp(alpha, 0.0f, 255.0f));
            return c;
        }

        float uniform(std::mt19937 &rng, float low, float high)
        {
            return std::uniform_real_distribution<float>(low, high)(rng);
        }
    } // namespace

    float precipitation_level_intensity(PrecipitationLevel level)
    {
        switch (level)
        {
        case PrecipitationLevel::light:
            return 0.25f;
        case PrecipitationLevel::moderate:
            return 0.5f;
        case PrecipitationLevel::heavy:
            return 0.8f;
        case PrecipitationLevel::storm:
            return 1.0f;
        }
        return 0.5f;
    }

    PrecipitationConfig precipitation_preset(PrecipitationType type, PrecipitationLevel level)
    {
        PrecipitationConfig config;
        config.type = type;
        config.intensity = precipitation_level_intensity(level);
        const float i = config.intensity;
        switch (type)
        {
        case PrecipitationType::none:
            config.intensity = 0.0f;
            break;
        case PrecipitationType::rain:
            config.wind = {20.0f + 260.0f * i * i, 0.0f};
            config.size = 0.8f + 0.5f * i;
            break;
        case PrecipitationType::hail:
            config.wind = {10.0f + 120.0f * i, 0.0f};
            config.size = 0.9f + 0.4f * i;
            break;
        case PrecipitationType::snow:
            config.wind = {10.0f + 90.0f * i * i, 0.0f};
            config.size = 1.2f - 0.3f * i;
            break;
        }
        return config;
    }

    Colour precipitation_default_colour(PrecipitationType type)
    {
        switch (type)
        {
        case PrecipitationType::rain:
            return {196, 214, 240, 190};
        case PrecipitationType::hail:
            return {232, 240, 255, 235};
        case PrecipitationType::snow:
            return {255, 255, 255, 235};
        case PrecipitationType::none:
            break;
        }
        return {255, 255, 255, 0};
    }

    // ---- PrecipitationController -------------------------------------------------------------------------

    void PrecipitationController::set_immediately(const PrecipitationConfig &config)
    {
        current_ = target_ = config;
        if (current_.type == PrecipitationType::none)
            current_.intensity = target_.intensity = 0.0f;
    }

    void PrecipitationController::transition_to(const PrecipitationConfig &target, float seconds)
    {
        target_ = target;
        if (target_.type == PrecipitationType::none)
            target_.intensity = 0.0f;
        seconds_ = std::max(0.0f, seconds);
        if (seconds_ <= 0.0f)
            current_ = target_;
    }

    bool PrecipitationController::transitioning() const
    {
        return current_.type != target_.type || std::fabs(current_.intensity - target_.intensity) > 1e-4f ||
               glm::length(current_.wind - target_.wind) > 1e-3f || std::fabs(current_.size - target_.size) > 1e-3f ||
               std::fabs(current_.fall_speed - target_.fall_speed) > 1e-3f || current_.splashes != target_.splashes ||
               current_.colour.red != target_.colour.red || current_.colour.green != target_.colour.green ||
               current_.colour.blue != target_.colour.blue || current_.colour.alpha != target_.colour.alpha;
    }

    void PrecipitationController::update(float dt)
    {
        if (dt <= 0.0f || !transitioning())
            return;
        if (seconds_ <= 0.0f)
        {
            current_ = target_;
            return;
        }
        const float step = dt / seconds_;
        const float k = std::min(1.0f, dt * 3.0f / seconds_);

        if (current_.type != target_.type)
        {
            if (current_.type == PrecipitationType::none || current_.intensity <= 0.0f)
            {
                const float wind_x = current_.wind.x, wind_y = current_.wind.y;
                current_ = target_;
                current_.intensity = 0.0f;
                current_.wind = {wind_x, wind_y};
            }
            else
            {
                current_.intensity = std::max(0.0f, current_.intensity - step);
                current_.wind += (target_.wind - current_.wind) * k;
                return;
            }
        }

        const float delta = target_.intensity - current_.intensity;
        current_.intensity += std::clamp(delta, -step, step);
        current_.wind += (target_.wind - current_.wind) * k;
        current_.size = lerpf(current_.size, target_.size, k);
        current_.fall_speed = lerpf(current_.fall_speed, target_.fall_speed, k);
        current_.colour = step_colour(current_.colour, target_.colour, k);
        current_.splashes = target_.splashes;
        if (std::fabs(current_.size - target_.size) < 1e-3f)
            current_.size = target_.size;
        if (std::fabs(current_.fall_speed - target_.fall_speed) < 1e-3f)
            current_.fall_speed = target_.fall_speed;
        if (glm::length(current_.wind - target_.wind) < 1e-3f)
            current_.wind = target_.wind;
    }

    // ---- Precipitation2D ---------------------------------------------------------------------------------

    namespace
    {
        constexpr float snow_column = 4.0f;

        float fall_speed_2d(const PrecipitationConfig &c)
        {
            if (c.fall_speed > 0.0f)
                return c.fall_speed;
            switch (c.type)
            {
            case PrecipitationType::rain:
                return 900.0f;
            case PrecipitationType::hail:
                return 720.0f;
            case PrecipitationType::snow:
                return 70.0f;
            case PrecipitationType::none:
                break;
            }
            return 0.0f;
        }
    } // namespace

    Precipitation2D::Precipitation2D(int width, int height)
    {
        if (width > 0 && height > 0)
            set_area(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
    }

    void Precipitation2D::set_area(float x, float y, float width, float height)
    {
        area_x_ = x;
        area_y_ = y;
        area_w_ = std::max(1.0f, width);
        area_h_ = std::max(1.0f, height);
        area_set_ = true;
        snow_.assign(static_cast<std::size_t>(std::ceil(area_w_ / snow_column)) + 1, 0.0f);
    }

    void Precipitation2D::set_ground(float y)
    {
        ground_y_ = y;
        ground_set_ = true;
        ground_fn_ = nullptr;
    }

    void Precipitation2D::set_ground(std::function<float(float)> ground_y)
    {
        ground_fn_ = std::move(ground_y);
        ground_set_ = true;
    }

    void Precipitation2D::set_max_particles(int count) { max_particles_ = std::max(0, count); }

    void Precipitation2D::set_max_snow_depth(float pixels) { max_snow_depth_ = std::max(0.0f, pixels); }

    float Precipitation2D::random(float low, float high) { return uniform(rng_, low, high); }

    float Precipitation2D::ground_at(float x) const
    {
        if (ground_fn_)
            return ground_fn_(x);
        return ground_set_ ? ground_y_ : area_y_ + area_h_;
    }

    float Precipitation2D::snow_depth_at(float x) const
    {
        if (snow_.empty() || max_snow_depth_ <= 0.0f)
            return 0.0f;
        const int index = static_cast<int>((x - area_x_) / snow_column);
        if (index < 0 || index >= static_cast<int>(snow_.size()))
            return 0.0f;
        return snow_[static_cast<std::size_t>(index)];
    }

    void Precipitation2D::add_snow(float x, float amount)
    {
        if (max_snow_depth_ <= 0.0f || snow_.empty())
            return;
        const int index = static_cast<int>((x - area_x_) / snow_column);
        if (index < 0 || index >= static_cast<int>(snow_.size()))
            return;
        snow_[static_cast<std::size_t>(index)] += amount;
    }

    void Precipitation2D::spawn(Drop &drop, bool anywhere)
    {
        drop.x = random(area_x_, area_x_ + area_w_);
        drop.y = anywhere ? random(area_y_, area_y_ + area_h_) : area_y_ - random(2.0f, 60.0f);
        drop.depth = random(0.0f, 1.0f);
        drop.phase = random(0.0f, 2.0f * pi);
        drop.size = random(0.7f, 1.3f);
        drop.bounces = 0;
        drop.rest = random(0.0f, 1.0f);
        drop.vx = drop.vy = 0.0f;
    }

    void Precipitation2D::clear()
    {
        drops_.clear();
        splashes_.clear();
        std::fill(snow_.begin(), snow_.end(), 0.0f);
    }

    void Precipitation2D::scroll(float dx, float dy)
    {
        if (dx == 0.0f && dy == 0.0f)
            return;
        for (Drop &drop : drops_)
        {
            const float parallax = 0.3f + 0.7f * drop.depth;
            drop.x -= dx * parallax;
            drop.y -= dy * parallax;
            drop.x = area_x_ + std::fmod(std::fmod(drop.x - area_x_, area_w_) + area_w_, area_w_);
            drop.y = area_y_ + std::fmod(std::fmod(drop.y - area_y_, area_h_) + area_h_, area_h_);
        }
        for (Splash &s : splashes_)
        {
            s.x -= dx;
            s.y -= dy;
        }
        // Snow is stored in screen columns, so it moves with the ground.
        const int columns = static_cast<int>(std::lround(-dx / snow_column));
        if (columns != 0 && !snow_.empty())
        {
            const int n = static_cast<int>(snow_.size());
            std::vector<float> moved(snow_.size(), 0.0f);
            for (int i = 0; i < n; ++i)
            {
                const int to = i + columns;
                if (to >= 0 && to < n)
                    moved[static_cast<std::size_t>(to)] = snow_[static_cast<std::size_t>(i)];
            }
            snow_.swap(moved);
        }
    }

    void Precipitation2D::update(float dt)
    {
        if (dt <= 0.0f)
            return;
        if (!area_set_)
            set_area(0.0f, 0.0f, static_cast<float>(screen_width()), static_cast<float>(screen_height()));
        dt = std::min(dt, 0.1f);
        weather.update(dt);
        const PrecipitationConfig &cfg = weather.current();
        time_ += dt;

        if (cfg.type != active_type_)
        {
            drops_.clear();
            active_type_ = cfg.type;
        }

        const float area_scale = (area_w_ * area_h_) / (1280.0f * 720.0f);
        int target = 0;
        if (cfg.type != PrecipitationType::none && cfg.intensity > 0.0f)
            target = std::max(1, static_cast<int>(cfg.intensity * static_cast<float>(max_particles_) * area_scale));

        while (static_cast<int>(drops_.size()) < target)
        {
            Drop drop;
            spawn(drop, true);
            drops_.push_back(drop);
        }

        const float fall = fall_speed_2d(cfg);
        for (std::size_t i = 0; i < drops_.size();)
        {
            Drop &d = drops_[i];
            const float ds = 0.4f + 0.6f * d.depth;
            bool respawn = false;

            if (cfg.type == PrecipitationType::rain)
            {
                d.vx = cfg.wind.x * ds;
                d.vy = fall * (0.8f + 0.2f * d.rest) * ds + cfg.wind.y * ds;
            }
            else if (cfg.type == PrecipitationType::hail)
            {
                if (d.bounces == 0)
                {
                    d.vx = cfg.wind.x * ds * 0.5f;
                    d.vy = fall * (0.85f + 0.3f * d.rest) * ds;
                }
                else
                {
                    d.vy += 1800.0f * dt;
                }
            }
            else
            {
                d.vx = cfg.wind.x * ds + std::sin(time_ * 1.3f + d.phase) * 22.0f * ds;
                d.vy = fall * (0.7f + 0.6f * d.rest) * ds + cfg.wind.y * ds + std::cos(time_ * 0.9f + d.phase) * 8.0f * ds;
            }
            d.x += d.vx * dt;
            d.y += d.vy * dt;

            if (d.x < area_x_)
                d.x += area_w_;
            else if (d.x >= area_x_ + area_w_)
                d.x -= area_w_;

            const float land_y = ground_at(d.x) - (1.0f - d.depth) * 18.0f;
            if (d.y >= land_y)
            {
                if (cfg.type == PrecipitationType::rain)
                {
                    if (cfg.splashes && random(0.0f, 1.0f) < 0.1f && splashes_.size() < 300)
                        splashes_.push_back({d.x, land_y, 0.0f, 0.35f, false});
                    respawn = true;
                }
                else if (cfg.type == PrecipitationType::hail)
                {
                    if (cfg.splashes && d.bounces < 2)
                    {
                        if (d.bounces == 0 && splashes_.size() < 400)
                            splashes_.push_back({d.x, land_y, 0.0f, 0.25f, true});
                        ++d.bounces;
                        d.y = land_y - 1.0f;
                        d.vy = -std::fabs(d.vy) * 0.38f;
                        d.vx = (d.vx != 0.0f ? d.vx : cfg.wind.x * 0.5f) * 0.7f + random(-30.0f, 30.0f);
                    }
                    else
                        respawn = true;
                }
                else
                {
                    add_snow(d.x, 1.0f * d.size * cfg.size * (0.4f + 0.6f * d.depth));
                    respawn = true;
                }
            }
            else if (d.y > area_y_ + area_h_ + 40.0f)
                respawn = true;

            if (respawn)
            {
                if (static_cast<int>(drops_.size()) > target)
                {
                    drops_[i] = drops_.back();
                    drops_.pop_back();
                    continue;
                }
                spawn(d, false);
            }
            ++i;
        }

        for (std::size_t i = 0; i < splashes_.size();)
        {
            splashes_[i].age += dt;
            if (splashes_[i].age >= splashes_[i].life)
            {
                splashes_[i] = splashes_.back();
                splashes_.pop_back();
            }
            else
                ++i;
        }

        if (max_snow_depth_ > 0.0f && !snow_.empty())
        {
            const bool snowing = cfg.type == PrecipitationType::snow && cfg.intensity > 0.0f;
            const float melt = snowing ? 0.0f : (cfg.type == PrecipitationType::rain && cfg.intensity > 0.0f ? 5.0f : 1.0f) * dt;
            const std::size_t n = snow_.size();
            // Let steep heaps slump into their neighbours, then cap and melt.
            for (std::size_t i = 0; i < n; ++i)
            {
                const float left = i > 0 ? snow_[i - 1] : snow_[i];
                const float right = i + 1 < n ? snow_[i + 1] : snow_[i];
                const float target_depth = (left + right + snow_[i] * 2.0f) * 0.25f;
                snow_[i] += (target_depth - snow_[i]) * std::min(1.0f, dt * 6.0f);
                snow_[i] = std::clamp(snow_[i] - melt, 0.0f, max_snow_depth_);
            }
        }
    }

    void Precipitation2D::draw_snow_cover()
    {
        if (max_snow_depth_ <= 0.0f || snow_.empty())
            return;
        for (std::size_t i = 0; i < snow_.size(); ++i)
        {
            if (snow_[i] < 0.3f)
                continue;
            const float x = area_x_ + static_cast<float>(i) * snow_column;
            const float g = ground_at(x + snow_column * 0.5f);
            const float top = g - snow_[i];
            rectfill(screen, x, top, x + snow_column + 0.5f, g + 1.0f, {244, 247, 255, 245});
            rectfill(screen, x, top, x + snow_column + 0.5f, top + 1.5f, {255, 255, 255, 255});
        }
    }

    void Precipitation2D::draw()
    {
        const PrecipitationConfig &cfg = weather.current();
        Colour base = cfg.colour.alpha ? cfg.colour : precipitation_default_colour(cfg.type);
        for (const Drop &d : drops_)
        {
            const float shade = 0.35f + 0.65f * d.depth;
            const Colour c = with_alpha(base, base.alpha * shade);
            if (cfg.type == PrecipitationType::rain)
            {
                const float length = 0.03f * cfg.size;
                line(screen, d.x, d.y, d.x - d.vx * length, d.y - d.vy * length, c, (1.2f + 1.6f * d.depth) * d.size);
            }
            else if (cfg.type == PrecipitationType::hail)
            {
                const float r = (1.2f + 2.2f * d.depth) * d.size * cfg.size;
                if (d.bounces == 0)
                    line(screen, d.x, d.y, d.x - d.vx * 0.012f, d.y - d.vy * 0.012f, with_alpha(c, c.alpha * 0.5f), r * 1.3f);
                circlefill(screen, d.x, d.y, r, c);
            }
            else
            {
                const float r = (0.9f + 2.1f * d.depth) * d.size * cfg.size;
                circlefill(screen, d.x, d.y, r, c);
            }
        }
        for (const Splash &s : splashes_)
        {
            const float t = s.age / s.life;
            const Colour c = with_alpha(base, base.alpha * (1.0f - t) * 0.9f);
            if (s.hail)
            {
                circlefill(screen, s.x - 6.0f * t, s.y - 10.0f * std::sin(t * pi), 1.6f, c);
                circlefill(screen, s.x + 6.0f * t, s.y - 8.0f * std::sin(t * pi), 1.4f, c);
                circlefill(screen, s.x, s.y - 14.0f * std::sin(t * pi), 1.8f, c);
            }
            else
                ellipse(screen, s.x, s.y, 1.5f + 7.0f * t, 0.7f + 2.2f * t, c, 1.0f);
        }
        draw_snow_cover();
    }

    // ---- Precipitation3D ---------------------------------------------------------------------------------

    namespace
    {
        float fall_speed_3d(const PrecipitationConfig &c)
        {
            if (c.fall_speed > 0.0f)
                return c.fall_speed;
            switch (c.type)
            {
            case PrecipitationType::rain:
                return 9.0f;
            case PrecipitationType::hail:
                return 15.0f;
            case PrecipitationType::snow:
                return 1.3f;
            case PrecipitationType::none:
                break;
            }
            return 0.0f;
        }

        glm::mat4 axes_matrix(const glm::vec3 &x, const glm::vec3 &y, const glm::vec3 &z, const glm::vec3 &position)
        {
            return glm::mat4(glm::vec4(x, 0.0f), glm::vec4(y, 0.0f), glm::vec4(z, 0.0f), glm::vec4(position, 1.0f));
        }

        Bitmap *make_soft_texture(bool ring)
        {
            constexpr int size = 32;
            Bitmap *bitmap = create_bitmap(size, size);
            if (!bitmap)
                return nullptr;
            clear_to_colour(bitmap, {255, 255, 255, 0});
            for (int y = 0; y < size; ++y)
                for (int x = 0; x < size; ++x)
                {
                    const float dx = (x + 0.5f) / size * 2.0f - 1.0f;
                    const float dy = (y + 0.5f) / size * 2.0f - 1.0f;
                    const float r = std::sqrt(dx * dx + dy * dy);
                    float a;
                    if (ring)
                        a = clamp01(1.0f - std::fabs(r - 0.78f) / 0.16f);
                    else
                        a = clamp01(1.0f - r);
                    if (!ring)
                        a = std::sqrt(a);
                    putpixel(bitmap, x, y, {255, 255, 255, static_cast<Uint8>(a * 255.0f)});
                }
            return bitmap;
        }
    } // namespace

    float Precipitation3D::random(float low, float high) { return uniform(rng_, low, high); }

    void Precipitation3D::set_volume(const glm::vec3 &size) { volume_ = glm::max(size, glm::vec3(4.0f)); }

    void Precipitation3D::set_max_particles(int count)
    {
        max_particles_ = std::max(0, count);
        drops_.clear();
        active_ = 0;
    }

    void Precipitation3D::set_ground(std::function<float(float, float)> height) { ground_fn_ = std::move(height); }

    void Precipitation3D::set_accumulation(float gain, float melt)
    {
        gain_ = std::max(0.0f, gain);
        melt_ = std::max(0.0f, melt);
    }

    void Precipitation3D::set_snow_cover(float cover) { snow_cover_ = clamp01(cover); }

    glm::vec2 Precipitation3D::wind_from_clouds(const CloudLayer &clouds, float scale) { return clouds.wind * scale; }

    float Precipitation3D::ground_at(float x, float z) const { return ground_fn_ ? ground_fn_(x, z) : 0.0f; }

    void Precipitation3D::respawn(Drop &drop, const glm::vec3 &centre, bool anywhere)
    {
        drop.pos.x = centre.x + random(-0.5f, 0.5f) * volume_.x;
        drop.pos.z = centre.z + random(-0.5f, 0.5f) * volume_.z;
        drop.pos.y = anywhere ? centre.y + random(-0.5f, 0.5f) * volume_.y : centre.y + 0.5f * volume_.y - random(0.0f, 1.5f);
        drop.vel = glm::vec3(0.0f);
        drop.size = random(0.7f, 1.3f);
        drop.phase = random(0.0f, 2.0f * pi);
        drop.rest = random(0.0f, 1.0f);
        drop.bounces = 0;
    }

    void Precipitation3D::update(float dt, const Camera &camera)
    {
        if (dt <= 0.0f)
            return;
        dt = std::min(dt, 0.1f);
        weather.update(dt);
        const PrecipitationConfig &cfg = weather.current();
        time_ += dt;

        if (cfg.type != active_type_)
        {
            drops_.clear();
            splashes_.clear();
            active_ = 0;
            active_type_ = cfg.type;
        }
        if (static_cast<int>(drops_.size()) != max_particles_)
        {
            drops_.assign(static_cast<std::size_t>(max_particles_), Drop{});
            active_ = 0;
        }

        glm::vec3 forward = camera.target - camera.position;
        forward.y = 0.0f;
        forward = glm::length(forward) > 1e-4f ? glm::normalize(forward) : glm::vec3(0.0f, 0.0f, -1.0f);
        // Bias the box toward where the camera looks so more particles land in view.
        const glm::vec3 centre = camera.position + forward * (volume_.x * 0.2f);
        const float half_y = volume_.y * 0.5f;
        const float bottom = centre.y - half_y;

        int target = 0;
        if (cfg.type != PrecipitationType::none && cfg.intensity > 0.0f)
            target = std::clamp(static_cast<int>(cfg.intensity * static_cast<float>(max_particles_)), 1, max_particles_);
        for (int i = active_; i < target; ++i)
            respawn(drops_[static_cast<std::size_t>(i)], centre, true);
        active_ = target;

        const float fall = fall_speed_3d(cfg);
        const float hx = volume_.x * 0.5f, hz = volume_.z * 0.5f;
        for (int i = 0; i < active_; ++i)
        {
            Drop &d = drops_[static_cast<std::size_t>(i)];
            if (cfg.type == PrecipitationType::rain)
                d.vel = {cfg.wind.x, -fall * (0.9f + 0.2f * d.rest), cfg.wind.y};
            else if (cfg.type == PrecipitationType::hail)
            {
                if (d.bounces == 0)
                    d.vel = {cfg.wind.x * 0.4f, -fall * (0.85f + 0.3f * d.rest), cfg.wind.y * 0.4f};
                else
                    d.vel.y -= 9.8f * dt;
            }
            else
                d.vel = {cfg.wind.x + std::sin(time_ * 0.8f + d.phase) * 0.35f, -fall * (0.7f + 0.6f * d.rest),
                         cfg.wind.y + std::cos(time_ * 0.6f + d.phase) * 0.35f};
            d.pos += d.vel * dt;

            float dx = d.pos.x - centre.x, dz = d.pos.z - centre.z;
            dx -= 2.0f * hx * std::floor((dx + hx) / (2.0f * hx));
            dz -= 2.0f * hz * std::floor((dz + hz) / (2.0f * hz));
            d.pos.x = centre.x + dx;
            d.pos.z = centre.z + dz;

            const float ground = ground_at(d.pos.x, d.pos.z);
            if (d.pos.y <= ground && ground > bottom)
            {
                const float near = glm::distance(d.pos, camera.position);
                if (cfg.type == PrecipitationType::rain)
                {
                    if (cfg.splashes && near < 24.0f && random(0.0f, 1.0f) < 0.15f && splashes_.size() < 400)
                        splashes_.push_back({{d.pos.x, ground + 0.02f, d.pos.z}, 0.0f, 0.4f, false});
                    respawn(d, centre, false);
                }
                else if (cfg.type == PrecipitationType::hail && cfg.splashes && d.bounces < 2)
                {
                    if (d.bounces == 0 && near < 24.0f && splashes_.size() < 400)
                        splashes_.push_back({{d.pos.x, ground + 0.02f, d.pos.z}, 0.0f, 0.25f, true});
                    ++d.bounces;
                    d.pos.y = ground + 0.01f;
                    d.vel = {d.vel.x * 0.7f + random(-0.8f, 0.8f), std::fabs(d.vel.y) * 0.38f, d.vel.z * 0.7f + random(-0.8f, 0.8f)};
                }
                else
                    respawn(d, centre, false);
            }
            else if (d.pos.y < bottom)
                respawn(d, centre, false);
        }

        for (std::size_t i = 0; i < splashes_.size();)
        {
            splashes_[i].age += dt;
            if (splashes_[i].age >= splashes_[i].life)
            {
                splashes_[i] = splashes_.back();
                splashes_.pop_back();
            }
            else
                ++i;
        }

        if (cfg.type == PrecipitationType::snow && cfg.intensity > 0.0f)
            snow_cover_ = clamp01(snow_cover_ + gain_ * cfg.intensity * dt);
        else
            snow_cover_ = clamp01(snow_cover_ - melt_ * (cfg.type == PrecipitationType::rain ? 2.0f : 1.0f) * dt);
    }

    bool Precipitation3D::ensure_resources()
    {
        if (quad_ && dot_ && ring_)
            return true;
        release();
        resource_generation_ = detail::display_generation();
        if (!dot_)
            dot_ = make_soft_texture(false);
        if (!ring_)
            ring_ = make_soft_texture(true);
        if (!quad_)
        {
            std::vector<Vertex3D> vertices(4);
            const float xs[4] = {-0.5f, 0.5f, 0.5f, -0.5f};
            const float ys[4] = {-0.5f, -0.5f, 0.5f, 0.5f};
            for (int i = 0; i < 4; ++i)
            {
                vertices[static_cast<std::size_t>(i)].position[0] = xs[i];
                vertices[static_cast<std::size_t>(i)].position[1] = ys[i];
                vertices[static_cast<std::size_t>(i)].normal[0] = 0.0f;
                vertices[static_cast<std::size_t>(i)].normal[1] = 0.0f;
                vertices[static_cast<std::size_t>(i)].normal[2] = 1.0f;
                vertices[static_cast<std::size_t>(i)].uv[0] = xs[i] + 0.5f;
                vertices[static_cast<std::size_t>(i)].uv[1] = 0.5f - ys[i];
            }
            quad_ = create_mesh(vertices, {0, 1, 2, 0, 2, 3});
        }
        return quad_ && dot_ && ring_;
    }

    void Precipitation3D::release()
    {
        // The display teardown already invalidated these handles; touching them would be a use-after-free.
        if (resource_generation_ != detail::display_generation())
        {
            quad_ = nullptr;
            dot_ = ring_ = nullptr;
            return;
        }
        if (quad_)
            destroy_mesh(quad_);
        quad_ = nullptr;
        if (dot_)
            destroy_bitmap(dot_);
        if (ring_)
            destroy_bitmap(ring_);
        dot_ = ring_ = nullptr;
    }

    bool Precipitation3D::draw(const Camera &camera)
    {
        if (active_ <= 0 && splashes_.empty())
            return true;
        if (!ensure_resources())
            return false;

        const PrecipitationConfig &cfg = weather.current();
        const Colour base = cfg.colour.alpha ? cfg.colour : precipitation_default_colour(cfg.type);
        const glm::vec3 forward = glm::normalize(camera.target - camera.position);
        glm::vec3 right = glm::cross(forward, camera.up);
        right = glm::length(right) > 1e-4f ? glm::normalize(right) : glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::vec3 up = glm::cross(right, forward);

        instances_.clear();
        for (int i = 0; i < active_; ++i)
        {
            const Drop &d = drops_[static_cast<std::size_t>(i)];
            const glm::vec3 to_camera = camera.position - d.pos;
            const float dist = glm::length(to_camera);
            if (dist < 0.8f)
                continue;
            const glm::vec3 view = to_camera / dist;
            // Keep far particles at least about a pixel wide so they do not shimmer out of existence.
            const float min_width = dist * 0.0027f;
            InstanceData3D instance;
            instance.tint = base;

            const bool streak = cfg.type == PrecipitationType::rain || (cfg.type == PrecipitationType::hail && d.bounces == 0);
            if (streak)
            {
                const float speed = glm::length(d.vel);
                const glm::vec3 dir = speed > 1e-3f ? d.vel / speed : glm::vec3(0.0f, -1.0f, 0.0f);
                glm::vec3 side = glm::cross(dir, view);
                side = glm::length(side) > 1e-3f ? glm::normalize(side) : right;
                const bool rain = cfg.type == PrecipitationType::rain;
                const float length = speed * (rain ? 0.035f : 0.012f) * cfg.size * d.size;
                const float width = std::max((rain ? 0.009f : 0.035f) * cfg.size * d.size, min_width * (rain ? 1.0f : 1.5f));
                instance.model = axes_matrix(side * width, dir * std::max(std::min(length, dist * 0.4f), width), view, d.pos);
            }
            else
            {
                const float size = (cfg.type == PrecipitationType::snow ? 0.04f : 0.035f) * cfg.size * d.size;
                const float width = std::max(size, min_width * 1.5f);
                instance.model = axes_matrix(right * width, up * width, -forward, d.pos);
            }
            instances_.push_back(instance);
        }

        Material material;
        material.lit = false;
        material.fog = true;
        material.cull = CullMode::none;
        material.blend = MaterialBlend::alpha;
        material.texture = dot_;
        bool ok = true;
        if (!instances_.empty())
            ok = draw_mesh_instanced(quad_, instances_, material);

        instances_.clear();
        for (const Splash &s : splashes_)
        {
            const float t = s.age / s.life;
            InstanceData3D instance;
            const float radius = s.hail ? lerpf(0.04f, 0.16f, t) : lerpf(0.04f, 0.32f, t);
            instance.tint = with_alpha(base, base.alpha * (1.0f - t) * (s.hail ? 0.9f : 0.75f) + 1.0f);
            instance.model = axes_matrix({radius * 2.0f, 0.0f, 0.0f}, {0.0f, 0.0f, radius * 2.0f}, {0.0f, 1.0f, 0.0f}, s.pos);
            instances_.push_back(instance);
        }
        if (!instances_.empty())
        {
            material.texture = ring_;
            ok = draw_mesh_instanced(quad_, instances_, material) && ok;
        }
        return ok;
    }

    void precipitation_apply_environment(Environment3D &environment, const PrecipitationConfig &weather)
    {
        const float i = clamp01(weather.intensity);
        if (weather.type == PrecipitationType::none || i <= 0.0f)
            return;
        Colour grey;
        float fog;
        switch (weather.type)
        {
        case PrecipitationType::snow:
            grey = {214, 220, 230, 255};
            fog = 0.0013f;
            break;
        case PrecipitationType::hail:
            grey = {150, 160, 176, 255};
            fog = 0.0007f;
            break;
        default:
            grey = {132, 142, 158, 255};
            fog = 0.0009f;
            break;
        }
        const float amount = clamp01(i * 1.5f) * 0.75f;
        environment.fog_enabled = true;
        environment.fog_density += fog * i;
        environment.fog_colour = lerp_colour(environment.fog_colour, grey, amount);
        environment.sky_zenith = lerp_colour(environment.sky_zenith, grey, amount * 0.8f);
        environment.sky_horizon = lerp_colour(environment.sky_horizon, grey, amount);
        environment.sun_intensity *= 1.0f - 0.55f * i;
        if (!environment.clouds.enabled)
            environment.clouds = cloud_layer(CloudType::stratus);
        environment.clouds.enabled = true;
        environment.clouds.coverage = std::max(environment.clouds.coverage, 0.55f + 0.45f * i);
        environment.clouds.density = std::max(environment.clouds.density, 0.7f + 0.3f * i);
    }

    Colour snow_tinted(Colour base, float cover)
    {
        return lerp_colour(base, {244, 248, 255, base.alpha}, clamp01(cover));
    }

    // ---- PrecipitationLens -------------------------------------------------------------------------------

    void PrecipitationLens::clear()
    {
        drops_.clear();
        accumulator_ = 0.0f;
    }

    void PrecipitationLens::update(float dt, const PrecipitationConfig &weather)
    {
        dt = std::min(dt, 0.1f);
        if (dt <= 0.0f)
            return;
        float rate = 0.0f;
        float radius_scale = 1.0f;
        if (!droplets && accumulator_ == 0.0f && drops_.empty())
            return;
        if (!droplets)
            rate = 0.0f;
        else if (weather.type == PrecipitationType::rain)
            rate = 5.0f * weather.intensity;
        else if (weather.type == PrecipitationType::snow)
        {
            rate = 2.5f * weather.intensity;
            radius_scale = 0.55f;
        }
        accumulator_ += rate * dt;
        while (accumulator_ >= 1.0f)
        {
            accumulator_ -= 1.0f;
            if (drops_.size() < 160)
                drops_.push_back({uniform(rng_, 0.0f, 1.0f), uniform(rng_, 0.0f, 1.0f),
                                  uniform(rng_, 1.5f, 5.0f) * radius_scale, 0.0f, 0.0f, 0.0f, false});
        }
        // Melting flakes are short-lived spots that never run; only rain beads up and slides.
        const bool runs = weather.type != PrecipitationType::snow;
        const float lifetime = runs ? 9.0f : 2.5f;
        for (std::size_t i = 0; i < drops_.size();)
        {
            Droplet &d = drops_[i];
            d.age += dt;
            // Larger drops are heavy enough to run sooner.
            if (runs && !d.sliding && d.age > 1.5f - d.radius * 0.12f + (static_cast<float>((i * 37) % 11) * 0.15f))
                d.sliding = true;
            if (d.sliding)
            {
                // Straight down, pausing now and then as the bead catches on the glass; no sideways wobble.
                const float stick = std::sin(d.age * 3.0f + static_cast<float>(i) * 1.7f);
                const float target_speed = stick > 0.55f ? 0.05f : 0.25f + d.radius * 0.03f;
                d.slide += (target_speed - d.slide) * std::min(1.0f, dt * 4.0f);
                d.y += d.slide * dt * 0.25f * (0.6f + d.radius * 0.15f);
                d.trail = std::min(d.trail + dt * 40.0f * d.slide, 60.0f);
            }
            if (d.y > 1.05f || d.age > lifetime + 8.0f || (!d.sliding && d.age > lifetime))
            {
                drops_[i] = drops_.back();
                drops_.pop_back();
            }
            else
                ++i;
        }
    }

    void PrecipitationLens::draw(float x, float y, float width, float height, const PrecipitationConfig &weather)
    {
        const float i = clamp01(weather.intensity);
        if (weather.type != PrecipitationType::none && i > 0.0f)
        {
            Colour haze;
            switch (weather.type)
            {
            case PrecipitationType::snow:
                haze = {232, 238, 248, static_cast<Uint8>(70.0f * i)};
                break;
            case PrecipitationType::hail:
                haze = {150, 160, 176, static_cast<Uint8>(45.0f * i)};
                break;
            default:
                haze = {120, 132, 150, static_cast<Uint8>(55.0f * i)};
                break;
            }
            rectfill(screen, x, y, x + width, y + height, haze);
        }
        for (const Droplet &d : drops_)
        {
            const float px = x + d.x * width;
            const float py = y + d.y * height;
            float fade = clamp01(d.age * 3.0f);
            if (weather.type == PrecipitationType::snow)
            {
                // A melting flake: a faint soft blotch that fades away.
                fade *= clamp01((2.5f - d.age) * 1.5f);
                circlefill(screen, px, py, d.radius, {225, 232, 244, static_cast<Uint8>(34.0f * fade)});
                circlefill(screen, px, py, d.radius * 0.55f, {240, 244, 252, static_cast<Uint8>(40.0f * fade)});
                continue;
            }
            if (d.trail > 1.0f)
            {
                // A wet streak that thins out as it runs up from the bead.
                constexpr int segments = 4;
                for (int segment = 0; segment < segments; ++segment)
                {
                    const float from = d.trail * static_cast<float>(segment + 1) / segments;
                    const float to = d.trail * static_cast<float>(segment) / segments;
                    const float strength = 1.0f - static_cast<float>(segment) / segments;
                    line(screen, px, py - from, px, py - to, {200, 215, 235, static_cast<Uint8>(16.0f * strength * fade)}, 1.0f);
                }
            }
            circlefill(screen, px, py, d.radius, {150, 175, 205, static_cast<Uint8>(34.0f * fade)});
            circlefill(screen, px + d.radius * 0.15f, py + d.radius * 0.2f, d.radius * 0.7f, {10, 14, 22, static_cast<Uint8>(22.0f * fade)});
            circlefill(screen, px - d.radius * 0.3f, py - d.radius * 0.35f, std::max(0.8f, d.radius * 0.28f),
                       {255, 255, 255, static_cast<Uint8>(150.0f * fade)});
        }
    }

    // ---- PrecipitationAudio ------------------------------------------------------------------------------

    namespace
    {
        // Filtered noise: rain is a bright hiss, hail a gated clatter, snow a faint low wind.
        Sample *make_weather_sample(PrecipitationType type)
        {
            SynthParams params;
            params.duration = 2.0f;
            params.envelope = {0.0f, 0.0f, 1.0f, 0.0f};
            params.oscillators.push_back({Waveform::noise, 440.0f, 1.0f, {1.0f, 1.0f}});
            params.noise_seed = type == PrecipitationType::rain ? 0xC0FFEEu : type == PrecipitationType::hail ? 0xBADC0DEu : 0xFEEDu;
            std::vector<std::int16_t> pcm = generate_audio(params);
            if (pcm.empty())
                return nullptr;

            std::mt19937 rng(static_cast<unsigned>(params.noise_seed));
            float low = 0.0f, band = 0.0f;
            float gate = 1.0f;
            std::vector<float> out(pcm.size());
            for (std::size_t n = 0; n < pcm.size(); ++n)
            {
                const float in = static_cast<float>(pcm[n]) / 32768.0f;
                float v;
                if (type == PrecipitationType::snow)
                {
                    low += (in - low) * 0.015f;
                    v = low * (0.8f + 0.2f * std::sin(static_cast<float>(n) / 44100.0f * pi));
                }
                else if (type == PrecipitationType::hail)
                {
                    if (n % 220 == 0)
                        gate = uniform(rng, 0.0f, 1.0f) < 0.12f ? 1.0f : gate * 0.15f;
                    gate *= 0.9985f;
                    low += (in - low) * 0.55f;
                    v = low * gate;
                }
                else
                {
                    low += (in - low) * 0.7f;
                    band += (low - band) * 0.08f;
                    v = low - band * 0.8f;
                }
                out[n] = v;
            }
            float max_abs = 1e-4f;
            for (float v : out)
                max_abs = std::max(max_abs, std::fabs(v));
            const float gain = (type == PrecipitationType::hail ? 0.9f : 0.7f) / max_abs;
            for (std::size_t n = 0; n < out.size(); ++n)
                pcm[n] = static_cast<std::int16_t>(std::clamp(out[n] * gain, -1.0f, 1.0f) * 32767.0f);
            return create_sample_from_pcm(pcm.data(), pcm.size(), params.sample_rate, 1);
        }
    } // namespace

    int PrecipitationAudio::volume_for(const PrecipitationConfig &weather, int master)
    {
        if (weather.type == PrecipitationType::none || weather.intensity <= 0.0f)
            return 0;
        const float type_gain = weather.type == PrecipitationType::snow ? 0.3f : 1.0f;
        const float level = std::pow(clamp01(weather.intensity), 0.7f) * type_gain * static_cast<float>(std::clamp(master, 0, 255));
        return std::clamp(static_cast<int>(level), 0, 255);
    }

    void PrecipitationAudio::update(const PrecipitationConfig &weather, int master)
    {
        const int volume = volume_for(weather, master);
        const int bucket = volume < 2 ? 0 : std::min(255, ((volume + 2) / 4) * 4);
        const int type = bucket == 0 ? -1 : static_cast<int>(weather.type) - 1;
        if (type == playing_type_ && bucket == playing_volume_)
            return;
        if (voice_)
            stop_voice(voice_);
        voice_ = 0;
        playing_type_ = type;
        playing_volume_ = bucket;
        if (type < 0)
            return;
        if (!samples_[type])
            samples_[type] = make_weather_sample(weather.type);
        if (samples_[type])
            voice_ = play_sample(samples_[type], bucket, 128, 1000, -1);
    }

    void PrecipitationAudio::release()
    {
        if (voice_)
            stop_voice(voice_);
        voice_ = 0;
        for (Sample *&sample : samples_)
        {
            if (sample)
                destroy_sample(sample);
            sample = nullptr;
        }
        playing_type_ = playing_volume_ = -1;
    }

} // namespace sl

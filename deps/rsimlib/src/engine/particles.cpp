/** @file
 * @brief Implements particle emitters, updates, and rendering.
 */

#include "particles.h"

#include "display.h"
#include "gl2d.h"

#include <SDL2/SDL_opengl.h>

#include <algorithm>
#include <cmath>
#include <random>

namespace sl
{
    namespace
    {

        constexpr float pi = 3.14159265358979323846f;

        std::mt19937 &rng()
        {
            static std::mt19937 generator{std::random_device{}()};
            return generator;
        }

        float random_range(float minimum, float maximum)
        {
            if (maximum <= minimum)
            {
                return minimum;
            }
            std::uniform_real_distribution<float> distribution(minimum, maximum);
            return distribution(rng());
        }

        /** Linearly interpolate one colour channel. */
        Uint8 lerp_channel(Uint8 from, Uint8 to, float t)
        {
            return static_cast<Uint8>(std::clamp(static_cast<float>(from) + (static_cast<float>(to) - static_cast<float>(from)) * t, 0.0f, 255.0f));
        }

    } // namespace

    ParticleEmitter::ParticleEmitter(const EmitterConfig &config, float x, float y)
        : config_(config), x_(x), y_(y)
    {
        particles_.reserve(static_cast<std::size_t>(std::max(0, config_.max_particles)));
    }

    void ParticleEmitter::set_position(float x, float y)
    {
        x_ = x;
        y_ = y;
    }

    float ParticleEmitter::x() const
    {
        return x_;
    }

    float ParticleEmitter::y() const
    {
        return y_;
    }

    void ParticleEmitter::set_active(bool active)
    {
        active_ = active;
    }

    bool ParticleEmitter::is_active() const
    {
        return active_;
    }

    void ParticleEmitter::clear()
    {
        particles_.clear();
        spawn_accumulator_ = 0.0f;
    }

    std::size_t ParticleEmitter::particle_count() const
    {
        return particles_.size();
    }

    void ParticleEmitter::update(float dt_seconds)
    {
        if (dt_seconds <= 0.0f)
        {
            return;
        }

        // integrate and cull dead particles
        for (std::size_t index = 0; index < particles_.size();)
        {
            Particle &particle = particles_[index];
            particle.life -= dt_seconds;
            if (particle.life <= 0.0f)
            {
                particle = particles_.back();
                particles_.pop_back();
                continue;
            }
            particle.vy += config_.gravity * dt_seconds;
            particle.x += particle.vx * dt_seconds;
            particle.y += particle.vy * dt_seconds;
            ++index;
        }

        if (!active_ || config_.emit_rate <= 0.0f)
        {
            return;
        }

        spawn_accumulator_ += config_.emit_rate * dt_seconds;
        const int maxParticles = std::max(0, config_.max_particles);
        while (spawn_accumulator_ >= 1.0f && static_cast<int>(particles_.size()) < maxParticles)
        {
            spawn_accumulator_ -= 1.0f;

            const float speed = random_range(config_.min_speed, config_.max_speed);
            const float angleDegrees = config_.direction_degrees + random_range(-config_.spread_degrees, config_.spread_degrees);
            const float angleRadians = angleDegrees * pi / 180.0f;

            Particle particle;
            particle.x = x_;
            particle.y = y_;
            particle.vx = speed * std::cos(angleRadians);
            particle.vy = speed * std::sin(angleRadians);
            particle.max_life = std::max(0.0001f, config_.particle_lifetime);
            particle.life = particle.max_life;
            particles_.push_back(particle);
        }
    }

    void ParticleEmitter::render() const
    {
        if (particles_.empty())
        {
            return;
        }

        std::uint32_t glTexture = 0;
        if (config_.texture && upload_bitmap(config_.texture))
        {
            glTexture = config_.texture->gpu_texture;
        }

        std::vector<detail::GLVertex> vertices;
        vertices.reserve(particles_.size() * 6);

        for (const Particle &particle : particles_)
        {
            const float t = 1.0f - particle.life / particle.max_life;
            const float size = std::max(0.0f, config_.start_size + (config_.end_size - config_.start_size) * t);
            const float half = size * 0.5f;

            const float r = lerp_channel(config_.start_colour.red, config_.end_colour.red, t) / 255.0f;
            const float g = lerp_channel(config_.start_colour.green, config_.end_colour.green, t) / 255.0f;
            const float b = lerp_channel(config_.start_colour.blue, config_.end_colour.blue, t) / 255.0f;
            const float a = lerp_channel(config_.start_colour.alpha, config_.end_colour.alpha, t) / 255.0f;

            const float left = particle.x - half;
            const float top = particle.y - half;
            const float right = particle.x + half;
            const float bottom = particle.y + half;

            const detail::GLVertex topLeft{left, top, 0.0f, 0.0f, r, g, b, a};
            const detail::GLVertex topRight{right, top, 1.0f, 0.0f, r, g, b, a};
            const detail::GLVertex bottomRight{right, bottom, 1.0f, 1.0f, r, g, b, a};
            const detail::GLVertex bottomLeft{left, bottom, 0.0f, 1.0f, r, g, b, a};

            vertices.push_back(topLeft);
            vertices.push_back(topRight);
            vertices.push_back(bottomRight);
            vertices.push_back(topLeft);
            vertices.push_back(bottomRight);
            vertices.push_back(bottomLeft);
        }

        detail::gl2d_begin(screen_width(), screen_height());
        detail::gl2d_submit(GL_TRIANGLES, vertices.data(), static_cast<int>(vertices.size()), glTexture);
    }

} // namespace sl

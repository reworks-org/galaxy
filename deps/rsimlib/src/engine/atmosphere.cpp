#include "atmosphere.h"

#include "atmosphere_internal.h"
#include "error.h"

#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <cmath>

namespace sl
{
    namespace
    {
        constexpr int view_steps = 16;
        constexpr int light_steps = 6;
        constexpr float mie_extinction = 1.11f;

        Environment3D current;
        bool have_environment = false;
        // Fog derived from the environment when it is set.
        glm::vec3 fog_colour{0.0f};
        glm::vec3 fog_beta{0.0f};

        glm::vec3 to_sun(const Environment3D &environment)
        {
            const float length = glm::length(environment.sun_direction);
            return length > 1e-6f ? -environment.sun_direction / length : glm::vec3(0.0f, 1.0f, 0.0f);
        }

        glm::vec3 local_up(const Environment3D &environment)
        {
            const float length = glm::length(environment.up);
            return length > 1e-6f ? environment.up / length : glm::vec3(0.0f, 1.0f, 0.0f);
        }

        glm::vec3 colour_to_vec(Colour colour)
        {
            return {colour.red / 255.0f, colour.green / 255.0f, colour.blue / 255.0f};
        }

        bool ray_sphere(const glm::vec3 &origin, const glm::vec3 &direction, float radius, float &t0, float &t1)
        {
            const float b = glm::dot(origin, direction);
            const float c = glm::dot(origin, origin) - radius * radius;
            float discriminant = b * b - c;
            if (discriminant < 0.0f)
                return false;
            discriminant = std::sqrt(discriminant);
            t0 = -b - discriminant;
            t1 = -b + discriminant;
            return true;
        }

        void optical_depth(const Environment3D &e, const glm::vec3 &p, const glm::vec3 &d, float length, float &rayleigh,
                           float &mie)
        {
            rayleigh = 0.0f;
            mie = 0.0f;
            const float step = length / static_cast<float>(light_steps);
            for (int i = 0; i < light_steps; ++i)
            {
                const float height = std::max(glm::length(p + d * ((static_cast<float>(i) + 0.5f) * step)) - e.planet_radius, 0.0f);
                rayleigh += std::exp(-height / e.rayleigh_scale_height) * step;
                mie += std::exp(-height / e.mie_scale_height) * step;
            }
        }

        // Transmittance of sunlight from `p` out of the atmosphere; zero when the planet blocks the sun.
        glm::vec3 sun_transmittance(const Environment3D &e, const glm::vec3 &p, const glm::vec3 &sun)
        {
            float q0, q1;
            if (ray_sphere(p, sun, e.planet_radius, q0, q1) && q1 > 0.0f && q0 > 0.0f)
                return glm::vec3(0.0f);
            float s0, s1;
            if (!ray_sphere(p, sun, e.planet_radius + e.atmosphere_height, s0, s1))
                return glm::vec3(1.0f);
            float od_r, od_m;
            optical_depth(e, p, sun, s1, od_r, od_m);
            return glm::exp(-(e.rayleigh * od_r + mie_extinction * e.mie * od_m));
        }

        // Single-scattered light along a ray; mirrors planet_sky() in shaders/glsl/sky.frag.
        glm::vec3 planet_sky(const Environment3D &e, const glm::vec3 &rd)
        {
            const glm::vec3 up = local_up(e);
            const glm::vec3 sun = to_sun(e);
            const float atmosphere_radius = e.planet_radius + e.atmosphere_height;
            const glm::vec3 ro = up * (e.planet_radius + e.camera_altitude);
            float t0, t1;
            if (!ray_sphere(ro, rd, atmosphere_radius, t0, t1))
                return glm::vec3(0.0f);
            t0 = std::max(t0, 0.0f);
            bool hits_planet = false;
            float p0, p1;
            if (ray_sphere(ro, rd, e.planet_radius, p0, p1) && p1 > 0.0f)
            {
                const float hit = p0 > 0.0f ? p0 : p1;
                if (hit > t0)
                {
                    t1 = std::min(t1, hit);
                    hits_planet = true;
                }
            }
            const float length = t1 - t0;
            if (length <= 0.0f)
                return glm::vec3(0.0f);
            const float step = length / static_cast<float>(view_steps);
            float od_r = 0.0f, od_m = 0.0f;
            glm::vec3 sum_r(0.0f), sum_m(0.0f);
            for (int i = 0; i < view_steps; ++i)
            {
                const glm::vec3 p = ro + rd * (t0 + (static_cast<float>(i) + 0.5f) * step);
                const float height = std::max(glm::length(p) - e.planet_radius, 0.0f);
                const float dr = std::exp(-height / e.rayleigh_scale_height) * step;
                const float dm = std::exp(-height / e.mie_scale_height) * step;
                od_r += dr;
                od_m += dm;
                float s0, s1;
                if (!ray_sphere(p, sun, atmosphere_radius, s0, s1))
                    continue;
                float q0, q1;
                if (ray_sphere(p, sun, e.planet_radius, q0, q1) && q1 > 0.0f && q0 > 0.0f)
                    continue;
                float light_r, light_m;
                optical_depth(e, p, sun, s1, light_r, light_m);
                const glm::vec3 attenuation =
                    glm::exp(-(e.rayleigh * (od_r + light_r) + mie_extinction * e.mie * (od_m + light_m)));
                sum_r += attenuation * dr;
                sum_m += attenuation * dm;
            }
            const float mu = glm::dot(rd, sun);
            const float g = e.mie_g;
            const float pi = glm::pi<float>();
            const float phase_r = 3.0f / (16.0f * pi) * (1.0f + mu * mu);
            const float phase_m = 3.0f / (8.0f * pi) * ((1.0f - g * g) * (1.0f + mu * mu)) /
                                  ((2.0f + g * g) * std::pow(std::max(1.0f + g * g - 2.0f * g * mu, 1e-4f), 1.5f));
            glm::vec3 colour = e.sun_intensity * (sum_r * e.rayleigh * phase_r + sum_m * e.mie * phase_m);
            if (hits_planet)
            {
                const glm::vec3 hit_point = ro + rd * t1;
                const glm::vec3 normal = glm::normalize(hit_point);
                const float lit = std::max(glm::dot(normal, sun), 0.0f);
                const glm::vec3 arriving = sun_transmittance(e, hit_point + normal * 2.0f, sun);
                const glm::vec3 view = glm::exp(-(e.rayleigh * od_r + mie_extinction * e.mie * od_m));
                colour += e.ground_albedo * lit * arriving * view * e.sun_intensity * 0.12f;
            }
            return colour;
        }

        // Extinction per metre of fog at the camera, per channel.
        glm::vec3 fog_extinction(const Environment3D &e)
        {
            const float altitude = std::max(e.camera_altitude, 0.0f);
            if (e.planet)
            {
                const glm::vec3 beta = (e.rayleigh + glm::vec3(mie_extinction * e.mie)) * e.haze_scale;
                return beta * std::exp(-altitude / e.rayleigh_scale_height);
            }
            return glm::vec3(std::max(e.fog_density, 0.0f) * std::exp(-std::max(e.fog_height_falloff, 0.0f) * altitude));
        }

        enum SkyRows
        {
            row_right = 0,       // xyz: camera right, w: tan(fov / 2) * aspect
            row_camera_up,       // xyz: camera up, w: tan(fov / 2)
            row_forward,         // xyz: camera forward, w: flags (1 planet)
            row_sun,             // xyz: toward the sun, w: intensity
            row_local_up,        // xyz: local vertical, w: camera altitude
            row_zenith,          // rgb: zenith colour, w: cos of the sun's angular radius
            row_horizon,         // rgb: horizon colour, w: Mie g
            row_rayleigh,        // rgb: Rayleigh coefficients, w: Rayleigh scale height
            row_planet,          // x: Mie coefficient, y: Mie scale height, z: planet radius, w: atmosphere height
            row_camera_position, // xyz: camera world position
            row_ground,          // rgb: ground albedo
            row_sun_colour,      // rgb: sunlight colour after the atmosphere
            row_layers,          // four rows per cloud layer, farthest layer first (see CloudRows)
            max_cloud_layers = 4,
            row_end = row_layers + max_cloud_layers * 4
        };
        static_assert(SkyRows::row_end == detail::SkyDrawParams::rows, "sky row count");

        enum CloudRows
        {
            cloud_main,  // x: altitude, y: coverage, z: density, w: feature scale
            cloud_extra, // x: softness, yz: wind offset in metres, w: slab thickness
            cloud_flags, // x: extinction per metre, y: phase anisotropy, z: flags (1 on, 2 volumetric), w: wind angle
            cloud_shape  // x: stretch along the wind, y: feature frequency, z: vertical profile, w: coverage bias
        };

        struct TypeShape
        {
            float stretch, frequency, profile, coverage_bias;
        };

        TypeShape type_shape(CloudType type)
        {
            switch (type)
            {
            case CloudType::stratocumulus:
                return {1.7f, 1.3f, 1.0f, 0.0f};
            case CloudType::stratus:
                return {1.0f, 0.6f, 1.0f, -0.08f};
            case CloudType::altocumulus:
                return {1.0f, 1.8f, 1.0f, 0.0f};
            case CloudType::cirrus:
                return {5.0f, 1.6f, 3.0f, 0.04f};
            case CloudType::cumulonimbus:
                return {1.0f, 0.7f, 2.0f, 0.0f};
            case CloudType::cumulus:
            default:
                return {1.0f, 1.0f, 0.0f, 0.0f};
            }
        }

        void set_row(detail::SkyDrawParams &params, int row, const glm::vec3 &v, float w)
        {
            params.data[row][0] = v.x;
            params.data[row][1] = v.y;
            params.data[row][2] = v.z;
            params.data[row][3] = w;
        }
    } // namespace

    glm::vec3 atmosphere_sun_colour(const Environment3D &environment)
    {
        const glm::vec3 up = local_up(environment);
        const glm::vec3 origin = up * (environment.planet_radius + std::max(environment.camera_altitude, 0.0f));
        return sun_transmittance(environment, origin, to_sun(environment));
    }

    glm::vec3 atmosphere_transmittance(const Environment3D &environment, float distance)
    {
        if (!environment.fog_enabled)
            return glm::vec3(1.0f);
        return glm::exp(-fog_extinction(environment) * std::max(distance, 0.0f));
    }

    glm::vec3 atmosphere_sky_colour(const Environment3D &environment, const glm::vec3 &direction)
    {
        const glm::vec3 rd = glm::normalize(direction);
        if (environment.planet)
            return glm::vec3(1.0f) - glm::exp(-planet_sky(environment, rd));
        const glm::vec3 up = local_up(environment);
        const float elevation = glm::dot(rd, up);
        const glm::vec3 zenith = colour_to_vec(environment.sky_zenith);
        const glm::vec3 horizon = colour_to_vec(environment.sky_horizon);
        glm::vec3 sky = glm::mix(horizon, zenith, std::pow(std::clamp(elevation, 0.0f, 1.0f), 0.45f));
        if (elevation < 0.0f)
            sky = glm::mix(horizon, horizon * 0.55f, glm::smoothstep(0.0f, 0.25f, -elevation));
        const float mu = std::max(glm::dot(rd, to_sun(environment)), 0.0f);
        return sky + atmosphere_sun_colour(environment) * (std::pow(mu, 48.0f) * 0.18f + std::pow(mu, 600.0f) * 0.4f);
    }

    CloudLayer cloud_layer(CloudType type)
    {
        CloudLayer layer;
        layer.enabled = true;
        layer.type = type;
        switch (type)
        {
        case CloudType::cumulus:
            layer.altitude = 1500.0f; layer.thickness = 1200.0f; layer.scale = 3500.0f;
            layer.coverage = 0.45f; layer.density = 0.9f; layer.softness = 0.35f; layer.volumetric = true;
            break;
        case CloudType::stratocumulus:
            layer.altitude = 1300.0f; layer.thickness = 600.0f; layer.scale = 5000.0f;
            layer.coverage = 0.6f; layer.density = 0.85f; layer.softness = 0.4f; layer.volumetric = true;
            break;
        case CloudType::stratus:
            layer.altitude = 700.0f; layer.thickness = 400.0f; layer.scale = 12000.0f;
            layer.coverage = 0.8f; layer.density = 0.8f; layer.softness = 0.6f; layer.volumetric = false;
            break;
        case CloudType::altocumulus:
            layer.altitude = 4500.0f; layer.thickness = 500.0f; layer.scale = 4000.0f;
            layer.coverage = 0.5f; layer.density = 0.6f; layer.softness = 0.45f; layer.volumetric = false;
            break;
        case CloudType::cirrus:
            layer.altitude = 9000.0f; layer.thickness = 300.0f; layer.scale = 9000.0f;
            layer.coverage = 0.5f; layer.density = 0.6f; layer.softness = 0.85f; layer.volumetric = false;
            layer.wind = {30.0f, 8.0f};
            break;
        case CloudType::cumulonimbus:
            layer.altitude = 1000.0f; layer.thickness = 9000.0f; layer.scale = 8000.0f;
            layer.coverage = 0.42f; layer.density = 1.0f; layer.softness = 0.3f; layer.volumetric = true;
            break;
        }
        return layer;
    }

    void set_environment_3d(const Environment3D &environment)
    {
        current = environment;
        have_environment = true;
        if (!environment.fog_enabled)
        {
            fog_beta = glm::vec3(0.0f);
            return;
        }
        fog_beta = fog_extinction(environment);
        if (environment.planet)
        {
            // Haze takes the colour of the sky at the horizon, to the side of the sun.
            const glm::vec3 up = local_up(environment);
            glm::vec3 side = glm::cross(up, to_sun(environment));
            if (glm::length(side) < 1e-3f)
                side = glm::cross(up, glm::vec3(1.0f, 0.0f, 0.0f));
            side = glm::normalize(side);
            fog_colour = atmosphere_sky_colour(environment, glm::normalize(side + up * 0.03f));
        }
        else
        {
            fog_colour = colour_to_vec(environment.fog_colour);
        }
    }

    void clear_environment_3d()
    {
        have_environment = false;
        fog_beta = glm::vec3(0.0f);
    }

    bool draw_sky_3d()
    {
        detail::Renderer *renderer = detail::active_renderer();
        detail::ActiveCameraInfo info;
        if (!renderer || !detail::active_camera_info(info))
        {
            detail::set_error("draw_sky_3d requires an active 3D surface");
            return false;
        }
        if (!have_environment)
        {
            detail::set_error("draw_sky_3d requires set_environment_3d() first");
            return false;
        }
        const Environment3D &e = current;
        const Camera &camera = info.camera;
        const glm::vec3 forward = glm::normalize(camera.target - camera.position);
        const glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));
        const glm::vec3 camera_up = glm::cross(right, forward);
        const float tan_y = std::tan(glm::radians(camera.field_of_view) * 0.5f);
        const float aspect = static_cast<float>(info.width) / static_cast<float>(std::max(info.height, 1));

        detail::SkyDrawParams params;
        const int flags = e.planet ? 1 : 0;
        set_row(params, row_right, right, tan_y * aspect);
        set_row(params, row_camera_up, camera_up, tan_y);
        set_row(params, row_forward, forward, static_cast<float>(flags));
        set_row(params, row_sun, to_sun(e), e.sun_intensity);
        set_row(params, row_local_up, local_up(e), e.camera_altitude);
        const float radius = glm::radians(e.sun_diameter) * 0.5f;
        set_row(params, row_zenith, colour_to_vec(e.sky_zenith), e.sun_diameter > 0.0f ? std::cos(radius) : 2.0f);
        set_row(params, row_horizon, colour_to_vec(e.sky_horizon), e.mie_g);
        set_row(params, row_rayleigh, e.rayleigh, e.rayleigh_scale_height);
        set_row(params, row_planet, glm::vec3(e.mie, e.mie_scale_height, e.planet_radius), e.atmosphere_height);
        set_row(params, row_camera_position, glm::vec3(e.camera_position), 0.0f);
        set_row(params, row_ground, e.ground_albedo, 0.0f);
        set_row(params, row_sun_colour, atmosphere_sun_colour(e), 0.0f);

        // Farthest layer first, so nearer layers are composited over it.
        const CloudLayer *layers[max_cloud_layers];
        int layer_count = 0;
        if (e.clouds.enabled)
            layers[layer_count++] = &e.clouds;
        for (const CloudLayer &layer : e.more_clouds)
            if (layer.enabled && layer_count < max_cloud_layers)
                layers[layer_count++] = &layer;
        std::stable_sort(layers, layers + layer_count, [&](const CloudLayer *a, const CloudLayer *b) {
            return std::abs(a->altitude - e.camera_altitude) > std::abs(b->altitude - e.camera_altitude);
        });
        for (int i = 0; i < layer_count; ++i)
        {
            const CloudLayer &c = *layers[i];
            const int row = row_layers + i * 4;
            const float scale = std::max(c.scale, 1.0f);
            const float thickness = std::max(c.thickness, 1.0f);
            set_row(params, row + cloud_main, glm::vec3(c.altitude, c.coverage, c.density), scale);
            // Keep the offset small so float precision holds over long sessions.
            const double wrap = static_cast<double>(scale) * 1024.0;
            const glm::vec2 wind_offset(static_cast<float>(std::fmod(static_cast<double>(c.wind.x) * e.time, wrap)),
                                        static_cast<float>(std::fmod(static_cast<double>(c.wind.y) * e.time, wrap)));
            set_row(params, row + cloud_extra, glm::vec3(c.softness, -wind_offset.x, -wind_offset.y), thickness);
            const float angle = glm::length(c.wind) > 1e-4f ? std::atan2(-c.wind.y, -c.wind.x) : 0.0f;
            const float flags = 1.0f + (c.volumetric ? 2.0f : 0.0f);
            set_row(params, row + cloud_flags, glm::vec3(5.0f * std::max(c.density, 0.05f) / thickness, 0.5f, flags), angle);
            const TypeShape shape = type_shape(c.type);
            set_row(params, row + cloud_shape, glm::vec3(shape.stretch, shape.frequency, shape.profile), shape.coverage_bias);
        }
        if (!renderer->draw_sky(params))
        {
            detail::set_error("The active graphics backend cannot draw the sky");
            return false;
        }
        return true;
    }

    namespace detail
    {
        bool apply_pbr_camera(MeshDrawParams &params)
        {
            ActiveCameraInfo info;
            if (!active_camera_info(info))
                return false;
            const Camera &camera = info.camera;
            const glm::vec3 forward = glm::normalize(camera.target - camera.position);
            const glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));
            const glm::vec3 up = glm::cross(right, forward);
            const float tan_y = std::tan(glm::radians(camera.field_of_view) * 0.5f);
            const float aspect = static_cast<float>(info.width) / static_cast<float>(std::max(info.height, 1));
            for (int i = 0; i < 3; ++i)
            {
                params.camera_right[i] = right[i] * tan_y * aspect;
                params.camera_up[i] = up[i] * tan_y;
                params.camera_forward[i] = forward[i];
            }
            params.fog_view[2] = 1.0f / static_cast<float>(std::max(info.width, 1));
            params.fog_view[3] = 1.0f / static_cast<float>(std::max(info.height, 1));
            return true;
        }

        void apply_environment(MeshDrawParams &params, bool fog)
        {
            if (!fog || !have_environment || glm::dot(fog_beta, glm::vec3(1.0f)) <= 0.0f)
                return;
            ActiveCameraInfo info;
            if (!active_camera_info(info))
                return;
            const float tan_y = std::tan(glm::radians(info.camera.field_of_view) * 0.5f);
            const float aspect = static_cast<float>(info.width) / static_cast<float>(std::max(info.height, 1));
            for (int i = 0; i < 3; ++i)
            {
                params.fog_colour[i] = fog_colour[i];
                params.fog_beta[i] = fog_beta[i];
            }
            params.fog_view[0] = tan_y * aspect;
            params.fog_view[1] = tan_y;
            params.fog_view[2] = 1.0f / static_cast<float>(std::max(info.width, 1));
            params.fog_view[3] = 1.0f / static_cast<float>(std::max(info.height, 1));
        }
    } // namespace detail

} // namespace sl

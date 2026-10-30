#include "terrain.h"

#include "noise.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace sl
{
    namespace
    {
        constexpr double pi = 3.14159265358979323846;

        /** Longitude (0..1 across) and latitude (0..1 from north pole to south pole) of a unit direction. */
        void direction_to_map(const dvec3 &direction, double &u, double &v)
        {
            const double longitude = std::atan2(direction.z, direction.x);
            const double latitude = std::asin(std::clamp(direction.y, -1.0, 1.0));
            u = longitude / (2.0 * pi) + 0.5;
            v = 0.5 - latitude / pi;
        }
    }

    double TerrainSource::height_on_sphere(const dvec3 &direction, double radius) const
    {
        const double longitude = std::atan2(direction.z, direction.x);
        const double latitude = std::asin(std::clamp(direction.y, -1.0, 1.0));
        return height(longitude * radius, latitude * radius);
    }

    double TerrainSource::height_on_sphere_lod(const dvec3 &direction, double radius, double) const
    {
        return height_on_sphere(direction, radius);
    }

    bool TerrainSource::take_dirty_region(double &, double &, double &, double &)
    {
        return false;
    }

    // ---- FunctionSource ----

    FunctionSource::FunctionSource(Flat flat_function, double min_height, double max_height, Sphere sphere_function)
        : flat_(std::move(flat_function)), sphere_(std::move(sphere_function)), min_height_(min_height),
          max_height_(max_height)
    {
    }

    double FunctionSource::height(double x, double z) const
    {
        return flat_ ? flat_(x, z) : 0.0;
    }

    double FunctionSource::height_on_sphere(const dvec3 &direction, double radius) const
    {
        return sphere_ ? sphere_(direction, radius) : TerrainSource::height_on_sphere(direction, radius);
    }

    // ---- NoiseSource ----

    struct NoiseSource::Impl
    {
        explicit Impl(int seed) : generator(seed)
        {
            generator.set_type(FastNoiseLite::NoiseType_OpenSimplex2);
            generator.set_fractal_type(FastNoiseLite::FractalType_None);
            generator.set_frequency(1.0f);
        }
        Generator generator;
    };

    NoiseSource::NoiseSource(const NoiseParameters &parameters)
        : parameters_(parameters), impl_(std::make_shared<Impl>(parameters.seed))
    {
        parameters_.octaves = std::clamp(parameters_.octaves, 1, 16);
    }

    double NoiseSource::fractal(double x, double y, double z, bool three_d) const
    {
        const NoiseParameters &p = parameters_;
        double sum = 0.0;
        double amplitude = 1.0;
        double frequency = p.frequency;
        // Each octave is offset so the octaves do not all share a zero crossing at the origin.
        for (int octave = 0; octave < p.octaves; ++octave)
        {
            const double offset = 37.0 * octave;
            float n;
            if (three_d)
                n = impl_->generator.get(static_cast<float>(x * frequency + offset), static_cast<float>(y * frequency + offset),
                                         static_cast<float>(z * frequency + offset));
            else
                n = impl_->generator.get(static_cast<float>(x * frequency + offset), static_cast<float>(z * frequency + offset));
            double value = n;
            if (p.ridged)
                value = 1.0 - 2.0 * std::fabs(value);
            sum += value * amplitude;
            amplitude *= p.gain;
            frequency *= p.lacunarity;
        }
        double height = p.base_height + p.amplitude * sum;
        if (std::isfinite(p.floor_height))
            height = std::max(height, p.floor_height);
        return height;
    }

    double NoiseSource::height(double x, double z) const
    {
        return fractal(x, 0.0, z, false);
    }

    double NoiseSource::height_on_sphere(const dvec3 &direction, double radius) const
    {
        return fractal(direction.x * radius, direction.y * radius, direction.z * radius, true);
    }

    double NoiseSource::min_height() const
    {
        const NoiseParameters &p = parameters_;
        double total = 0.0;
        double amplitude = 1.0;
        for (int i = 0; i < p.octaves; ++i)
        {
            total += amplitude;
            amplitude *= p.gain;
        }
        double low = p.base_height - p.amplitude * total;
        if (std::isfinite(p.floor_height))
            low = std::max(low, p.floor_height);
        return low;
    }

    double NoiseSource::max_height() const
    {
        const NoiseParameters &p = parameters_;
        double total = 0.0;
        double amplitude = 1.0;
        for (int i = 0; i < p.octaves; ++i)
        {
            total += amplitude;
            amplitude *= p.gain;
        }
        double high = p.base_height + p.amplitude * total;
        if (std::isfinite(p.floor_height))
            high = std::max(high, p.floor_height);
        return high;
    }

    // ---- HeightmapSource ----

    HeightmapSource::HeightmapSource(std::vector<float> samples, int width, int depth, double world_width,
                                     double world_depth, double origin_x, double origin_z)
        : samples_(std::move(samples)), width_(std::max(width, 1)), depth_(std::max(depth, 1)),
          world_width_(std::max(world_width, 1.0e-6)), world_depth_(std::max(world_depth, 1.0e-6)), origin_x_(origin_x),
          origin_z_(origin_z)
    {
        samples_.resize(static_cast<std::size_t>(width_) * static_cast<std::size_t>(depth_), 0.0f);
        if (!samples_.empty())
        {
            const auto range = std::minmax_element(samples_.begin(), samples_.end());
            min_ = *range.first;
            max_ = *range.second;
        }
    }

    std::shared_ptr<HeightmapSource> HeightmapSource::from_bitmap(Bitmap *bitmap, double min_height, double max_height,
                                                                  double world_width, double world_depth,
                                                                  double origin_x, double origin_z, bool pack_16_bit)
    {
        if (!bitmap || bitmap->width <= 0 || bitmap->height <= 0)
            return nullptr;
        if (bitmap->pixels.empty())
            download_bitmap(bitmap);
        const std::size_t count = static_cast<std::size_t>(bitmap->width) * static_cast<std::size_t>(bitmap->height);
        if (bitmap->pixels.size() < count * 4)
            return nullptr;
        std::vector<float> samples(count);
        for (std::size_t i = 0; i < count; ++i)
        {
            const double red = bitmap->pixels[i * 4];
            double t;
            if (pack_16_bit)
                t = (red * 256.0 + bitmap->pixels[i * 4 + 1]) / 65535.0;
            else
                t = red / 255.0;
            samples[i] = static_cast<float>(min_height + t * (max_height - min_height));
        }
        return std::make_shared<HeightmapSource>(std::move(samples), bitmap->width, bitmap->height, world_width,
                                                 world_depth, origin_x, origin_z);
    }

    double HeightmapSource::sample_grid(double fx, double fz) const
    {
        fx = std::clamp(fx, 0.0, static_cast<double>(width_ - 1));
        fz = std::clamp(fz, 0.0, static_cast<double>(depth_ - 1));
        const int x0 = static_cast<int>(std::floor(fx));
        const int z0 = static_cast<int>(std::floor(fz));
        const int x1 = std::min(x0 + 1, width_ - 1);
        const int z1 = std::min(z0 + 1, depth_ - 1);
        const double tx = fx - x0;
        const double tz = fz - z0;
        auto at = [&](int x, int z) { return static_cast<double>(samples_[static_cast<std::size_t>(z) * width_ + x]); };
        const double a = at(x0, z0) * (1.0 - tx) + at(x1, z0) * tx;
        const double b = at(x0, z1) * (1.0 - tx) + at(x1, z1) * tx;
        return a * (1.0 - tz) + b * tz;
    }

    double HeightmapSource::height(double x, double z) const
    {
        std::shared_lock lock(mutex_);
        const double fx = width_ > 1 ? (x - origin_x_) / world_width_ * (width_ - 1) : 0.0;
        const double fz = depth_ > 1 ? (z - origin_z_) / world_depth_ * (depth_ - 1) : 0.0;
        return sample_grid(fx, fz);
    }

    double HeightmapSource::height_on_sphere(const dvec3 &direction, double) const
    {
        double u, v;
        direction_to_map(direction, u, v);
        std::shared_lock lock(mutex_);
        // The map wraps in longitude, so interpolate across the seam rather than clamping.
        const double fx = u * width_ - 0.5;
        const double fz = v * (depth_ - 1);
        const int x0 = static_cast<int>(std::floor(fx));
        const double tx = fx - x0;
        const int xa = ((x0 % width_) + width_) % width_;
        const int xb = (xa + 1) % width_;
        const double zc = std::clamp(fz, 0.0, static_cast<double>(depth_ - 1));
        const int z0 = static_cast<int>(std::floor(zc));
        const int z1 = std::min(z0 + 1, depth_ - 1);
        const double tz = zc - z0;
        auto at = [&](int x, int z) { return static_cast<double>(samples_[static_cast<std::size_t>(z) * width_ + x]); };
        const double a = at(xa, z0) * (1.0 - tx) + at(xb, z0) * tx;
        const double b = at(xa, z1) * (1.0 - tx) + at(xb, z1) * tx;
        return a * (1.0 - tz) + b * tz;
    }

    double HeightmapSource::min_height() const
    {
        std::shared_lock lock(mutex_);
        return min_;
    }

    double HeightmapSource::max_height() const
    {
        std::shared_lock lock(mutex_);
        return max_;
    }

    // ---- GridSource ----

    GridSource::GridSource(int width, int depth, double world_width, double world_depth, double origin_x,
                           double origin_z, float initial)
        : HeightmapSource(std::vector<float>(static_cast<std::size_t>(std::max(width, 1)) * std::max(depth, 1), initial),
                          width, depth, world_width, world_depth, origin_x, origin_z)
    {
    }

    GridSource::GridSource(std::vector<float> samples, int width, int depth, double world_width, double world_depth,
                           double origin_x, double origin_z)
        : HeightmapSource(std::move(samples), width, depth, world_width, world_depth, origin_x, origin_z)
    {
    }

    void GridSource::expand_range(float value)
    {
        min_ = std::min(min_, static_cast<double>(value));
        max_ = std::max(max_, static_cast<double>(value));
    }

    void GridSource::mark_dirty(double x0, double z0, double x1, double z1)
    {
        std::lock_guard<std::mutex> lock(dirty_mutex_);
        if (!dirty_)
        {
            dirty_x0_ = x0;
            dirty_z0_ = z0;
            dirty_x1_ = x1;
            dirty_z1_ = z1;
            dirty_ = true;
            return;
        }
        dirty_x0_ = std::min(dirty_x0_, x0);
        dirty_z0_ = std::min(dirty_z0_, z0);
        dirty_x1_ = std::max(dirty_x1_, x1);
        dirty_z1_ = std::max(dirty_z1_, z1);
    }

    bool GridSource::take_dirty_region(double &x0, double &z0, double &x1, double &z1)
    {
        std::lock_guard<std::mutex> lock(dirty_mutex_);
        if (!dirty_)
            return false;
        x0 = dirty_x0_;
        z0 = dirty_z0_;
        x1 = dirty_x1_;
        z1 = dirty_z1_;
        dirty_ = false;
        return true;
    }

    double GridSource::min_height() const
    {
        return HeightmapSource::min_height();
    }

    double GridSource::max_height() const
    {
        return HeightmapSource::max_height();
    }

    void GridSource::set_sample(int ix, int iz, float height)
    {
        if (ix < 0 || iz < 0 || ix >= width_ || iz >= depth_ || !std::isfinite(height))
            return;
        {
            std::unique_lock lock(mutex_);
            samples_[static_cast<std::size_t>(iz) * width_ + ix] = height;
            expand_range(height);
        }
        const double cx = origin_x_ + (width_ > 1 ? ix * world_width_ / (width_ - 1) : 0.0);
        const double cz = origin_z_ + (depth_ > 1 ? iz * world_depth_ / (depth_ - 1) : 0.0);
        const double sx = width_ > 1 ? world_width_ / (width_ - 1) : world_width_;
        const double sz = depth_ > 1 ? world_depth_ / (depth_ - 1) : world_depth_;
        mark_dirty(cx - sx, cz - sz, cx + sx, cz + sz);
    }

    float GridSource::sample(int ix, int iz) const
    {
        if (ix < 0 || iz < 0 || ix >= width_ || iz >= depth_)
            return 0.0f;
        std::shared_lock lock(mutex_);
        return samples_[static_cast<std::size_t>(iz) * width_ + ix];
    }

    void GridSource::raise(double x, double z, double radius, double amount, double falloff)
    {
        if (radius <= 0.0 || !std::isfinite(amount))
            return;
        const double sx = width_ > 1 ? world_width_ / (width_ - 1) : world_width_;
        const double sz = depth_ > 1 ? world_depth_ / (depth_ - 1) : world_depth_;
        const int ix0 = std::max(0, static_cast<int>(std::floor((x - radius - origin_x_) / sx)));
        const int ix1 = std::min(width_ - 1, static_cast<int>(std::ceil((x + radius - origin_x_) / sx)));
        const int iz0 = std::max(0, static_cast<int>(std::floor((z - radius - origin_z_) / sz)));
        const int iz1 = std::min(depth_ - 1, static_cast<int>(std::ceil((z + radius - origin_z_) / sz)));
        if (ix0 > ix1 || iz0 > iz1)
            return;
        {
            std::unique_lock lock(mutex_);
            for (int iz = iz0; iz <= iz1; ++iz)
                for (int ix = ix0; ix <= ix1; ++ix)
                {
                    const double dx = origin_x_ + ix * sx - x;
                    const double dz = origin_z_ + iz * sz - z;
                    const double d = std::sqrt(dx * dx + dz * dz) / radius;
                    if (d >= 1.0)
                        continue;
                    const double smooth = 1.0 - d * d * (3.0 - 2.0 * d);
                    const double weight = (1.0 - falloff) + falloff * smooth;
                    float &cell = samples_[static_cast<std::size_t>(iz) * width_ + ix];
                    cell += static_cast<float>(amount * weight);
                    expand_range(cell);
                }
        }
        mark_dirty(x - radius - sx, z - radius - sz, x + radius + sx, z + radius + sz);
    }

    void GridSource::flatten(double x, double z, double radius, double height, double strength)
    {
        if (radius <= 0.0 || !std::isfinite(height))
            return;
        strength = std::clamp(strength, 0.0, 1.0);
        const double sx = width_ > 1 ? world_width_ / (width_ - 1) : world_width_;
        const double sz = depth_ > 1 ? world_depth_ / (depth_ - 1) : world_depth_;
        const int ix0 = std::max(0, static_cast<int>(std::floor((x - radius - origin_x_) / sx)));
        const int ix1 = std::min(width_ - 1, static_cast<int>(std::ceil((x + radius - origin_x_) / sx)));
        const int iz0 = std::max(0, static_cast<int>(std::floor((z - radius - origin_z_) / sz)));
        const int iz1 = std::min(depth_ - 1, static_cast<int>(std::ceil((z + radius - origin_z_) / sz)));
        if (ix0 > ix1 || iz0 > iz1)
            return;
        {
            std::unique_lock lock(mutex_);
            for (int iz = iz0; iz <= iz1; ++iz)
                for (int ix = ix0; ix <= ix1; ++ix)
                {
                    const double dx = origin_x_ + ix * sx - x;
                    const double dz = origin_z_ + iz * sz - z;
                    const double d = std::sqrt(dx * dx + dz * dz) / radius;
                    if (d >= 1.0)
                        continue;
                    const double weight = strength * (1.0 - d * d * (3.0 - 2.0 * d));
                    float &cell = samples_[static_cast<std::size_t>(iz) * width_ + ix];
                    cell = static_cast<float>(cell + (height - cell) * weight);
                    expand_range(cell);
                }
        }
        mark_dirty(x - radius - sx, z - radius - sz, x + radius + sx, z + radius + sz);
    }

    void GridSource::smooth(double x, double z, double radius, double strength)
    {
        if (radius <= 0.0)
            return;
        strength = std::clamp(strength, 0.0, 1.0);
        const double sx = width_ > 1 ? world_width_ / (width_ - 1) : world_width_;
        const double sz = depth_ > 1 ? world_depth_ / (depth_ - 1) : world_depth_;
        const int ix0 = std::max(0, static_cast<int>(std::floor((x - radius - origin_x_) / sx)));
        const int ix1 = std::min(width_ - 1, static_cast<int>(std::ceil((x + radius - origin_x_) / sx)));
        const int iz0 = std::max(0, static_cast<int>(std::floor((z - radius - origin_z_) / sz)));
        const int iz1 = std::min(depth_ - 1, static_cast<int>(std::ceil((z + radius - origin_z_) / sz)));
        if (ix0 > ix1 || iz0 > iz1)
            return;
        {
            std::unique_lock lock(mutex_);
            const std::vector<float> before = samples_;
            auto at = [&](int cx, int cz) {
                cx = std::clamp(cx, 0, width_ - 1);
                cz = std::clamp(cz, 0, depth_ - 1);
                return before[static_cast<std::size_t>(cz) * width_ + cx];
            };
            for (int iz = iz0; iz <= iz1; ++iz)
                for (int ix = ix0; ix <= ix1; ++ix)
                {
                    const double dx = origin_x_ + ix * sx - x;
                    const double dz = origin_z_ + iz * sz - z;
                    const double d = std::sqrt(dx * dx + dz * dz) / radius;
                    if (d >= 1.0)
                        continue;
                    float average = 0.0f;
                    for (int oz = -1; oz <= 1; ++oz)
                        for (int ox = -1; ox <= 1; ++ox)
                            average += at(ix + ox, iz + oz);
                    average /= 9.0f;
                    const double weight = strength * (1.0 - d * d * (3.0 - 2.0 * d));
                    float &cell = samples_[static_cast<std::size_t>(iz) * width_ + ix];
                    cell = static_cast<float>(cell + (average - cell) * weight);
                }
        }
        mark_dirty(x - radius - sx, z - radius - sz, x + radius + sx, z + radius + sz);
    }

    std::vector<TerrainLayer> default_terrain_layers(double low_height, double high_height)
    {
        const double span = std::max(high_height - low_height, 1.0);
        std::vector<TerrainLayer> layers(4);
        TerrainLayer &sand = layers[0];
        sand.colour = {194, 178, 128, 255};
        sand.max_height = low_height + span * 0.04;
        sand.height_blend = span * 0.03;
        sand.max_slope = 25.0;
        TerrainLayer &grass = layers[1];
        grass.colour = {84, 128, 58, 255};
        grass.min_height = low_height;
        grass.max_height = low_height + span * 0.6;
        grass.height_blend = span * 0.12;
        grass.max_slope = 32.0;
        grass.slope_blend = 10.0;
        TerrainLayer &rock = layers[2];
        rock.colour = {112, 108, 104, 255};
        rock.min_slope = 28.0;
        rock.slope_blend = 10.0;
        rock.strength = 1.2;
        TerrainLayer &snow = layers[3];
        snow.colour = {240, 244, 250, 255};
        snow.min_height = low_height + span * 0.78;
        snow.height_blend = span * 0.1;
        snow.max_slope = 55.0;
        snow.slope_blend = 12.0;
        return layers;
    }

} // namespace sl

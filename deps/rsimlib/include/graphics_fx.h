#pragma once

#include "draw.h"

#include <cstdint>
#include <string>
#include <vector>

namespace sl
{
    struct Bitmap;
}

namespace sl
{

    /** A collection of colours loaded from the first row of an image. */
    class PixelPallete {
        public:
        PixelPallete() = default;
        ~PixelPallete() = default;
        /** Load and append the first image row's pixels as palette entries. */
        bool load(const std::string &filename)
        {
            Bitmap *bmp = load_bitmap(filename);
            if (!bmp)
                return false;

            int numcolours=bmp->width;
            for (int i = 0; i < numcolours; ++i)
            {
                const Colour col = getpixel(bmp,i,0);
                //fprintf(stderr, "Loaded colour: R=%d, G=%d, B=%d\n", col.red, col.green, col.blue);
                colours.emplace_back(col);
            }
            return true;
        }
        /** Return a palette colour by index, throwing std::out_of_range if invalid. */
        const Colour at(std::size_t index) const
        {
            return colours.at(index);
        }
        /** Return a mutable palette colour by index, throwing std::out_of_range if invalid. */
        Colour &operator[](std::size_t index) 
        {
            return colours.at(index);
        }
        /** Return the number of stored colours. */
        const size_t size() const
        {
            return colours.size();
        }
        private:
        std::vector<Colour> colours;
    };
    /**
     * Describes one named field inside a custom Vulkan shader's fragment
     * push-constant block, relative to the block's own start (0-based).
     * Sizes: float/int = 4, vec2 = 8, vec3 = 12, vec4/mat4's row = varies;
     * use 64 for a full mat4.
     */
    struct ShaderUniform
    {
        /** Field name as declared in the shader's push-constant block. */
        std::string name;
        /** Byte offset from the start of the block. */
        std::uint32_t offset = 0;
        /** Field size in bytes. */
        std::uint32_t size = 0;
    };

    /** Owns and applies a shader program for the active graphics backend. */
    class Shader
    {
    public:
        Shader() = default;
        /** Compile and link the supplied vertex and fragment GLSL sources. */
        Shader(const std::string &vertexSource, const std::string &fragmentSource);
        ~Shader();

        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;
        /** Transfer ownership of a compiled shader program. */
        Shader(Shader &&other) noexcept;
        /** Release this program and take ownership of another shader's program. */
        Shader &operator=(Shader &&other) noexcept;

        /** Compile and link shader source, replacing the current program. */
        bool load(const std::string &vertexSource, const std::string &fragmentSource);
        /** Compile and link a named built-in shader asset, replacing the current program. */
        bool load(const std::string &vertexSource, const std::string &fragmentSource, const std::string &assetId);
        /**
         * Compile and link an arbitrary custom shader, replacing the current program.
         * On Vulkan this compiles the supplied GLSL to SPIR-V at runtime (requires
         * glslangValidator to be installed) and builds a pipeline whose fragment
         * stage samples `vulkanSamplerNames` (in binding order 0..N-1) and exposes
         * `vulkanUniforms` as named push-constant fields settable via set_uniform.
         * The special uniform name "uProjection" is always available for a 4x4
         * matrix and does not need to be listed in `vulkanUniforms`. On OpenGL,
         * `vulkanSamplerNames` and `vulkanUniforms` are ignored.
         */
        bool load(const std::string &vertexSource, const std::string &fragmentSource,
                  const std::vector<std::string> &vulkanSamplerNames,
                  const std::vector<ShaderUniform> &vulkanUniforms);
        /** Load vertex and fragment GLSL source files and compile them. */
        bool load_files(const std::string &vertexPath, const std::string &fragmentPath,
                const std::vector<std::string> &vulkanSamplerNames = {},
                const std::vector<ShaderUniform> &vulkanUniforms = {},
                const std::string &assetId = {});
        /** Compile and link a compute shader, replacing the current program. */
        bool load_compute(const std::string &computeSource);
        /** Compile and link a named built-in compute shader asset, replacing the current program. */
        bool load_compute(const std::string &computeSource, const std::string &assetId);
        /**
         * Compile and link a compute shader that uses `layout(push_constant)` uniforms on Vulkan.
         * On Vulkan, `vulkanUniforms` maps named push-constant fields (matching a block the
         * compute source itself declares) to offsets settable via set_uniform(); the GLSL text
         * must be Vulkan-valid (no loose `uniform` globals). On OpenGL, `vulkanUniforms` is
         * ignored and plain `uniform` globals in the same source work as usual.
         */
        bool load_compute(const std::string &computeSource, const std::vector<ShaderUniform> &vulkanUniforms,
                          const std::string &assetId = {});
        /** Load and compile a compute GLSL source file. */
        bool load_compute_file(const std::string &computePath, const std::string &assetId = {});
        /** Delete the program and clear its error state. */
        void reset();

        /** Return whether a linked program is available. */
        bool is_valid() const;
        /** Return the most recent shader compilation or linking error. */
        const std::string &error() const;

        /** Bind this shader program for subsequent OpenGL calls. */
        bool use() const;
        /** Draw a textured quad with this shader using the active renderer. */
        bool draw_textured_quad(Bitmap *texture, float x, float y, float width, float height) const;
        /** Unbind the current shader program. */
        static void stop();
        /** Dispatch this program as a compute shader. */
        bool dispatch_compute(unsigned int groupsX, unsigned int groupsY, unsigned int groupsZ) const;
        /** Bind a bitmap to a shader texture unit (0..N-1). Pass nullptr to unbind. */
        bool set_texture(unsigned int unit, Bitmap *bitmap) const;
        /** Bind a bitmap to a texture unit and assign that unit index to the named sampler uniform. */
        bool set_texture(const char *samplerName, unsigned int unit, Bitmap *bitmap) const;
        /** Bind a bitmap to an image storage unit (0..N-1) for compute image load/store. Pass nullptr to unbind. */
        bool set_storage_texture(unsigned int unit, Bitmap *bitmap) const;
        /** Bind a bitmap to an image storage unit and assign that unit index to the named image uniform. */
        bool set_storage_texture(const char *samplerName, unsigned int unit, Bitmap *bitmap) const;
        /** Set an integer shader uniform. */
        bool set_uniform(const char *name, int value) const;
        /** Set a floating-point shader uniform. */
        bool set_uniform(const char *name, float value) const;
        /** Set a two-component floating-point shader uniform. */
        bool set_uniform(const char *name, float x, float y) const;
        /** Set a two-component integer shader uniform. */
        bool set_uniform(const char *name, int x, int y) const;
        /** Set a three-component floating-point shader uniform. */
        bool set_uniform(const char *name, float x, float y, float z) const;
        /** Set a 4x4 matrix shader uniform (column-major). */
        bool set_uniform_mat4(const char *name, const float *matrix4x4) const;

    private:
        std::uint32_t program_ = 0;
        std::string error_;
    };

    /** Owns a GPU Storage Buffer Object (SSBO) for compute and shader operations. */
    class StorageBuffer
    {
    public:
        StorageBuffer() = default;
        /** Allocate a GPU storage buffer with the requested capacity in bytes. */
        explicit StorageBuffer(std::size_t sizeBytes);
        ~StorageBuffer();

        StorageBuffer(const StorageBuffer &) = delete;
        StorageBuffer &operator=(const StorageBuffer &) = delete;
        /** Transfer ownership of a GPU storage buffer. */
        StorageBuffer(StorageBuffer &&other) noexcept;
        /** Release this buffer and take ownership of another buffer's resource. */
        StorageBuffer &operator=(StorageBuffer &&other) noexcept;

        /** Allocate/reallocate a GPU storage buffer of the specified size. */
        bool create(std::size_t sizeBytes);
        /** Release the GPU buffer resource. */
        void destroy();
        /** Return whether the buffer holds a valid handle. */
        bool is_valid() const;
        /** Return the buffer size in bytes. */
        std::size_t size_bytes() const;
        /** Return the backend handle. */
        std::uint32_t handle() const;

        /** Upload data from CPU memory into the GPU storage buffer. */
        bool upload(const void *data, std::size_t sizeBytes, std::size_t offsetBytes = 0);
        /** Upload a std::vector into the GPU storage buffer. */
        template <typename T>
        bool upload(const std::vector<T> &data, std::size_t offsetBytes = 0)
        {
            return upload(data.data(), data.size() * sizeof(T), offsetBytes);
        }

        /** Read back data from the GPU storage buffer into CPU memory. */
        bool readback(void *outData, std::size_t sizeBytes, std::size_t offsetBytes = 0) const;
        /** Read back data from the GPU storage buffer into a std::vector. */
        template <typename T>
        bool readback(std::vector<T> &outData, std::size_t offsetBytes = 0) const
        {
            return readback(outData.data(), outData.size() * sizeof(T), offsetBytes);
        }

        /** Bind this storage buffer to a shader binding index (0..N-1). */
        bool bind(unsigned int bindingIndex) const;

    private:
        std::uint32_t handle_ = 0;
        std::size_t sizeBytes_ = 0;
    };

    /** Ensure all memory writes from compute shaders are visible to subsequent commands. */
    void compute_barrier();

    /** Convenience helper to dispatch a compute shader for a total work count, automatically computing group counts. */
    bool dispatch_compute_for(const Shader &shader,
                             unsigned int totalItemsX, unsigned int totalItemsY = 1, unsigned int totalItemsZ = 1,
                             unsigned int localSizeX = 16, unsigned int localSizeY = 16, unsigned int localSizeZ = 1);

    /** Internal helper for chaining fullscreen post-process passes without managing one bitmap per effect. */
    class PingPongBuffer
    {
    public:
        PingPongBuffer() = default;
        ~PingPongBuffer();

        PingPongBuffer(const PingPongBuffer &) = delete;
        PingPongBuffer &operator=(const PingPongBuffer &) = delete;

        /** Allocate two render targets of the given dimensions. */
        void initialise(int width, int height);
        /** Release both render targets and clear the current chain state. */
        void shutdown();
        /** Return whether both render targets are available. */
        bool valid() const;

        /** Begin a chain with the source bitmap and return the first destination. */
        Bitmap *begin(Bitmap *source);
        /** Return the most recently completed bitmap in the chain. */
        Bitmap *source() const;
        /** Return the destination bitmap for the next effect pass. */
        Bitmap *target() const;
        /** Promote the current destination to source and rotate to the other target. */
        Bitmap *advance();

    private:
        Bitmap *buffers_[2] = {nullptr, nullptr};
        Bitmap *source_ = nullptr;
        Bitmap *current_target_ = nullptr;
        int width_ = 0;
        int height_ = 0;
        int next_index_ = 0;
    };

    /** Thresholds bright pixels, blurs them, and composites a glow over a bitmap. */
    class Bloom
    {
    public:
        Bloom() = default;
        ~Bloom();

        Bloom(const Bloom &) = delete;
        Bloom &operator=(const Bloom &) = delete;
        /** Transfer ownership of the effect's shaders and render targets. */
        Bloom(Bloom &&other) noexcept;
        /** Release this effect's resources and take ownership of another effect's. */
        Bloom &operator=(Bloom &&other) noexcept;

        /** Compile the bloom shaders and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;

        /** Set the luminance threshold that contributes to bloom. */
        void set_threshold(float threshold);
        /** Set the bloom brightness multiplier. */
        void set_intensity(float intensity);
        /** Set the blur sample spacing, in downsampled-buffer texels. */
        void set_radius(float radius);
        /**
         * Set how many times smaller the internal blur buffers are than the source
         * (e.g. 4 blurs at quarter resolution). Bigger values give a wider, softer,
         * cheaper glow; smaller values look tighter and costlier. Takes effect on
         * the next apply() call.
         */
        void set_downsample(int factor);

        /**
         * Apply a wide-area glow to a bitmap region: thresholds bright pixels,
         * downsamples, blurs with a separable Gaussian, then composites the result
         * back over the original at full resolution. Texture orientation is
         * resolved by the active backend.
         */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader brightShader_;
        Shader blurShader_;
        Shader compositeShader_;
        float threshold_ = 0.7f;
        float intensity_ = 0.9f;
        float radius_ = 1.5f;
        int downsample_ = 2;
        mutable Bitmap *blurTargetA_ = nullptr;
        mutable Bitmap *blurTargetB_ = nullptr;

        bool ensure_targets(int width, int height) const;
    };

    /** Darkens the screen edges around a soft-radius circle to draw focus toward the centre. */
    class Vignette
    {
    public:
        Vignette() = default;
        ~Vignette();

        Vignette(const Vignette &) = delete;
        Vignette &operator=(const Vignette &) = delete;
        /** Transfer ownership of the vignette shader. */
        Vignette(Vignette &&other) noexcept;
        /** Release this shader and take ownership of another vignette's. */
        Vignette &operator=(Vignette &&other) noexcept;

        /** Compile the vignette shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;

        /** Set where the darkening starts, as a fraction of the screen's half-diagonal (0-1). */
        void set_radius(float radius);
        /** Set how wide the fade from clear to fully darkened is (0-1). */
        void set_softness(float softness);
        /** Set how dark the vignette gets at its outer edge (0 = no effect, 1 = black). */
        void set_intensity(float intensity);

        /**
         * Darken a bitmap region's edges and draw the result to the current render target.
         * Texture orientation is resolved by the active backend.
         */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float radius_ = 0.75f;
        float softness_ = 0.45f;
        float intensity_ = 0.8f;
    };

    /** Applies brightness, contrast, saturation, and exposure to a bitmap. */
    class ColourAdjust
    {
    public:
        ColourAdjust() = default;
        ~ColourAdjust() = default;

        /** Compile the colour-adjustment shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set brightness offset, where zero leaves brightness unchanged. */
        void set_brightness(float value);
        /** Set contrast multiplier, where one leaves contrast unchanged. */
        void set_contrast(float value);
        /** Set saturation multiplier, where one leaves saturation unchanged. */
        void set_saturation(float value);
        /** Set exposure adjustment in stops, where zero leaves exposure unchanged. */
        void set_exposure(float value);
        /** Apply the configured colour adjustments to a bitmap region. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float brightness_ = 0.0f;
        float contrast_ = 1.0f;
        float saturation_ = 1.0f;
        float exposure_ = 0.0f;
    };

    /** Applies a separable Gaussian blur to a bitmap using two temporary targets. */
    class Blur
    {
    public:
        Blur() = default;
        ~Blur();
        Blur(const Blur &) = delete;
        Blur &operator=(const Blur &) = delete;
        /** Transfer ownership of the blur shader and temporary targets. */
        Blur(Blur &&other) noexcept;
        /** Release this blur's resources and take ownership of another blur's. */
        Blur &operator=(Blur &&other) noexcept;

        /** Compile the blur shader and prepare the effect. */
        bool initialise();
        /** Release the shader and temporary render targets. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set the blur radius in source-pixel units. */
        void set_radius(float radius);
        /** Set the number of horizontal/vertical blur passes. */
        void set_iterations(int iterations);
        /** Set the composited blur opacity. */
        void set_opacity(float opacity);
        /** Blur a bitmap region and composite the result over its source. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float radius_ = 1.5f;
        int iterations_ = 2;
        float opacity_ = 1.0f;
        mutable Bitmap *target_a_ = nullptr;
        mutable Bitmap *target_b_ = nullptr;

        bool ensure_targets(int width, int height) const;
    };

    /** Separates RGB samples toward the screen edges for a lens/damage effect. */
    class ChromaticAberration
    {
    public:
        /** Compile the chromatic-aberration shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set the normalized separation between colour channels. */
        void set_strength(float strength);
        /** Apply the effect to a bitmap region. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float strength_ = 0.004f;
    };

    /** Reduces a bitmap to configurable screen-space colour blocks. */
    class Pixelate
    {
    public:
        /** Compile the pixelation shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set the pixel-block size in source pixels. */
        void set_pixel_size(float size);
        /** Apply pixelation to a bitmap region. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float pixel_size_ = 8.0f;
    };

    /** Blurs pixels along rays from a configurable focal point. */
    class RadialBlur
    {
    public:
        /** Compile the radial-blur shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set the focal point in normalized source coordinates from 0 to 1. */
        void set_centre(float x, float y);
        /** Set the distance samples are displaced toward the focal point. */
        void set_strength(float strength);
        /** Set the number of samples used for the blur. */
        void set_samples(int samples);
        /** Apply the radial blur to a bitmap region. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float centre_x_ = 0.5f;
        float centre_y_ = 0.5f;
        float strength_ = 0.25f;
        int samples_ = 8;
    };

    /** Applies animated sinusoidal UV distortion for heat, water, or portal effects. */
    class HeatHaze
    {
    public:
        /** Compile the heat-haze shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set the maximum texture-coordinate displacement. */
        void set_strength(float strength);
        /** Set the spatial frequency of the distortion pattern. */
        void set_frequency(float frequency);
        /** Set animation time in seconds. */
        void set_time(float time);
        /** Apply the current heat-haze distortion to a bitmap region. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float strength_ = 0.008f;
        float frequency_ = 24.0f;
        float time_ = 0.0f;
    };

    /** Pushes pixels outward along an expanding ring centered on a focal point. */
    class Shockwave
    {
    public:
        /** Compile the shockwave shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set the normalized focal point; (0, 0) is top-left and (1, 1) is bottom-right. */
        void set_centre(float x, float y);
        /** Set the normalized distance of the expanding ring from its focal point. */
        void set_radius(float radius);
        /** Set the normalized thickness of the distortion ring. */
        void set_width(float width);
        /** Set the normalized outward displacement at the ring peak. */
        void set_strength(float strength);
        /** Distort a bitmap region around the configured shockwave ring. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float centre_x_ = 0.5f;
        float centre_y_ = 0.5f;
        float radius_ = 0.25f;
        float width_ = 0.08f;
        float strength_ = 0.025f;
    };

    /** A retro CRT composite combining scanlines, light curvature, and blocky pixelation. */
    class CRTFilter
    {
    public:
        /** Compile the CRT shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set the simulated pixel-block size in source pixels. */
        void set_pixel_size(float size);
        /** Set scanline darkness from zero (off) to one (strong). */
        void set_scanline_strength(float strength);
        /** Set the barrel-distortion amount; zero disables curvature. */
        void set_curvature(float curvature);
        /** Apply the CRT treatment to a bitmap region. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float pixel_size_ = 4.0f;
        float scanline_strength_ = 0.35f;
        float curvature_ = 0.18f;
    };

    /** Applies ordered (Bayer) dithering to reduce colour depth for a retro/print look. */
    class DitherFilter
    {
    public:
        /** Compile the dithering shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set the Bayer pattern block size in source pixels. */
        void set_pixel_size(float size);
        /** Set the number of output shades per channel. */
        void set_levels(float levels);
        /** Apply ordered colour dithering to a bitmap region. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float pixel_size_ = 1.0f;
        float levels_ = 4.0f;
    };

    /** Adds animated monochrome film grain over a bitmap. */
    class FilmGrain
    {
    public:
        /** Compile the film-grain shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;
        /** Set the grain intensity from 0 (off) upward. */
        void set_strength(float strength);
        /** Set the animation time in seconds. */
        void set_time(float time);
        /** Add animated monochrome grain to a bitmap region. */
        void apply(Bitmap *source, int x = 0, int y = 0, int width = 0, int height = 0) const;

    private:
        Shader shader_;
        float strength_ = 0.08f;
        float time_ = 0.0f;
    };

    /** Applies a decaying camera offset to 2D projections for impact and motion effects. */
    class ScreenShake
    {
    public:
        ScreenShake() = default;
        ~ScreenShake();
        ScreenShake(const ScreenShake &) = delete;
        ScreenShake &operator=(const ScreenShake &) = delete;

        /** Start a shake with pixel amplitude and duration in seconds. */
        void trigger(float amplitude, float duration);
        /** Advance the shake envelope by elapsed seconds. */
        void update(float delta_seconds);
        /** Stop shaking and reset the current offset. */
        void clear();
        /** Return whether the shake still has non-zero duration. */
        bool active() const;

    private:
        float amplitude_ = 0.0f;
        float duration_ = 0.0f;
        float remaining_ = 0.0f;
        float offset_x_ = 0.0f;
        float offset_y_ = 0.0f;
    };

    /** A radial or optional cone-shaped 2D light expressed in screen pixels. */
    struct Light
    {
        /** Horizontal position in screen pixels. */
        float x = 0.0f;
        /** Vertical position in screen pixels. */
        float y = 0.0f;
        /** Influence radius in screen pixels. */
        float radius = 256.0f;
        /** Light contribution multiplier. */
        float intensity = 1.0f;
        /** Softness of polygon shadow edges in screen pixels. */
        float shadow_softness = 0.0f;
        /** Direction in screen space; used when the cone angles are positive. */
        float direction_x = 0.0f;
        /** Vertical component of the spotlight direction. */
        float direction_y = -1.0f;
        /** Inner/outer cone angles in degrees. Zero angles disable spotlight falloff. */
        float inner_angle = 0.0f;
        /** Outer spotlight cone angle in degrees. */
        float outer_angle = 0.0f;
        /** RGB light colour and alpha intensity. */
        Colour colour{255, 255, 255};
    };

    /** A point in a 2D shadow-casting polygon. */
    struct ShadowPoint
    {
        /** Horizontal screen coordinate in pixels. */
        float x = 0.0f;
        /** Vertical screen coordinate in pixels. */
        float y = 0.0f;
    };

    /** A polygon whose edges can block a radial light. Vertices should be ordered around its perimeter. */
    struct ShadowCaster
    {
        /** Polygon boundary vertices in screen coordinates, ordered around the perimeter. */
        std::vector<ShadowPoint> vertices;
    };

    /** Create an axis-aligned rectangular shadow caster. */
    ShadowCaster make_rectangle_shadow_caster(float left, float top, float right, float bottom);

    /** Modulates a rendered scene with an arbitrary number of radial lights. */
    class LightingPass
    {
    public:
        /** Maximum number of lights processed with shadow casters. */
        static constexpr std::size_t max_shadow_lights = 8;
        /** Screen tile dimension used to cull lights. */
        static constexpr int tile_size = 16;
        /** Maximum lights stored for one screen tile. */
        static constexpr int max_lights_per_tile = 128;

        LightingPass() = default;
        ~LightingPass();

        LightingPass(const LightingPass &) = delete;
        LightingPass &operator=(const LightingPass &) = delete;
        /** Transfer ownership of lighting shaders and GPU buffers. */
        LightingPass(LightingPass &&other) noexcept;
        /** Release this pass's resources and take ownership of another pass's. */
        LightingPass &operator=(LightingPass &&other) noexcept;

        /** Compile the lighting shader and prepare the effect. */
        bool initialise();
        /** Release resources owned by the effect. */
        void shutdown();
        /** Return whether the effect is ready to apply. */
        bool is_valid() const;
        /** Return the most recent initialization error. */
        const std::string &error() const;

        /** Set the minimum scene illumination, from 0 (black) to 1 (full brightness). */
        void set_ambient(float ambient);

        /** Apply ambient plus radial lighting and polygon shadows to a bitmap. */
        void apply(Bitmap *source, const Light &light, int x = 0, int y = 0,
                   int width = 0, int height = 0,
                   const std::vector<ShadowCaster> &casters = {}) const;
        /** Apply an arbitrary batch of lights; only the first max_shadow_lights receive shadows. */
        void apply(Bitmap *source, const std::vector<Light> &lights, int x = 0, int y = 0,
               int width = 0, int height = 0,
               const std::vector<ShadowCaster> &casters = {}) const;

    private:
        Shader shader_;
        Shader cullShader_;
        float ambient_ = 0.2f;
        std::uint32_t lightBuffer_ = 0;
        mutable std::uint32_t tileCountsBuffer_ = 0;
        mutable std::uint32_t tileIndicesBuffer_ = 0;
        mutable std::uint32_t shadowEdgesBuffer_ = 0;
        mutable int tileCountX_ = 0;
        mutable int tileCountY_ = 0;
    };

    /** Draws a solid-colour overlay over the whole screen, useful for fades and hit-flash feedback. */
    class ScreenFade
    {
    public:
        /** Set the overlay colour, including its alpha (0 = invisible, 255 = fully opaque). */
        void set_colour(Colour colour);
        /** Return the current overlay colour. */
        Colour colour() const;

        /** Draw the overlay colour over the entire screen. No-op when the colour's alpha is zero. */
        void apply() const;

    private:
        Colour colour_{0, 0, 0, 0};
    };

} // namespace sl

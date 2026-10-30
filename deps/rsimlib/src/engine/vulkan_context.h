#pragma once

#include <SDL2/SDL_video.h>
#include <vulkan/vulkan.h>

#include "renderer.h"

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace sl::detail
{
    struct VulkanBuffer
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDeviceSize size = 0;
    };

    struct VulkanImage
    {
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkFormat format = VK_FORMAT_UNDEFINED;
        std::uint32_t mip_levels = 1;
        std::uint32_t array_layers = 1;
        std::uint32_t width = 0;
        std::uint32_t height = 0;
    };

    struct VulkanShaderModule
    {
        VkShaderModule module = VK_NULL_HANDLE;
    };

    struct VulkanDescriptorSetLayout
    {
        VkDescriptorSetLayout layout = VK_NULL_HANDLE;
    };

    struct VulkanStorageDescriptorLayout
    {
        VkDescriptorSetLayout layout = VK_NULL_HANDLE;
    };

    struct VulkanDescriptorPool
    {
        VkDescriptorPool pool = VK_NULL_HANDLE;
    };

    struct VulkanSampler
    {
        VkSampler sampler = VK_NULL_HANDLE;
    };

    struct VulkanGraphicsPipeline
    {
        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout layout = VK_NULL_HANDLE;
    };

    struct Vulkan3DVertex
    {
        float position[3];
        float colour[3];
    };

    /** Vertex layout used by the engine's mesh pipeline (see Vertex3D in surface3d.h). */
    struct Vulkan3DMeshVertex
    {
        float position[3];
        float normal[3];
        float colour[4];
        float uv[2];
        float tangent[4];
        float extra[4];
    };

    struct VulkanComputePipeline
    {
        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout layout = VK_NULL_HANDLE;
    };

    /** Owns the Vulkan instance, selected physical device, logical device, and graphics queue. */
    class VulkanContext
    {
    public:
        VulkanContext() = default;
        ~VulkanContext();

        VulkanContext(const VulkanContext &) = delete;
        VulkanContext &operator=(const VulkanContext &) = delete;

        bool initialise(SDL_Window *window, std::string &error);
        bool recreate_swapchain(int width, int height, std::string &error);
        void set_vsync_enabled(bool enabled) { vsync_enabled_ = enabled; }
        void destroy_swapchain();
        bool begin_frame(std::string &error);
        bool end_frame(std::string &error);
        // Submits the commands recorded so far without presenting, waits for them, and resumes recording.
        bool submit_pending_work(std::string &error);
        bool frame_active() const { return frame_active_; }
        bool begin_offscreen_render_pass(VkFramebuffer framebuffer, VulkanImage &image,
                                         int width, int height,
                                         std::string &error, int samples = 1);
        int clamp_msaa_samples(int requested) const;
        std::uint64_t pipelines_created() const { return pipelines_created_; }
        bool end_offscreen_render_pass(std::string &error);
        bool prepare_image_for_sampling(VulkanImage &image, std::string &error);
        /** Size in pixels of the framebuffer currently being drawn to. */
        VkExtent2D active_extent() const { return active_extent_; }
        /** Restrict 2D vertex draws to a pixel rectangle (top-left origin), or lift the restriction. */
        void set_clip(bool enabled, std::int32_t x, std::int32_t y, std::uint32_t width, std::uint32_t height)
        {
            clip_enabled_ = enabled;
            clip_rect_ = VkRect2D{{x, y}, {width, height}};
        }
        bool record_vertex_draw(VkPipeline pipeline, VkPipelineLayout layout,
                                VkBuffer vertex_buffer, VkDescriptorSet descriptor_set,
                                std::uint32_t vertex_count, PrimitiveType topology,
                                const float *projection, std::string &error);
        bool record_3d_draw(VkPipeline pipeline, VkPipelineLayout layout,
                            VkBuffer vertex_buffer, std::uint32_t vertex_count,
                            const float *mvp, std::string &error);
        /**
         * vertex_constants: 28 floats (mvp, then three normal-matrix rows with tint in w); fragment_constants: 4 floats.
         * material_descriptor (set 3, the PBR occlusion-roughness-metalness texture) is bound when not null; shaders that
         * sample it need it. shadow_descriptor (set 4, the packed-depth sun shadow map) is likewise bound when not null, as is local_shadow_descriptor (set 5, the local light shadow atlas).
         * With an instance_buffer, the pipeline must be an instanced one and it supplies instance_count instances.
         */
        bool record_mesh_draw(VkPipeline pipeline, VkPipelineLayout layout,
                              VkBuffer vertex_buffer, VkBuffer index_buffer, std::uint32_t index_count,
                              VkDescriptorSet texture_descriptor, VkDescriptorSet normal_descriptor,
                              const float *vertex_constants, const float *fragment_constants,
                              const float *environment, std::size_t environment_size,
                              std::string &error, VkBuffer instance_buffer = VK_NULL_HANDLE,
                              std::uint32_t instance_count = 1, VkDescriptorSet material_descriptor = VK_NULL_HANDLE,
                              VkDescriptorSet shadow_descriptor = VK_NULL_HANDLE,
                              VkDescriptorSet local_shadow_descriptor = VK_NULL_HANDLE);
        /** Fullscreen sky draw; sky_rows: 14 vec4 rows read from the per-draw environment block. */
        bool record_sky_draw(VkPipeline pipeline, VkPipelineLayout layout, const float *sky_rows,
                             std::string &error);
        bool clear_active_frame_3d(float red, float green, float blue, float alpha, std::string &error);
        bool clear_active_depth_3d(std::string &error);
        /**
         * Record any mesh draws queued for parallel recording into the primary command buffer, in order. Every
         * method that records commands or destroys resources calls this first, so queued draws never reorder.
         */
        bool flush_pending_draws(std::string &error);
        bool flush_pending_draws()
        {
            std::string ignored;
            return flush_pending_draws(ignored);
        }
        /** vkDeviceWaitIdle() that first records queued draws, so their resources are still valid. */
        void wait_device_idle();
        bool record_lighting_draw(VkPipeline pipeline, VkPipelineLayout layout,
                                  VkBuffer vertex_buffer, VkDescriptorSet texture_descriptor,
                                  VkDescriptorSet storage_descriptor, std::uint32_t vertex_count,
                                  const float *projection, const void *lighting_constants,
                                  std::uint32_t lighting_constant_size, std::string &error);
        bool record_postprocess_draw(VkPipeline pipeline, VkPipelineLayout layout,
                                     VkBuffer vertex_buffer, VkDescriptorSet texture_descriptor,
                                     std::uint32_t vertex_count, const float *projection,
                                     const void *constants, std::uint32_t constant_size, std::string &error);
        bool clear_active_frame(float red, float green, float blue, float alpha,
                                std::string &error);
        bool record_compute_dispatch(VkPipeline pipeline, VkPipelineLayout layout,
                                     VkDescriptorSet descriptor_set, const void *push_constants,
                                     std::uint32_t push_constant_size,
                                     std::uint32_t groups_x, std::uint32_t groups_y,
                                     std::uint32_t groups_z, std::string &error);
        bool create_buffer(VkDeviceSize size, VkBufferUsageFlags usage,
                           VkMemoryPropertyFlags properties, VulkanBuffer &result,
                           std::string &error);
        void destroy_buffer(VulkanBuffer &buffer);
        bool upload_buffer(const VulkanBuffer &buffer, const void *data, std::size_t size,
                           std::string &error);
        bool upload_image_rgba(VulkanImage &image, int width, int height,
                               const std::uint8_t *pixels, std::string &error);
        bool upload_image_compressed(VulkanImage &image, const CompressedTextureDesc &description,
                                     const std::uint8_t *data, std::string &error);
        bool compressed_format_supported(VkFormat format) const;
        bool create_texture_array_image(const TextureArrayDesc &description, VulkanImage &result,
                                        std::string &error);
        bool upload_image_array_rgba(VulkanImage &image, const TextureArrayDesc &description,
                                     const std::uint8_t *pixels, std::string &error);
        bool create_terrain_sampler(VulkanSampler &result, std::string &error);
        std::uint32_t max_texture_array_layers() const;
        std::uint32_t max_texture_size() const;
        bool download_image_rgba(VulkanImage &image, int width, int height,
                                 std::uint8_t *out_pixels, std::string &error);
        bool download_swapchain_rgba(int width, int height,
                                     std::uint8_t *out_pixels, std::string &error);
        bool create_image(int width, int height, VkFormat format, VkImageUsageFlags usage,
                          VulkanImage &result, std::string &error,
                          VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT,
                          std::uint32_t mip_levels = 1);
        void destroy_image(VulkanImage &image);
        bool record_image_upload(VulkanImage &image, VulkanBuffer &staging,
                                 const std::vector<VkBufferImageCopy> &copies, std::string &error);
        bool create_render_target_framebuffer(const VulkanImage &image, const VulkanImage &depth,
                                              int width, int height,
                                              VkFramebuffer &framebuffer, std::string &error,
                                              const VulkanImage *multisample_color = nullptr, int samples = 1);
        void destroy_framebuffer(VkFramebuffer &framebuffer);
        bool load_shader_module(const std::filesystem::path &path, VulkanShaderModule &result,
                                std::string &error);
        bool create_shader_module(const std::vector<std::uint32_t> &code,
                                  VulkanShaderModule &result, std::string &error);
        void destroy_shader_module(VulkanShaderModule &module);
        bool create_texture_descriptor_layout(VulkanDescriptorSetLayout &result, std::string &error);
        bool create_lighting_descriptor_layout(VulkanDescriptorSetLayout &result, std::string &error);
        void destroy_descriptor_set_layout(VulkanDescriptorSetLayout &layout);
        bool create_descriptor_pool(VulkanDescriptorPool &result, std::string &error);
        void destroy_descriptor_pool(VulkanDescriptorPool &pool);
        bool create_sampler(TextureFilter filter, VulkanSampler &result, std::string &error,
                            float anisotropy = 1.0f, bool mipmaps = false);
        void destroy_sampler(VulkanSampler &sampler);
        bool allocate_texture_descriptor(const VulkanDescriptorPool &pool,
                                         const VulkanDescriptorSetLayout &layout,
                                         const VulkanImage &image, const VulkanSampler &sampler,
                                         VkDescriptorSet &set, std::string &error);
        bool update_texture_descriptor(VkDescriptorSet set, const VulkanImage &image,
                                       const VulkanSampler &sampler, std::string &error);
        void free_descriptor_set(const VulkanDescriptorPool &pool, VkDescriptorSet &set);
        bool allocate_lighting_descriptor(const VulkanDescriptorPool &pool,
                                          const VulkanDescriptorSetLayout &layout,
                                          const VulkanImage *images, const VulkanSampler *samplers,
                                          VkDescriptorSet &set, std::string &error);
        bool update_lighting_descriptor(VkDescriptorSet set, const VulkanImage *images,
                                        const VulkanSampler *samplers, std::string &error);
        bool create_composite_descriptor_layout(VulkanDescriptorSetLayout &result, std::string &error);
        bool allocate_composite_descriptor(const VulkanDescriptorPool &pool,
                                           const VulkanDescriptorSetLayout &layout,
                                           const VulkanImage *images, const VulkanSampler *samplers,
                                           VkDescriptorSet &set, std::string &error);
        bool update_composite_descriptor(VkDescriptorSet set, const VulkanImage *images,
                                         const VulkanSampler *samplers, std::string &error);
        bool create_dynamic_descriptor_layout(const std::vector<VkDescriptorSetLayoutBinding> &bindings,
                                              VulkanDescriptorSetLayout &result, std::string &error);
        bool allocate_descriptor_set(const VulkanDescriptorPool &pool, const VulkanDescriptorSetLayout &layout,
                                     VkDescriptorSet &set, std::string &error);
        bool update_dynamic_descriptor_set(VkDescriptorSet set, const std::vector<VulkanImage> &images,
                                           const std::vector<VulkanSampler> &samplers, std::string &error);
        bool create_storage_descriptor_layout(VulkanStorageDescriptorLayout &result, std::string &error);
        void destroy_storage_descriptor_layout(VulkanStorageDescriptorLayout &layout);
        bool create_compute_descriptor_layout_flexible(std::uint32_t storage_buffer_count,
                                                       std::uint32_t storage_image_count,
                                                       std::uint32_t sampler_count,
                                                       VulkanStorageDescriptorLayout &result,
                                                       std::string &error);
        bool allocate_storage_descriptor(const VulkanDescriptorPool &pool,
                                         const VulkanStorageDescriptorLayout &layout,
                                         const VulkanBuffer *buffers, std::size_t count,
                                         VkDescriptorSet &set, std::string &error);
        bool allocate_compute_descriptor_set(const VulkanDescriptorPool &pool,
                                             const VulkanStorageDescriptorLayout &layout,
                                             VkDescriptorSet &set, std::string &error);
        bool update_storage_descriptor(VkDescriptorSet set, const VulkanBuffer *buffers,
                                       std::size_t count, std::string &error);
        bool download_buffer(const VulkanBuffer &buffer, void *out_data, std::size_t size,
                             std::size_t offset, std::string &error);
        bool create_imgui_descriptor_pool(VkDescriptorPool &pool, std::string &error);
        void destroy_imgui_descriptor_pool(VkDescriptorPool &pool);
        void bind_storage_descriptor(VkCommandBuffer command_buffer, VkPipelineLayout layout,
                                     VkDescriptorSet descriptor_set, VkPipelineBindPoint bind_point);
        void storage_barrier(VkCommandBuffer command_buffer);
        bool create_pipeline_layout(const VulkanDescriptorSetLayout *descriptor_layout,
                                    VkPipelineLayout &layout, std::string &error);
        void destroy_pipeline_layout(VkPipelineLayout &layout);
        bool create_graphics_pipeline(const VulkanShaderModule &vertex,
                                      const VulkanShaderModule &fragment,
                                      const VulkanDescriptorSetLayout &descriptor_layout,
                                      PrimitiveType topology,
                                      VulkanGraphicsPipeline &result, std::string &error,
                                      const VulkanStorageDescriptorLayout *storage_layout = nullptr,
                                      std::uint32_t fragment_push_constant_size = 0,
                                      bool three_dimensional = false,
                                      bool premultiplied_alpha = false,
                                      bool additive_blend = false,
                                      bool mesh_vertices = false,
                                      VkCullModeFlags cull_mode = VK_CULL_MODE_NONE,
                                      int samples = 1,
                                      bool sky = false,
                                      bool instanced = false,
                                      int blend_variant = 0,
                                      bool depth_write = true,
                                      bool wireframe = false);
        void destroy_graphics_pipeline(VulkanGraphicsPipeline &pipeline);
        bool create_compute_pipeline(const VulkanShaderModule &compute,
                                     const VulkanStorageDescriptorLayout &descriptor_layout,
                                     std::uint32_t push_constant_size,
                                     VulkanComputePipeline &result, std::string &error);
        void destroy_compute_pipeline(VulkanComputePipeline &pipeline);
        void shutdown();
        bool is_valid() const;
        VkInstance instance() const { return instance_; }
        VkPhysicalDevice physical_device() const { return physical_device_; }
        VkDevice device() const { return device_; }
        VkQueue graphics_queue() const { return graphics_queue_; }
        std::uint32_t graphics_queue_family() const { return graphics_queue_family_; }
        VkRenderPass render_pass() const { return render_pass_; }
        VkFormat render_target_format() const { return swapchain_format_; }
        VkFormat depth_format() const { return depth_format_; }
        VkCommandBuffer command_buffer() const { return command_buffer_; }
        std::uint32_t swapchain_image_count() const { return static_cast<std::uint32_t>(framebuffers_.size()); }
        /** Whether the actual chosen present mode blocks/paces on real vsync (FIFO family) rather
         * than presenting immediately (IMMEDIATE/MAILBOX), which needs a software frame limiter. */
        bool present_mode_blocks_on_vsync() const
        {
            return present_mode_ == VK_PRESENT_MODE_FIFO_KHR || present_mode_ == VK_PRESENT_MODE_FIFO_RELAXED_KHR;
        }

    private:
        VkInstance instance_ = VK_NULL_HANDLE;
        VkSurfaceKHR surface_ = VK_NULL_HANDLE;
        VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
        VkDevice device_ = VK_NULL_HANDLE;
        std::uint64_t pipelines_created_ = 0;
        VkQueue graphics_queue_ = VK_NULL_HANDLE;
        unsigned int graphics_queue_family_ = 0;
        VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
        VkPresentModeKHR present_mode_ = VK_PRESENT_MODE_FIFO_KHR;
        VkFormat swapchain_format_ = VK_FORMAT_UNDEFINED;
        VkFormat depth_format_ = VK_FORMAT_UNDEFINED;
        VkExtent2D swapchain_extent_{};
        std::vector<VkImage> swapchain_images_;
        std::vector<VkImageView> swapchain_image_views_;
        std::vector<VkImageLayout> swapchain_image_layouts_;
        VkImageLayout depth_image_layout_ = VK_IMAGE_LAYOUT_UNDEFINED;
        VkCommandPool command_pool_ = VK_NULL_HANDLE;
        VkRenderPass render_pass_ = VK_NULL_HANDLE;
        VkRenderPass resume_render_pass_ = VK_NULL_HANDLE;
        VkRenderPass offscreen_render_pass_ = VK_NULL_HANDLE;
        VkRenderPass resume_offscreen_render_pass_ = VK_NULL_HANDLE;
        struct MultisamplePasses
        {
            VkRenderPass clear = VK_NULL_HANDLE;
            VkRenderPass resume = VK_NULL_HANDLE;
        };
        std::map<int, MultisamplePasses> multisample_passes_;
        bool ensure_multisample_passes(int samples, std::string &error);
        std::vector<VkFramebuffer> framebuffers_;
        VulkanImage depth_image_{};
        VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;
        VkSemaphore image_available_ = VK_NULL_HANDLE;
        VkSemaphore render_finished_ = VK_NULL_HANDLE;
        VkFence in_flight_ = VK_NULL_HANDLE;
        std::uint32_t current_image_ = 0;
        VkExtent2D active_extent_{};
        bool clip_enabled_ = false;
        VkRect2D clip_rect_{};
        VkRenderPass active_render_pass_ = VK_NULL_HANDLE;
        VkRenderPass active_resume_render_pass_ = VK_NULL_HANDLE;
        VkFramebuffer active_framebuffer_ = VK_NULL_HANDLE;
        std::vector<VulkanBuffer> upload_staging_buffers_;
        struct RenderPassState
        {
            VkExtent2D extent{};
            VkRenderPass render_pass = VK_NULL_HANDLE;
            VkRenderPass resume_render_pass = VK_NULL_HANDLE;
            VkFramebuffer framebuffer = VK_NULL_HANDLE;
            bool offscreen = false;
            VkImage offscreen_image = VK_NULL_HANDLE;
            int samples = 1;
        };
        std::vector<RenderPassState> render_pass_stack_;
        int active_samples_ = 1;
        bool frame_active_ = false;
        // Per-draw environment (light colour, fog, sky rows) lives in a host-visible uniform buffer bound with a
        // dynamic offset as the last descriptor set of mesh and sky pipelines. The single frame fence makes
        // reusing the buffer from the start of every frame safe.
        static constexpr VkDeviceSize environment_slot_size = 3072;
        // Draws pack their blocks at 256-byte alignment (the most the spec allows for a uniform offset) into 24 MB.
        static constexpr VkDeviceSize environment_buffer_size = 24u * 1024u * 1024u;
        // A fully resolved mesh draw: everything workers need to record it without touching shared state.
        struct MeshDrawRecord
        {
            VkPipeline pipeline = VK_NULL_HANDLE;
            VkPipelineLayout layout = VK_NULL_HANDLE;
            VkBuffer vertex_buffer = VK_NULL_HANDLE;
            VkBuffer index_buffer = VK_NULL_HANDLE;
            VkBuffer instance_buffer = VK_NULL_HANDLE;
            VkDescriptorSet texture = VK_NULL_HANDLE;
            VkDescriptorSet normal = VK_NULL_HANDLE;
            VkDescriptorSet material = VK_NULL_HANDLE;
            VkDescriptorSet shadow = VK_NULL_HANDLE;
            VkDescriptorSet local_shadow = VK_NULL_HANDLE;
            std::uint32_t index_count = 0;
            std::uint32_t instance_count = 1;
            std::uint32_t environment_offset = 0;
            float vertex_constants[28];
            float fragment_constants[4];
        };
        struct RecordWorker
        {
            VkCommandPool pool = VK_NULL_HANDLE;
            std::vector<VkCommandBuffer> buffers;
            std::size_t used = 0;
        };
        void record_mesh_commands(VkCommandBuffer command, const MeshDrawRecord &draw, bool set_state);
        bool record_mesh_secondary(unsigned worker, const MeshDrawRecord *draws, std::size_t count,
                                   VkCommandBuffer &out);
        bool ensure_record_workers(unsigned count);
        void reset_record_workers();
        void destroy_record_workers();
        std::vector<MeshDrawRecord> pending_draws_;
        std::size_t direct_mesh_run_ = 0;
        std::vector<RecordWorker> record_workers_;
        bool ensure_environment_block(std::string &error);
        void destroy_environment_block();
        bool write_environment(const void *data, std::size_t bytes, std::uint32_t &dynamic_offset, std::string &error);
        VkDescriptorSetLayout environment_layout_ = VK_NULL_HANDLE;
        VkDescriptorPool environment_pool_ = VK_NULL_HANDLE;
        VkDescriptorSet environment_set_ = VK_NULL_HANDLE;
        VulkanBuffer environment_buffer_;
        std::uint8_t *environment_mapped_ = nullptr;
        std::uint32_t environment_next_ = 0; // Bytes used this frame.
        std::uint32_t environment_last_offset_ = 0;
        std::size_t environment_last_bytes_ = 0;
        bool image_available_consumed_ = false;
        bool command_buffer_recording_ = false;
        bool render_pass_active_ = false;
        bool offscreen_active_ = false;
        VkImage active_offscreen_image_ = VK_NULL_HANDLE;
        bool vsync_enabled_ = true;
        bool sampler_anisotropy_enabled_ = false;
        bool fill_mode_non_solid_enabled_ = false;
        float max_sampler_anisotropy_ = 1.0f;

        std::uint32_t find_memory_type(std::uint32_t type_filter,
                                       VkMemoryPropertyFlags properties) const;
    };
}

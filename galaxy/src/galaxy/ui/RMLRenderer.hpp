///
/// RMLRenderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_RMLRENDERER_HPP_
#define GALAXY_UI_RMLRENDERER_HPP_

#include <glad/glad.h>
#include <RmlUi/Core/RenderInterface.h>

namespace galaxy
{
	namespace ui
	{
		///
		///
		///
		enum class RMLProgramUniform
		{
			Translate,
			Transform,
			Tex,
			Count
		};

		///
		///
		///
		enum class RMLVertexAttribute
		{
			Position,
			Color0,
			TexCoord0,
			Count
		};

		///
		///
		///
		enum class RMLScissoringState
		{
			Disable,
			Scissor,
			RMLStencil
		};

		///
		///
		///
		enum class RMLProgramId
		{
			None,
			Texture = 1,
			Color   = 2,
			All     = (Texture | Color)
		};

		///
		///
		///
		struct RMLCompiledGeometryData
		{
			Rml::TextureHandle texture;
			GLuint vao;
			GLuint vbo;
			GLuint ibo;
			GLsizei draw_count;
		};

		///
		///
		///
		struct RMLProgramData
		{
			GLuint id;
			GLint uniform_locations[(size_t)RMLProgramUniform::Count];
		};

		///
		///
		///
		struct RMLShaderData
		{
			RMLProgramData program_color;
			RMLProgramData program_texture;
			GLuint shader_main_vertex;
			GLuint shader_main_fragment_color;
			GLuint shader_main_fragment_texture;
		};

		///
		///
		///
		struct RMLStencil
		{
			int func;
			int ref;
			int value_mask;
			int writemask;
			int fail;
			int pass_depth_fail;
			int pass_depth_pass;
		};

		///
		///
		///
		struct RMLGLStateBackup
		{
			bool enable_cull_face;
			bool enable_blend;
			bool enable_stencil_test;
			bool enable_scissor_test;

			int viewport[4];
			int scissor[4];

			int stencil_clear_value;
			float color_clear_value[4];

			int blend_equation_rgb;
			int blend_equation_alpha;
			int blend_src_rgb;
			int blend_dst_rgb;
			int blend_src_alpha;
			int blend_dst_alpha;

			RMLStencil stencil_front;
			RMLStencil stencil_back;
		};

		///
		/// Custom RML rendering interface.
		///
		class RMLRenderer final : public Rml::RenderInterface
		{
		public:
			///
			/// Constructor.
			///
			RMLRenderer();

			///
			/// Destructor.
			///
			virtual ~RMLRenderer();

			///
			/// Initialize rml renderer.
			///
			void init();

			///
			/// Cleanup rml renderer data.
			///
			void destroy();

			///
			/// \brief Begin isolated rendering.
			///
			/// Used for things like load screens, drawing to a framebuffer, etc.
			///
			void begin_frame();

			///
			/// Finish isolated rendering.
			///
			void end_frame();

			///
			/// Clear viewport.
			///
			void clear();

			///
			/// \brief Called by RmlUi when it wants to render geometry that the application does not wish to optimise.
			///
			/// Note that RmlUi renders everything as triangles.
			///
			/// \param vertices The geometry's vertex data.
			/// \param num_vertices The number of vertices passed to the function.
			/// \param indices The geometry's index data.
			/// \param num_indices The number of indices passed to the function. This will always be a multiple of three.
			/// \param texture The texture to be applied to the geometry. This may be nullptr, in which case the geometry is untextured.
			/// \param translation The translation to apply to the geometry.
			///
			void RenderGeometry(Rml::Vertex* vertices,
				int num_vertices,
				int* indices,
				int num_indices,
				Rml::TextureHandle texture,
				const Rml::Vector2f& translation) override;

			///
			/// Called by RmlUi when it wants to compile geometry it believes will be static for the forseeable future.
			///
			/// If supported, this should return a handle to an optimised, application-specific version of the data. If
			/// not, do not override the function or return zero; the simpler RenderGeometry() will be called instead.
			///
			/// \param vertices The geometry's vertex data.
			/// \param num_vertices The number of vertices passed to the function.
			/// \param indices The geometry's index data.
			/// \param num_indices The number of indices passed to the function. This will always be a multiple of three.
			/// \param texture The texture to be applied to the geometry. This may be nullptr, in which case the geometry is untextured.
			///
			/// \return The application-specific compiled geometry. Compiled geometry will be stored and rendered using RenderCompiledGeometry() in future
			/// calls, and released with ReleaseCompiledGeometry() when it is no longer needed.
			///
			Rml::CompiledGeometryHandle
			CompileGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture) override;

			///
			/// Called by RmlUi when it wants to render application-compiled geometry.
			///
			/// \param handle The application-specific compiled geometry to render.
			/// \param translation The translation to apply to the geometry.
			///
			void RenderCompiledGeometry(Rml::CompiledGeometryHandle handle, const Rml::Vector2f& translation) override;

			///
			/// Called by RmlUi when it wants to release application-compiled geometry.
			///
			/// \param handle The application-specific compiled geometry to release.
			///
			void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle handle) override;

			///
			/// Called by RmlUi when it wants to enable or disable scissoring to clip content.
			///
			/// \param enable True if scissoring is to enabled, false if it is to be disabled.
			///
			void EnableScissorRegion(bool enable) override;

			///
			/// Called by RmlUi when it wants to change the scissor region.
			///
			/// \param x The left-most pixel to be rendered. All pixels to the left of this should be clipped.
			/// \param y The top-most pixel to be rendered. All pixels to the top of this should be clipped.
			/// \param width The width of the scissored region. All pixels to the right of (x + width) should be clipped.
			/// \param height The height of the scissored region. All pixels to below (y + height) should be clipped.
			///
			void SetScissorRegion(int x, int y, int width, int height) override;

			///
			/// Called by RmlUi when a texture is required by the library.
			///
			/// \param texture_handle The handle to write the texture handle for the loaded texture to.
			/// \param texture_dimensions The variable to write the dimensions of the loaded texture.
			/// \param source The application-defined image source, joined with the path of the referencing document.
			///
			/// \return True if the load attempt succeeded and the handle and dimensions are valid, false if not.
			///
			[[nodiscard]] bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source) override;

			///
			/// Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
			///
			/// \param texture_handle The handle to write the texture handle for the generated texture to.
			/// \param source The raw 8-bit texture data. Each pixel is made up of four 8-bit values, indicating red, green, blue and alpha in that order.
			/// \param source_dimensions The dimensions, in pixels, of the source data.
			///
			/// \return True if the texture generation succeeded and the handle is valid, false if not.
			///
			[[nodiscard]] bool GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions) override;

			///
			/// Called by RmlUi when a loaded texture is no longer required.
			///
			/// \param texture_handle The texture handle to release.
			///
			void ReleaseTexture(Rml::TextureHandle texture_handle) override;

			///
			/// \brief Called by RmlUi when it wants the renderer to use a new transform matrix.
			///
			/// This will only be called if 'transform' properties are encountered. If no transform applies to the current element, nullptr
			/// is submitted. Then it expects the renderer to use an identity matrix or otherwise omit the multiplication with the transform.
			///
			/// \param new_transform The new transform to apply, or nullptr if no transform applies to the current element.
			///
			void SetTransform(const Rml::Matrix4f* new_transform) override;

		private:
			///
			/// Update rml transform.
			///
			/// \param program_id Shader to modify.
			/// \param uniform_location Location of uniform to modify.
			///
			void submit_transform_uniform(RMLProgramId program_id, int uniform_location);

		private:
			///
			/// Rml transformation.
			///
			Rml::Matrix4f transform;

			///
			/// Rml ortho projection.
			///
			Rml::Matrix4f projection;

			///
			/// Shader Id for updating a dirty transform.
			///
			RMLProgramId transform_dirty_state = RMLProgramId::All;

			///
			/// Is there a transform applied.
			///
			bool transform_active = false;

			///
			/// Current rml renderer scissor state.
			///
			RMLScissoringState scissoring_state = RMLScissoringState::Disable;

			///
			/// Rml shader data.
			///
			RMLShaderData shaders;

			///
			/// OpenGL state backed before rendering.
			///
			RMLGLStateBackup glstate_backup = {};
		};
	} // namespace ui
} // namespace galaxy

#endif
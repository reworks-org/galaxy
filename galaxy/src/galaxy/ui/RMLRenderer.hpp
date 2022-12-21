///
/// RMLRenderer.hpp
/// galaxy
///
/// Originally from https://github.com/mikke89/RmlUi/blob/master/Backends/RmlUi_Renderer_GL3.h
/// Code has been modified to work in galaxy.
///

#ifndef GALAXY_UI_RMLRENDERER_HPP_
#define GALAXY_UI_RMLRENDERER_HPP_

#include <RmlUi/Core/RenderInterface.h>

namespace Gfx
{
	struct ShadersData;
}

namespace galaxy
{
	namespace core
	{
		class Window;
	} // namespace core

	namespace ui
	{
		///
		/// Custom RML rendering interface.
		///
		class RMLRenderer final : public Rml::RenderInterface
		{
		public:
			///
			/// Constructor.
			///
			RMLRenderer() noexcept;

			///
			/// Destructor.
			///
			virtual ~RMLRenderer() noexcept;

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
			/// \param texture The texture handle to release.
			///
			void ReleaseTexture(Rml::TextureHandle texture) override;

			///
			/// \brief Called by RmlUi when it wants the renderer to use a new transform matrix.
			///
			/// This will only be called if 'transform' properties are encountered. If no transform applies to the current element, nullptr
			/// is submitted. Then it expects the renderer to use an identity matrix or otherwise omit the multiplication with the transform.
			///
			/// \param new_transform The new transform to apply, or nullptr if no transform applies to the current element.
			///
			void SetTransform(const Rml::Matrix4f* new_transform) override;

		public:
			///
			/// Can be passed to RenderGeometry() to enable texture rendering without changing the bound texture.
			///
			static const Rml::TextureHandle TextureEnableWithoutBinding = Rml::TextureHandle(-1);

		private:
			///
			/// Types of rml shaders.
			///
			enum class ProgramId
			{
				None,
				Texture = 1,
				Color   = 2,
				All     = (Texture | Color)
			};

			///
			/// Rml renderer scissor state.
			///
			enum class ScissoringState
			{
				Disable,
				Scissor,
				Stencil
			};

		private:
			///
			/// Update rml transform.
			///
			/// \param program_id Shader to modify.
			/// \param uniform_location Location of uniform to modify.
			///
			void SubmitTransformUniform(ProgramId program_id, int uniform_location);

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
			ProgramId transform_dirty_state;

			///
			/// Is there a transform applied.
			///
			bool transform_active;

			///
			/// Current rml renderer scissor state.
			///
			ScissoringState scissoring_state;

			///
			/// Rml shader data.
			///
			Rml::UniquePtr<Gfx::ShadersData> shaders;

			///
			/// Pointer to window.
			///
			core::Window* m_window;
		};
	} // namespace ui
} // namespace galaxy

#endif
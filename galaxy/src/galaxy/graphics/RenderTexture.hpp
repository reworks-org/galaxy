///
/// RenderTexture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERTEXTURE_HPP_
#define GALAXY_GRAPHICS_RENDERTEXTURE_HPP_

#include <glm/gtc/matrix_transform.hpp>

#include "galaxy/graphics/Framebuffer.hpp"

namespace galaxy
{
	namespace core
	{
		class Window;
	} // namespace core

	namespace graphics
	{
		///
		/// Framebuffer + Render Target.
		///
		class RenderTexture final
		{
		public:
			///
			/// Constructor.
			///
			RenderTexture() noexcept;

			///
			/// Argument constructor.
			///
			/// \param width Width of the RenderTexture.
			/// \param height Height of the RenderTexture.
			///
			RenderTexture(const int width, const int height);

			///
			/// Move constructor.
			///
			RenderTexture(RenderTexture&&) noexcept;

			///
			/// Move assignment operator.
			///
			RenderTexture& operator=(RenderTexture&&) noexcept;

			///
			/// Destructor.
			///
			~RenderTexture() noexcept = default;

			///
			/// Create the RenderTexture.
			///
			/// \param width Width of the RenderTexture.
			/// \param height Height of the RenderTexture.
			///
			void create(int width, int height);

			///
			/// Saves texture to file on disk.
			///
			/// \param file_name Path and filename to save texture to. Does not need extension (it will be ignored).
			///
			void save(std::string_view file_name);

			///
			/// Change RenderTexture size.
			///
			/// \param width Width of the RenderTexture.
			/// \param height Height of the RenderTexture.
			///
			void resize(int width, int height);

			///
			/// Activate texture context.
			///
			void bind() noexcept;

			///
			/// Deactivate texture context.
			///
			void unbind() noexcept;

			///
			/// Modify projection of render texture.
			///
			/// \param left Left point of ortho perspective.
			/// \param right Right point of ortho perspective.
			/// \param bottom Bottom point of ortho perspective.
			/// \param top Top point of ortho perspective.
			///
			void set_projection(const float left, const float right, const float bottom, const float top) noexcept;

			///
			/// Get projection.
			///
			/// \return Const reference to glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_proj() noexcept;

			///
			/// Get texture.
			///
			/// \return Const unsigned int. OpenGL texture handle.
			///
			[[nodiscard]] const unsigned int get_texture() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			RenderTexture(const RenderTexture&) = delete;

			///
			/// Copy assignment operator.
			///
			RenderTexture& operator=(const RenderTexture&) = delete;

		private:
			///
			/// Framebuffer to draw to.
			///
			Framebuffer m_framebuffer;

			///
			/// Projection.
			///
			glm::mat4 m_projection;
		};
	} // namespace graphics
} // namespace galaxy

#endif
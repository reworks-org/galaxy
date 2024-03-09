///
/// Texture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_TEXTURE_HPP_
#define GALAXY_GRAPHICS_GL_TEXTURE_HPP_

#include <span>
#include <string_view>

#include "galaxy/graphics/gl/Enums.hpp"

namespace galaxy
{
	namespace graphics
	{
		class TextureView;

		///
		/// OpenGL 2D Texture.
		///
		class Texture
		{
		  public:
			///
			/// Move constructor.
			///
			Texture(Texture&&);

			///
			/// Move assignment operator.
			///
			Texture& operator=(Texture&&);

			///
			/// Destructor.
			///
			virtual ~Texture();

			///
			/// Create a texture view.
			///
			/// \param parent Texture id of which to create a view from.
			/// \param minlevel Mipmap level.
			/// \param numlevels Number of mipmaps to include.
			/// \param minlayer Specifies the index of the first layer to include in the view.
			/// \param numlayers Specifies the number of layers to include in the view.
			///
			/// \return OpenGL texture view of this texture.
			///
			[[nodiscard]] TextureView
			make_view(const unsigned int minlevel = 0, const unsigned int numlevels = 1, const unsigned int minlayer = 0, const unsigned int numlayers = 1);

			///
			/// Activate context.
			///
			virtual void bind() = 0;

			///
			/// Deactivate context.
			///
			virtual void unbind() = 0;

			///
			/// Set texture mode.
			///
			/// \param mode I.e. GL_REPEAT, GL_CLAMP_TO_EDGE, etc.
			///
			void mode(const TextureMode mode);

			///
			/// Get texture mode.
			///
			/// \return Enum.
			///
			[[nodiscard]] TextureMode mode() const;

			///
			/// Set filter when texture is (up/down)scaled in OpenGL.
			///
			/// \param filter Filtering to use for min/mag.
			///
			void filter(const TextureFilter filter);

			///
			/// Get texture filter.
			///
			/// \return Enum.
			///
			[[nodiscard]] TextureFilter filter() const;

			///
			/// Set ansiotropic filtering level.
			///
			/// \param level 0 to 16. Must be 0/2/4/8/16.
			///
			void anisotropy(const int level);

			///
			/// Get current anisotropy level.
			///
			/// \return Integer.
			///
			[[nodiscard]] int anisotropy() const;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float width() const;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float height() const;

			///
			/// Gets opengl handle.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int id() const;

		  protected:
			///
			/// Constructor.
			///
			Texture();

		  private:
			///
			/// Copy constructor.
			///
			Texture(const Texture&) = delete;

			///
			/// Copy assignment operator.
			///
			Texture& operator=(const Texture&) = delete;

		  protected:
			///
			/// OpenGL id.
			///
			unsigned int m_id;

			///
			/// Cached width.
			///
			int m_width;

			///
			/// Cached height.
			///
			int m_height;

			///
			/// Mode.
			///
			TextureMode m_mode;

			///
			/// Filter.
			///
			TextureFilter m_filter;

			///
			/// Ansiotrophy level.
			///
			int m_anisotropy;
		};
	} // namespace graphics
} // namespace galaxy

#endif

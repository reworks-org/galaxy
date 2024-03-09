///
/// TextureView.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_TEXTUREVIEW_HPP_
#define GALAXY_GRAPHICS_GL_TEXTUREVIEW_HPP_

#include "galaxy/graphics/gl/Texture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// OpenGL 2D TextureView.
		///
		class TextureView final : public Texture
		{
		  public:
			///
			/// Args constructor.
			///
			/// \param parent Texture id of which to create a view from.
			/// \param minlevel Mipmap level.
			/// \param numlevels Number of mipmaps to include.
			/// \param minlayer Specifies the index of the first layer to include in the view.
			/// \param numlayers Specifies the number of layers to include in the view.
			///
			TextureView(const unsigned int parent,
				const unsigned int         minlevel,
				const unsigned int         numlevels,
				const unsigned int         minlayer,
				const unsigned int         numlayers);

			///
			/// Move constructor.
			///
			TextureView(TextureView&&);

			///
			/// Move assignment operator.
			///
			TextureView& operator=(TextureView&&);

			///
			/// Destructor.
			///
			virtual ~TextureView();

			///
			/// Activate context.
			///
			void bind() override;

			///
			/// Deactivate context.
			///
			void unbind() override;

		  private:
			///
			/// Constructor.
			///
			TextureView() = delete;

			///
			/// Copy constructor.
			///
			TextureView(const TextureView&) = delete;

			///
			/// Copy assignment operator.
			///
			TextureView& operator=(const TextureView&) = delete;
		};
	} // namespace graphics
} // namespace galaxy

#endif

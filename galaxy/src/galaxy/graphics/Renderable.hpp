///
/// Renderable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERABLE_HPP_
#define GALAXY_GRAPHICS_RENDERABLE_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Data to submit to renderer.
		///
		class Renderable
		{
		  public:
			///
			/// Destructor.
			///
			virtual ~Renderable() = default;

			///
			/// Number of instances to render.
			///
			/// \return Count for number of instances.
			///
			[[nodiscard]] virtual int get_instances() const = 0;

			///
			/// Get OpenGL rendering mode.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] virtual unsigned int get_mode() const = 0;

			///
			/// Get vertex array object.
			///
			/// \return VAO handle.
			///
			[[nodiscard]] virtual unsigned int get_vao() const = 0;

			///
			/// Gets OpenGL texture id.
			///
			/// \return unsigned int. 0 if no texture.
			///
			[[nodiscard]] virtual unsigned int get_texture() const = 0;

			///
			/// Get index (element) buffer count.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] virtual unsigned int get_count() const = 0;

			///
			/// Get rendering layer.
			///
			/// \return Integer. 0 is valid as a layer. So are negatives.
			///
			[[nodiscard]] virtual int get_layer() const = 0;

		  protected:
			///
			/// Constructor.
			///
			Renderable() = default;
		};
	} // namespace graphics
} // namespace galaxy

#endif

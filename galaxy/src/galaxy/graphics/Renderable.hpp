///
/// Renderable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERABLE_HPP_
#define GALAXY_GRAPHICS_RENDERABLE_HPP_

#include "galaxy/graphics/Primitives.hpp"
#include "galaxy/graphics/VertexArray.hpp"

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
			/// Move constructor.
			///
			Renderable(Renderable&&);

			///
			/// Move assignment operator.
			///
			virtual Renderable& operator=(Renderable&&);

			///
			/// Destructor.
			///
			virtual ~Renderable();

			///
			/// Gets OpenGL texture id.
			///
			/// \return unsigned int. 0 if no texture.
			///
			[[nodiscard]] unsigned int get_texture_handle() const;

			///
			/// Get rendering layer.
			///
			/// \return Integer. 0 is valid as a layer. So are negatives.
			///
			[[nodiscard]] int get_layer() const;

			///
			/// Get VAO object.
			///
			/// \return Reference to this renderable's VAO.
			///
			[[nodiscard]] virtual const graphics::VertexArray& get_vao() const = 0;

		protected:
			///
			/// Constructor.
			///
			Renderable();

		private:
			///
			/// Copy constructor.
			///
			Renderable(const Renderable&) = delete;

			///
			/// Copy assignment operator.
			///
			Renderable& operator=(const Renderable&) = delete;

		protected:
			///
			/// Opengl texture id.
			///
			unsigned int m_texture_handle;

			///
			/// Object z-level for drawing.
			///
			int m_layer;
		};
	} // namespace graphics
} // namespace galaxy

#endif
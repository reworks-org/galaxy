///
/// Renderable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERABLE_HPP_
#define GALAXY_GRAPHICS_RENDERABLE_HPP_

#include <functional>

#include "galaxy/graphics/Primitives.hpp"

namespace galaxy
{
	namespace graphics
	{
		class Renderer;

		///
		/// Data to submit to renderer.
		///
		class Renderable
		{
			friend class Renderer;

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

		protected:
			///
			/// Constructor.
			///
			Renderable();

			///
			/// Configure renderable.
			///
			virtual void configure() = 0;

			///
			/// Set primitive draw type.
			///
			/// \param type Primitive OpenGL draw type.
			///
			void set_primitive_type(const Primitives type);

		protected:
			///
			/// Vertex Array Object to bind.
			///
			unsigned int m_vao_id;

			///
			/// Texture ID.
			///
			unsigned int m_texture_id;

			///
			/// Index (element) buffer count.
			///
			int m_index_count;

			///
			/// Number of instances to render.
			///
			int m_instances;

			///
			/// Object z-level for drawing.
			///
			int m_layer;

			///
			/// Shader config function.
			///
			std::function<unsigned int(void)> m_configure_shader;

			///
			/// Shader id to sort by.
			///
			unsigned int m_shader_sort_id;

		private:
			///
			/// Copy constructor.
			///
			Renderable(const Renderable&) = delete;

			///
			/// Copy assignment operator.
			///
			Renderable& operator=(const Renderable&) = delete;

		private:
			///
			/// Type to render i.e. GL_LINES, GL_TRIANGLES, etc.
			///
			unsigned int m_type;
		};
	} // namespace graphics
} // namespace galaxy

#endif
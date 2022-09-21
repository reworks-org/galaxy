///
/// Renderable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERABLE_HPP_
#define GALAXY_GRAPHICS_RENDERABLE_HPP_

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
			Renderable(Renderable&&) noexcept;

			///
			/// Move assignment operator.
			///
			Renderable& operator=(Renderable&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Renderable() noexcept;

			///
			/// Set shader program.
			///
			/// \param program OpenGL handle.
			///
			void set_shader(const unsigned int program) noexcept;

			///
			/// Set rendering layer.
			///
			/// \param layer Z-Level layering.
			///
			void set_layer(const int layer) noexcept;

			///
			/// Get current render layer.
			///
			/// \return Integer.
			///
			[[nodiscard]] int get_layer() const noexcept;

		protected:
			///
			/// Constructor.
			///
			Renderable() noexcept;

			///
			/// Configure renderable.
			///
			virtual void configure() noexcept = 0;

			///
			/// Set Vertex Array Object to draw with.
			///
			/// \param vao OpenGL handle.
			///
			void set_vao(const unsigned int vao) noexcept;

			///
			/// Set Texture to draw with.
			///
			/// \param texture OpenGL handle.
			///
			void set_texture(const unsigned int texture) noexcept;

			///
			/// Set element index count.
			///
			/// \param count Element index count.
			///
			void set_index_count(const int count) noexcept;

			///
			/// Set instance count.
			///
			/// \param count Number of instances to render.
			///
			void set_instance_count(const int count) noexcept;

			///
			/// Set primitive draw type.
			///
			/// \param type Primitive OpenGL draw type.
			///
			void set_primitive_type(const Primitives type) noexcept;

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
			/// Vertex Array Object to bind.
			///
			unsigned int m_vao;

			///
			/// Texture ID.
			///
			unsigned int m_texture;

			///
			/// Index (element) buffer count.
			///
			int m_count;

			///
			/// Number of instances to render.
			///
			int m_instances;

			///
			/// Type to render i.e. GL_LINES, GL_TRIANGLES, etc.
			///
			unsigned int m_type;

			///
			/// Object z-level for drawing.
			///
			int m_layer;

			///
			/// Shader program handle.
			///
			unsigned int m_shader;
		};
	} // namespace graphics
} // namespace galaxy

#endif
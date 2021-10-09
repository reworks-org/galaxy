///
/// Object.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_OBJECT_HPP_
#define GALAXY_GRAPHICS_LIGHT_OBJECT_HPP_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace galaxy
{
	namespace light
	{
		///
		/// Light object to provide a "view" of the light position.
		/// For level editing or debugging purposes.
		///
		class Object final
		{
		public:
			///
			/// Constructor.
			///
			Object() noexcept;

			///
			/// Destructor.
			///
			~Object() noexcept;

			///
			/// \brief Create object verticies and transform.
			///
			/// Call this after setting m_pos.
			///
			void create();

			///
			/// Bind object.
			///
			void bind() noexcept;

			///
			/// Unbind object.
			///
			void unbind() noexcept;

			///
			/// Get transform.
			///
			/// \return Const reference to a glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_transform() const noexcept;

			///
			/// Get vertex count.
			///
			/// \return Compile-time constant integer.
			///
			const unsigned int get_count() const noexcept;

		public:
			///
			/// Position of object.
			///
			glm::vec3 m_pos;

		private:
			///
			/// GL VBO id.
			///
			unsigned int m_vbo;

			///
			/// GL VAO id.
			///
			unsigned int m_vao;

			///
			/// Object transform.
			///
			glm::mat4 m_transform;
		};
	} // namespace light
} // namespace galaxy

#endif
///
/// Skybox.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SKYBOX_HPP_
#define GALAXY_GRAPHICS_SKYBOX_HPP_

#include <array>
#include <string>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Standard cubemap style skybox implementation.
		///
		class Skybox final
		{
		public:
			///
			/// Constructor.
			///
			Skybox() noexcept;

			///
			/// Move constructor.
			///
			Skybox(Skybox&&) noexcept;

			///
			/// Move assignment operator.
			///
			Skybox& operator=(Skybox&&) noexcept;

			///
			/// Destructor.
			///
			~Skybox() noexcept;

			///
			/// Loads Skybox from file.
			///
			/// \param faces Each file represents a face. Must be in order: right, left, top, bottom, front, back.
			///
			void load(const std::array<std::string, 6>& faces);

			///
			/// Activate skybox context.
			///
			void bind() noexcept;

			///
			/// Deactivate skybox context.
			///
			void unbind() noexcept;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as int. int over unsigned for compat with float.
			///
			[[nodiscard]] const int get_width() const noexcept;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as int. int over unsigned for compat with float.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Gets internal OpenGL id.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int gl_texture() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			Skybox(const Skybox&) = delete;

			///
			/// Copy assignment operator.
			///
			Skybox& operator=(const Skybox&) = delete;

			///
			/// Create vertex data.
			///
			void create();

		private:
			///
			/// OpenGL texture handle.
			///
			unsigned int m_texture;

			///
			/// VBO handle.
			///
			unsigned int m_vbo;

			///
			/// VAO handle.
			///
			unsigned int m_vao;

			///
			/// Cached texture width.
			///
			int m_width;

			///
			/// Cached texture height.
			///
			int m_height;
		};
	} // namespace graphics
} // namespace galaxy

#endif
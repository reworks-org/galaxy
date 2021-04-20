///
/// Texture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE_TEXTURE_HPP_
#define GALAXY_GRAPHICS_TEXTURE_TEXTURE_HPP_

#include <span>

#include "galaxy/graphics/texture/BaseTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Holds an OpenGL texture information and data.
		///
		class Texture : public BaseTexture
		{
		public:
			///
			/// \brief Default constructor.
			///
			/// Remember to call load()!
			///
			Texture() noexcept;

			///
			/// Move constructor.
			///
			Texture(Texture&&) noexcept;

			///
			/// Move assignment operator.
			///
			Texture& operator=(Texture&&) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~Texture() noexcept = default;

			///
			/// \brief Loads texture from file.
			///
			/// Can throw exceptions.
			///
			/// \param file File on disk to load from.
			///
			void load(std::string_view file);

			///
			/// \brief Loads texture from memory.
			///
			/// Can throw exceptions.
			///
			/// \param buffer Memory buffer to load from. Not freed, you must free after.
			///
			void load_mem(std::span<unsigned char> buffer);

			///
			/// Loads texture from OpenGL generated id.
			///
			/// \param id ID to use from OpenGL.
			/// \param width Width of texture.
			/// \param height Height of texture.
			///
			void load(const unsigned int id, const int width, const int height) noexcept;

			///
			/// Activate texture context.
			///
			void bind() noexcept override;

			///
			/// Deactivate texture context.
			///
			void unbind() noexcept override;

			///
			/// Check if texture has been loaded.
			///
			/// \return True if load() or load_mem() has been successfully called.
			///
			[[nodiscard]] const bool is_loaded() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			Texture(const Texture&) = delete;

			///
			/// Copy assignment operator.
			///
			Texture& operator=(const Texture&) = delete;

		private:
			///
			/// Is texture loaded flag.
			///
			bool m_loaded;
		};
	} // namespace graphics
} // namespace galaxy

#endif
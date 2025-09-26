///
/// Texture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_TEXTURE_HPP_
#define GALAXY_GRAPHICS_GL_TEXTURE_HPP_

#include <string>
#include <span>

namespace galaxy
{
	///
	/// \brief Bindless OpenGL 2D Texture.
	///
	/// https://ktstephano.github.io/rendering/opengl/bindless
	///
	class Texture final
	{
	public:
		///
		/// Constructor.
		///
		Texture();

		///
		/// Move constructor.
		///
		Texture(Texture&&) noexcept;

		///
		/// Move assignment operator.
		///
		Texture& operator=(Texture&&) noexcept;

		///
		/// Destructor.
		///
		~Texture();

		///
		/// Load a texture from vfs.
		///
		/// \param file Texture in VFS to load.
		///
		/// \return True if texture loaded successfully.
		///
		[[nodiscard]]
		bool load(const std::string& file);

		///
		/// \brief Loads texture from memory.
		///
		/// Loads from standard 256byte array.
		///
		/// \param buffer Memory buffer to load from.
		///
		/// \return True if texture loaded successfully.
		///
		[[nodiscard]]
		bool load_mem(std::span<std::uint8_t> buffer);

		///
		/// Saves texture to file on disk.
		///
		/// \param file Path and filename to save texture to. Does not need extension (it will be ignored).
		///
		void save(std::string_view file);

		///
		/// \brief Bind to sampler.
		///
		/// Not required because this supports bindless textures.
		///
		void bind() const noexcept;

		///
		/// Deactivate sampler.
		///
		void unbind() const noexcept;

		///
		/// Destroy texture.
		///
		void destroy();

		///
		/// \brief Deletes texture data and configuration in OpenGL.
		///
		/// You need to call load again!
		///
		void recreate();

		///
		/// \brief Get texture width.
		///
		/// Is cached for performance.
		///
		/// \return Width as float.
		///
		[[nodiscard]]
		float width() const noexcept;

		///
		/// \brief Get texture height.
		///
		/// Is cached for performance.
		///
		/// \return Height as float.
		///
		[[nodiscard]]
		float height() const noexcept;

		///
		/// Get OpenGL texture id.
		///
		/// \return Unsigned int.
		///
		[[nodiscard]]
		unsigned int id() const noexcept;

		///
		/// Get OpenGL bindless handle.
		///
		/// \return Unsigned long long.
		///
		[[nodiscard]]
		std::uint64_t handle() const noexcept;

	private:
		///
		/// Copy constructor.
		///
		Texture(const Texture&) = delete;

		///
		/// Copy assignment operator.
		///
		Texture& operator=(const Texture&) = delete;

		///
		/// \brief Sets filter when texture is (up/down)scaled in OpenGL.
		///
		/// Includes mipmapping.
		///
		void set_filter() const noexcept;

		///
		/// \brief Set texture mode.
		///
		/// I.e. REPEAT, CLAMP_TO_EDGE, etc.
		///
		void set_mode() const noexcept;

		///
		/// Set ansiotropic filtering level.
		///
		void set_anisotropy() const noexcept;

	private:
		///
		/// OpenGL id.
		///
		unsigned int m_id;

		///
		/// Bindless handle.
		///
		std::uint64_t m_handle;

		///
		/// Texture width.
		///
		int m_width;

		///
		/// Texture height.
		///
		int m_height;
	};
} // namespace galaxy

#endif

///
/// FileSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FILESYSTEM_HPP_
#define GALAXY_FILESYSTEM_HPP_

#include <string>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Filesystem paths for various things needing to be loaded.
	///
	class FileSystem
	{
	public:
		///
		/// Destructor.
		///
		~FileSystem() noexcept = default;

	public:
		///
		/// Root directory of all files.
		///
		static std::string s_root;

		///
		/// \brief Root directory of textures.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_textures;

		///
		/// \brief Root directory of shaders.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_shaders;

		///
		/// \brief Root directory of scripts.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_scripts;

		///
		/// \brief Root directory of music.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_music;

		///
		/// \brief Root directory of sound effects.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_sfx;

		///
		/// \brief Root directory of json.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_json;

		///
		/// \brief Root directory of fonts.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_fonts;

	private:
		///
		/// Constructor.
		///
		FileSystem() = delete;
	};
}

#endif
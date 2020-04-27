///
/// FileSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FILESYSTEM_HPP_
#define GALAXY_FILESYSTEM_HPP_

#include <vector>
#include <string>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// A virtual file system can allow for using archives, file overrides, etc.
	/// Uses PhysFS under the hood.
	///
	class FileSystem final
	{
	public:
		///
		/// Constructor.
		///
		FileSystem();
		
		///
		/// Constructor, calls mount() on contents of array.
		///
		/// \param archives Array of archives to load. This can be directorys or archives.
		///
		explicit FileSystem(const std::vector<std::string>& archives);

		///
		/// Destructor.
		//
		~FileSystem() noexcept;

		///
		/// \brief Set the directory to write files to.
		///
		/// Depending on the order you mount your files in,
		/// you can overwrite files in a zip with files in a physical directory,
		/// essentially allowing for basic "mods".
		///
		/// \param dir Directory to write to.
		///
		void setWriteDir(const std::string& dir);

		///
		/// \brief Mount an archive or folder.
		///
		/// This is done by physfs so it supports the most common archive types. 
		/// You can also point to a folder.
		///
		/// \param archive Path or archive to mount.
		///
		void mount(const std::string& archive);

		///
		/// Checks to see if a file exists in the VFS.
		///
		/// \param file File to see if exists.
		///
		/// \return True if file exists.
		///
		bool has(const std::string& file);

		///
		/// \brief Read a file to a string.
		///
		/// Can throw exceptions.
		///
		/// \param file File in VFS to read.
		///
		/// \return Const std::string containing read info.
		///
		const std::string read(const std::string& file);

		///
		/// \brief Write to a file.
		///
		/// This will create the file if it does not exist.
		/// Additionally, this writes to the physical asset folder, not the zip file.
		/// You'll have to reload the gamestate or restart the game to see changes.
		///
		/// \param file File to write data to.
		/// \param data Data to write to file.
		/// \param size Size of the data* buffer.
		///
		/// \return True on success.
		///
		bool write(const std::string& file, const void* data, const unsigned int size);	
	};
}

#endif
///
/// VirtualFS.hpp
/// starlight
///
/// Created by reworks on 12/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_VIRTUALFS_HPP_
#define STARLIGHT_VIRTUALFS_HPP_

#include <vector>

typedef struct ALLEGRO_FILE ALLEGRO_FILE;

namespace sl
{
	///
	/// Represents a virtual file system.
	/// A virtual file system can allow for using archives, file overrides, etc.
	/// Uses PhysFS under the hood.
	///
	class VirtualFS final
	{
	public:
		///
		/// Constructor.
		///
		VirtualFS();
		
		///
		/// Constructor, calls mount() on contents of array.
		///
		/// \param archives Array of archives to load. This can be directorys or archives.
		///
		VirtualFS(const std::vector<std::string>& archives);

		///
		/// Destructor.
		//
		~VirtualFS();

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
		/// \brief Open a file and return as an ALLEGRO_FILE*.
		///
		/// Please note you must free this file with al_fclose()!
		///
		/// \param file The file in the VirtualFS to open.
		///
		/// \return Returns an ALLEGRO_FILE structure that you must free when done with it.
		///
		ALLEGRO_FILE* open(const std::string& file);

		///
		/// \brief Open a file and return as a std::string.
		///
		/// Please note this string is self-contained. You don't need to free anything. 
		/// Also note this only supports files that contain text!
		///
		/// \param file The file in the VirtualFS to open.
		///
		/// \return Returns a std::string. This is a plain string containing the information.
		///
		std::string openAsString(const std::string& file);

		///
		/// \brief Write to a file.
		///
		/// This will create the file if it does not exist.
		/// Additionally, this writes to the physical asset folder, not the zip file.
		/// You'll have to reload the gamestate or restart the game to see changes.
		///
		/// \param file File to write data to.
		/// \param data Data to write to file.
		///
		/// \return true on success.
		///
		bool writeToFile(const std::string& file, const char* data);
	};
}

#endif
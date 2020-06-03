///
/// Serializer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SERIALIZER_HPP_
#define GALAXY_SERIALIZER_HPP_

#include "galaxy/fs/Serializable.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Class used to serialize game data.
	///
	class Serializer final
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Serialize folder defaults to "saves/".
		///
		Serializer() noexcept;

		///
		/// Argument constructor.
		///
		/// \param folder Path to the folder containing save files to read / write from. Expected to end in '/'.
		///
		explicit Serializer(const std::string& folder) noexcept;

		///
		/// Destructor.
		///
		~Serializer() noexcept;

		///
		/// Add a pointer to a class that contains a serialize() function.
		///
		void add(Serializable* serializable) noexcept;

		///
		/// Set serialize folder.
		///
		/// \param folder Path to the folder containing save files to read / write from. Expected to end in '/'.
		///
		void setFolder(const std::string& folder) noexcept;

		///
		/// \brief Serialize all data.
		///
		/// Cannot throw exceptions.
		///
		void serialize() noexcept;

		///
		/// \brief Deserialize a save file.
		///
		/// Can throw exceptions.
		///
		void deserialize(const std::string& file);

	private:
		///
		/// Path where the save files are located.
		///
		std::string m_folder;

		///
		/// Stores pointers to serializable classes.
		///
		std::vector<Serializable*> m_objects;
	};
}

#endif
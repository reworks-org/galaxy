///
/// MusicComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MUSICCOMPONENT_HPP_
#define GALAXY_MUSICCOMPONENT_HPP_

#include <SFML/Audio/Music.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Component representing a piece of music.
	///
	class MusicComponent final
	{
	public:
		///
		/// Default constructor.
		///
		MusicComponent() noexcept = default;

		///
		/// Argument constructor.
		///
		/// \param file Name of the file in the VFS.
		///
		explicit MusicComponent(const std::string& file) noexcept;

		///
		/// Default destructor.
		///
		~MusicComponent() noexcept = default;

		///
		/// Load music file.
		///
		/// \param file Name of the file in the VFS.
		///
		void load(const std::string& file) noexcept;

	public:
		///
		/// The music file.
		///
		sf::Music m_music;
	};
}

#endif
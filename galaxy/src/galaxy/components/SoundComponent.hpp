///
/// SoundComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SOUNDCOMPONENT_HPP_
#define GALAXY_SOUNDCOMPONENT_HPP_

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Component representing a piece of music.
	///
	class SoundComponent final
	{
	public:
		///
		/// Default constructor.
		///
		SoundComponent() noexcept = default;

		///
		/// Argument constructor.
		///
		/// \param file Name of the file in the VFS.
		///
		explicit SoundComponent(const std::string& file) noexcept;

		///
		/// Default destructor.
		///
		~SoundComponent() noexcept = default;

		///
		/// Load sound file.
		///
		/// \param file Name of the file in the VFS.
		///
		void load(const std::string& file) noexcept;

	public:
		///
		/// Control / manipulate sound buffer.
		///
		sf::Sound m_sound;

	private:
		///
		/// Stores the sound data.
		///
		sf::SoundBuffer m_buffer;
	};
}

#endif
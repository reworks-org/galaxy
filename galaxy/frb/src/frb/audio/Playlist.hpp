///
/// Playlist.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_PLAYLIST_HPP_
#define FRB_PLAYLIST_HPP_

#include "frb/audio/Music.hpp"
#include "frb/audio/Sound.hpp"

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// Concept to ensure template is using an frb audio object.
	///
	template<typename Type>
	concept frb_audio = ((std::is_same<frb::Sound, Type>::value) || (std::is_same<frb::Music, Type>::value));

	///
	/// \brief Queued sound effects / music.
	///
	/// A source with queued buffers.
	///
	template<frb_audio Audio>
	class Playlist final
	{
	public:
		///
		/// Constructor.
		///
		Playlist() noexcept;

		///
		/// Destructor.
		///
		~Playlist() noexcept;

	private:
	};

	template<frb_audio Audio>
	inline Playlist<Audio>::Playlist() noexcept
	{
	}

	template<frb_audio Audio>
	inline Playlist<Audio>::~Playlist() noexcept
	{
	}

} // namespace frb

#endif
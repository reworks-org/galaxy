///
/// Playlist.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_PLAYLIST_HPP_
#define FRB_PLAYLIST_HPP_

#include <algorithm>
#include <concepts>
#include <mutex>
#include <random>
#include <string_view>

#include <jthread/jthread.hpp>

#include "frb/audio/Music.hpp"
#include "frb/audio/Sound.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// Concept to ensure only music or sound is used.
	///
	template<typename Type>
	concept is_audio = (std::is_same<Music, Type>::value || std::is_same<Sound, Type>::value);

	///
	/// Plays a series of audio files.
	///
	template<is_audio Audio>
	class Playlist
	{
	public:
		///
		/// Constructor.
		///
		Playlist();

		///
		/// Copy constructor.
		///
		Playlist(const Playlist&) = delete;

		///
		/// Move constructor.
		///
		Playlist(Playlist&&);

		///
		/// Copy assignment operator.
		///
		Playlist& operator=(const Playlist&) = delete;

		///
		/// Move assignment operator.
		///
		Playlist& operator=(Playlist&&);

		///
		/// Destructor.
		///
		~Playlist();

		///
		/// Add a piece of audio to the playlist.
		///
		void add(std::string_view file);

		///
		/// Shuffles music in playlist.
		///
		void shuffle();

		///
		/// \brief Set looping status.
		///
		/// Defaults to false.
		///
		void toggle_loop() noexcept;

		///
		/// \brief Play playlist.
		///
		/// Plays from current index.
		///
		void play();

		///
		/// \brief Pause playlist.
		///
		/// Keeps current index.
		///
		void pause();

		///
		/// \brief Stop playlist.
		///
		/// Sets index back to 0.
		///
		void stop();

	private:
		///
		/// Playlist containing music to loop through.
		///
		std::vector<Audio> m_playlist;

		///
		/// Current index.
		///
		std::size_t m_index;

		///
		/// Loop when the end of the audio is reached.
		///
		bool m_loop;

		///
		/// Is currently playing?
		///
		bool m_playing;

		///
		/// Protection mutex.
		///
		std::mutex m_mutex;

		///
		/// Update thread.
		///
		std::jthread m_thread;

		///
		/// Thread loop control.
		///
		std::atomic_bool m_run_loop;
	};

	template<is_audio Audio>
	inline Playlist<Audio>::Playlist()
	    : m_index {0}, m_loop {false}, m_playing {false}, m_run_loop {true}
	{
		// clang-format off
		
		m_thread = std::jthread([&]()
		{
			while (m_run_loop)
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				if (!m_playlist.empty())
				{
					if (m_playlist[m_index].get_state() == AL_STOPPED)
					{
						if (m_playing)
						{
							m_index++;
						}
					}
				}
			}
		});

		// clang-format on
	}

	template<is_audio Audio>
	inline Playlist<Audio>::Playlist(Playlist<Audio>&& p)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (p.m_playing)
		{
			p.pause();

			this->m_playlist = std::move(p.m_playlist);
			this->m_index    = p.m_index;
			this->m_loop     = p.m_loop;
			this->m_playing  = p.m_playing;
			this->m_mutex    = std::move(p.m_mutex);
			this->m_thread   = std::move(p.m_thread);
			this->m_run_loop = std::move(m_run_loop);

			p.m_playlist.clear();

			this->play();
		}
		else
		{
			this->m_playlist = std::move(p.m_playlist);
			this->m_index    = p.m_index;
			this->m_loop     = p.m_loop;
			this->m_playing  = p.m_playing;
			this->m_mutex    = std::move(p.m_mutex);
			this->m_thread   = std::move(p.m_thread);
			this->m_run_loop = std::move(m_run_loop);

			p.m_playlist.clear();
		}
	}

	template<is_audio Audio>
	inline Playlist<Audio>& Playlist<Audio>::operator=(Playlist<Audio>&& p)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (this != &p)
		{
			if (p.m_playing)
			{
				p.pause();

				this->m_playlist = std::move(p.m_playlist);
				this->m_index    = p.m_index;
				this->m_loop     = p.m_loop;
				this->m_playing  = p.m_playing;
				this->m_mutex    = std::move(p.m_mutex);
				this->m_thread   = std::move(p.m_thread);
				this->m_run_loop = std::move(m_run_loop);

				p.m_playlist.clear();

				this->play();
			}
			else
			{
				this->m_playlist = std::move(p.m_playlist);
				this->m_index    = p.m_index;
				this->m_loop     = p.m_loop;
				this->m_playing  = p.m_playing;
				this->m_mutex    = std::move(p.m_mutex);
				this->m_thread   = std::move(p.m_thread);
				this->m_run_loop = std::move(m_run_loop);

				p.m_playlist.clear();
			}
		}

		return *this;
	}

	template<is_audio Audio>
	inline Playlist<Audio>::~Playlist()
	{
		{
			std::lock_guard<std::mutex> lock {m_mutex};
			if (m_playing)
			{
				stop();
			}
		}

		m_run_loop = false;
		m_thread.request_stop();
		m_thread.join();

		m_playlist.clear();
	}

	template<is_audio Audio>
	inline void Playlist<Audio>::add(std::string_view file)
	{
		std::lock_guard<std::mutex> lock {m_mutex};
		m_playlist.emplace_back(file);
	}

	template<is_audio Audio>
	inline void Playlist<Audio>::shuffle()
	{
		std::random_device rd;
		std::mt19937_64 mt {rd()};

		if (m_playing)
		{
			stop();

			{
				std::lock_guard<std::mutex> lock {m_mutex};
				std::shuffle(m_playlist.begin(), m_playlist.end(), mt);
			}

			play();
		}
		else
		{
			std::lock_guard<std::mutex> lock {m_mutex};
			std::shuffle(m_playlist.begin(), m_playlist.end(), mt);
		}
	}

	template<is_audio Audio>
	inline void Playlist<Audio>::toggle_loop() noexcept
	{
		m_loop = !m_loop;
	}

	template<is_audio Audio>
	inline void Playlist<Audio>::play()
	{
		std::lock_guard<std::mutex> lock {m_mutex};

		if (!m_playlist.empty())
		{
			m_playlist[m_index].play();
			m_playing = true;
		}
	}

	template<is_audio Audio>
	inline void Playlist<Audio>::pause()
	{
		std::lock_guard<std::mutex> lock {m_mutex};

		if (!m_playlist.empty())
		{
			m_playlist[m_index].pause();
			m_playing = false;
		}
	}

	template<is_audio Audio>
	inline void Playlist<Audio>::stop()
	{
		std::lock_guard<std::mutex> lock {m_mutex};

		if (!m_playlist.empty())
		{
			m_playlist[m_index].stop();
			m_index   = 0;
			m_playing = false;
		}
	}
} // namespace frb

#endif
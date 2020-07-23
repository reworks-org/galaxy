///
/// Music.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_MUSIC_HPP_
#define FRB_MUSIC_HPP_

#include <jthread/jthread.hpp>

#include "frb/detail/Source.hpp"
#include "frb/detail/SourceManipulator.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// \brief Streamed audio source.
	///
	class Music final : public BufferStream, public SourceManipulator
	{
	public:
		///
		/// Constructor.
		///
		Music();

		///
		/// Destructor.
		///
		~Music() noexcept;

		///
		/// Load a file to stream from disk.
		///
		/// \param file File to load from disk. Can only load ogg vorbis.
		///
		/// \return False if load failed.
		///
		bool load(std::string_view file);

		///
		/// Destroy music.
		///
		void destroy() noexcept;

	private:
		///
		/// Update stream buffers as it plays.
		///
		void update() noexcept;

		///
		/// OpenAL audio source object.
		///
		Source m_source;

		///
		/// Thread to process music updates on.
		///
		std::jthread m_thread;

		///
		/// Mutex to help with synchronization.
		///
		std::mutex m_mutex;

		///
		/// Makes sure thread exits cleanly.
		///
		std::atomic_bool m_run_loop;
	};
} // namespace frb

#endif
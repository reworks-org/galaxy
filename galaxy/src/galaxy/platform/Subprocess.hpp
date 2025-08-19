///
/// Subprocess.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_SUBPROCESS_HPP_
#define GALAXY_PLATFORM_SUBPROCESS_HPP_

#include <span>
#include <string_view>

#include <SDL3/SDL_process.h>

namespace galaxy
{
	///
	/// Manages a subprocess launched by galaxy.
	///
	class Subprocess final
	{
	public:
		///
		/// Constructor.
		///
		Subprocess() noexcept;

		///
		/// Create constructor.
		///
		/// \param process Subprocess to launch. Can include a path.
		/// \param args Command-line args to set for process. Optional.
		///
		Subprocess(std::string_view process, std::span<std::string> args = {});

		///
		/// \brief Destructor.
		///
		/// Calls terminate().
		///
		~Subprocess() noexcept;

		///
		/// Launch a subprocess.
		///
		/// \param process Subprocess to launch. Can include a path.
		/// \param args Command-line args to set for process. Optional.
		///
		void create(std::string_view process, std::span<std::string> args = {});

		///
		/// Wait for a process to finish execution.
		///
		/// \param block Should the main app be blocked until the subprocess exits?
		///
		/// \return Returns True if process exited, otherwise false.
		///
		[[maybe_unused]]
		bool wait(const bool block) noexcept;

		///
		/// Terminates process.
		///
		void kill() noexcept;

	private:
		///
		/// Process information and handles.
		///
		SDL_Process* m_process;
	};
} // namespace galaxy

#endif

///
/// Subprocess.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_SUBPROCESS_HPP_
#define GALAXY_PLATFORM_SUBPROCESS_HPP_

#include <span>
#include <string>
#include <string_view>

#include <subprocess.h>

///
/// Typedef subprocess struct.
///
typedef struct subprocess_s subprocess;

namespace galaxy
{
	namespace platform
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
			/// \return Process exit code. -1 on error.
			///
			[[maybe_unused]]
			int join() noexcept;

			///
			/// Terminate process, killing if alive.
			///
			void terminate() noexcept;

			///
			/// Destroy process, preserving if alive.
			///
			void destroy() noexcept;

			///
			/// Check if subprocess is still alive and executing.
			///
			/// \return True if process is alive.
			///
			[[nodiscard]]
			bool alive() noexcept;

		private:
			///
			/// Process information and handles.
			///
			subprocess m_process;
		};
	} // namespace platform
} // namespace galaxy

#endif

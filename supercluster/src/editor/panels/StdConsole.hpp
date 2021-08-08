///
/// StdConsole.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_STDCONSOLE_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_STDCONSOLE_HPP_

#include <sstream>
#include <vector>

namespace sc
{
	namespace panel
	{
		// Thanks to: https://codereview.stackexchange.com/q/185490
		class StdConsoleStream final : public std::stringbuf
		{
		protected:
			[[maybe_unused]] std::streamsize xsputn(const char* s, std::streamsize n) override;
			[[maybe_unused]] int overflow(int c) override;
			[[maybe_unused]] int sync() override;

		public:
			std::vector<std::string> m_history;

		private:
			std::string m_buffer;
		};

		class StdConsole final
		{
		public:
			StdConsole() noexcept;
			~StdConsole() noexcept = default;

			void render();

			[[nodiscard]] std::ostream& get_stream() noexcept;

		private:
			std::ostream m_stream;
			StdConsoleStream m_streambuf;
		};

	} // namespace panel
} // namespace sc

#endif
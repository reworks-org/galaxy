///
/// Subprocess.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "Subprocess.hpp"

namespace galaxy
{
	namespace platform
	{
		Subprocess::Subprocess()
			: m_process {nullptr}
		{
		}

		Subprocess::Subprocess(std::string_view process, std::span<std::string> args)
			: m_process {nullptr}
		{
			create(process, args);
		}

		Subprocess::~Subprocess()
		{
			terminate();
		}

		void Subprocess::create(std::string_view process, std::span<std::string> args)
		{
			const auto sp_to_run = std::filesystem::absolute(process).replace_extension("").string();

			meta::vector<const char*> cmd_line;
			cmd_line.reserve(args.size() + 2);

			cmd_line.push_back(sp_to_run.c_str());
			for (const auto& arg : args)
			{
				cmd_line.push_back(arg.c_str());
			}
			cmd_line.push_back(nullptr);

			if (subprocess_create(cmd_line.data(), subprocess_option_inherit_environment, &m_process) != 0)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to launch subprocess: {0}.", process);
			}
		}

		int Subprocess::join()
		{
			if (alive())
			{
				int code = -1;
				if (subprocess_join(&m_process, &code) != 0)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to join subprocess.");
				}

				return code;
			}

			return -1;
		}

		void Subprocess::terminate()
		{
			if (alive())
			{
				if (subprocess_terminate(&m_process) != 0)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to terminate subprocess.");
				}
			}
		}

		void Subprocess::destroy()
		{
			if (alive())
			{
				if (subprocess_destroy(&m_process) != 0)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to destroy subprocess.");
				}
			}
		}

		bool Subprocess::alive()
		{
			return subprocess_alive(&m_process) != 0 ? true : false;
		}
	} // namespace platform
} // namespace galaxy

///
/// Subprocess.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/logging/Log.hpp"

#include "Subprocess.hpp"

namespace galaxy
{
	Subprocess::Subprocess() noexcept
		: m_process {nullptr}
	{
	}

	Subprocess::Subprocess(std::string_view process, std::span<std::string> args)
		: m_process {nullptr}
	{
		create(process, args);
	}

	Subprocess::~Subprocess() noexcept
	{
		kill();
	}

	void Subprocess::create(std::string_view process, std::span<std::string> args)
	{
		const auto sp_to_run = std::filesystem::absolute(process).replace_extension("").string();

		std::vector<const char*> cmd_line;
		cmd_line.reserve(args.size() + 2);

		cmd_line.push_back(sp_to_run.c_str());
		for (const auto& arg : args)
		{
			cmd_line.push_back(arg.c_str());
		}
		cmd_line.push_back(nullptr);

		m_process = SDL_CreateProcess(cmd_line.data(), false);
		if (!m_process)
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to launch subprocess: {0}.", process);
		}
	}

	bool Subprocess::wait(const bool block) noexcept
	{
		return SDL_WaitProcess(m_process, block, nullptr);
	}

	void Subprocess::kill() noexcept
	{
		if (m_process)
		{
			SDL_KillProcess(m_process, false);
			SDL_DestroyProcess(m_process);

			m_process = nullptr;
		}
	}
} // namespace galaxy

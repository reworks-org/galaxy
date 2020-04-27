///
/// UI.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "UI.hpp"

///
/// Core namespace.
///
namespace celestial
{
	UI::UI() noexcept
	{
		m_mainLoop.set([&](protostar::ProtectedBool* threadPoolFinished)
		{
			while (m_running.get() && threadPoolFinished->get())
			{
				event();
				update();
			}
		});
	}

	UI::~UI() noexcept
	{
		close();
		m_mainLoop.waitUntilFinished();
	}

	void UI::close() noexcept
	{
		m_running.set(false);
	}

	protostar::Task* UI::getTask() noexcept
	{
		return &m_mainLoop;
	}

	const celestial::ErrorState UI::getError() noexcept
	{
		std::lock_guard<std::mutex> lock(m_ioMutex);
		return m_errorState;
	}
}
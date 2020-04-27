///
/// UI.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_UI_HPP_
#define CELESTIAL_UI_HPP_

#include <mutex>
#include <atomic>

#include <protostar/async/Task.hpp>

#include "celestial/UITheme.hpp"

namespace celestial
{
	class UI final
	{
	public:
		UI() noexcept;
		~UI() noexcept;

		//void setDTPtr(const double* deltaTime) noexcept;
		//void sendEvents() noexcept;

		void close() noexcept;

		protostar::Task* getTask() noexcept;
		const celestial::ErrorState getError() noexcept;


		//void render() noexcept;

	private:
		//void event() noexcept;
		//void update() noexcept;

	private:
		protostar::ProtectedBool m_running;

		protostar::Task m_mainLoop;
		celestial::ErrorState m_errorState;
		std::mutex m_ioMutex;
		std::mutex m_loopMutex;
	};
}

#endif
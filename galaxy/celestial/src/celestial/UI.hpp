///
/// UI.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_UI_HPP_
#define CELESTIAL_UI_HPP_

#include <protostar/async/Task.hpp>
#include <starlight/Dispatcher.hpp>

#include "celestial/Widget.hpp"

namespace celestial
{
	class UI final
	{
	public:
		UI(protostar::ProtectedDouble* deltaTime) noexcept;
		~UI() noexcept;

		void render(qs::Renderer& renderer) noexcept;

		template<typename WidgetType, typename... Args>
		WidgetType* add(Args&&... args) noexcept;

		///
		/// Registers a function to be called on the triggering of an event.
		///
		/// \param callback void function that takes a const Event&.
		///
		template<typename Event, typename WidgetType>
		void addEventToWidget(starlight::Callback<Event>&& func, WidgetType* widget) noexcept;

		///
		/// Queues an event to be triggered, does not trigger immediately.
		///
		/// \param args Constructor arguments for event.
		///
		template<typename Event, typename ...Args>
		void queue(Args&&... args) noexcept;

		virtual void setVisibility(const bool isVisible) noexcept final;

		protostar::Task* getTask() noexcept;
		//const celestial::ErrorState getError() noexcept;

		void remove(const unsigned int id) noexcept;

		void destroy() noexcept;

	private:
		UI() noexcept = default;
		void processEvents() noexcept;
		void update(protostar::ProtectedDouble* deltaTime) noexcept;

	private:
		bool m_isDestroyed;
		unsigned int m_counter;
		std::vector<unsigned int> m_free;
		std::vector<celestial::WidgetPtr> m_widgets;

		protostar::ProtectedBool m_visible;
		protostar::ProtectedBool m_running;

		protostar::Task m_mainLoop;
		celestial::ErrorState m_errorState;

		std::mutex m_widgetMutex;
		std::mutex m_eventMutex;

		protostar::ProtectedDouble* m_dt;

		starlight::Dispatcher m_uiEventManager;
	};

	template<typename WidgetType, typename ...Args>
	inline WidgetType* UI::add(Args&& ...args) noexcept
	{
		// Default to 0.
		// Avoids random value assignment by compiler.
		unsigned int idToUse = 0;

		if (!m_free.empty())
		{
			idToUse = m_free[0];

			m_free.erase(m_free.begin());
			m_free.shrink_to_fit();
		}
		else
		{
			idToUse = m_counter;

			// Increment counter for next widget.
			++m_counter;
		}

		// Protect m_widgets.
		std::lock_guard<std::mutex> lock(m_widgetMutex);

		// Make sure widget is the right size.
		if (m_widgets.size() >= idToUse)
		{
			m_widgets.resize(idToUse);
		}

		// Forward arguments to std::vector's construct in place method.
		m_widgets.emplace(idToUse, std::make_unique<WidgetType>(std::forward<Args>(args)...));

		// Set ID.
		Widget* ref = m_widgets[idToUse].get();
		ref->m_id = idToUse;

		// Then return a pointer to object placed.
		return dynamic_cast<WidgetType*>(ref);
	}

	template<typename Event, typename WidgetType>
	inline void UI::addEventToWidget(starlight::Callback<Event>&& func, WidgetType* widget) noexcept
	{
		m_uiEventManager.add<Event>(std::bind(func, widget, std::placeholders::_1));
	}

	template<typename Event, typename ...Args>
	inline void UI::queue(Args&& ...args) noexcept
	{
		std::lock_guard<std::mutex> lock(m_eventMutex);
		m_uiEventManager.queue<Event>(std::forward<Args>(args)...);
	}
}

#endif
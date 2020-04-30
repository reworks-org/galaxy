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

///
/// Core namespace.
///
namespace celestial
{
	class UI final
	{
	public:
		///
		/// Constructor.
		///
		/// \param deltaTime A pointer to a protected double to update widgets with.
		///
		UI(protostar::ProtectedDouble* deltaTime) noexcept;

		///
		/// Destructor.
		///
		~UI() noexcept;

		///
		/// \brief Render UI.
		///
		/// \param renderer Renderer to use to draw widgets with.
		/// \param shader Shader to use when drawing widgets.
		///
		void render(qs::Renderer& renderer, qs::Shader& shader) noexcept;

		///
		/// Add a widget to the UI.
		///
		/// \param args Constructor arguments for WidgetType.
		///
		/// \return Pointer to newly created widget.
		///
		template<typename WidgetType, typename... Args>
		WidgetType* add(Args&&... args) noexcept;

		///
		/// Registers a function to be called on the triggering of an event.
		///
		/// \param func void function that takes a const Event&.
		/// \param widget Widget to add function to.
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

		///
		/// \brief Sets visibility of UI.
		///
		/// Note: UI will not process events/updates to widgets while not visible.
		///
		/// \param isVisible False to make invisible.
		///
		virtual void setVisibility(const bool isVisible) noexcept final;

		///
		/// Get pointer to internal TaskPool task.
		///
		/// \return Pointer to a task.
		///
		protostar::Task* getTask() noexcept;

		///
		/// Remove a widget.
		///
		/// \param id Widget id (widget->id()).
		///
		void remove(const unsigned int id) noexcept;

		///
		/// Destroy all widgets and clean up UI.
		///
		void destroy() noexcept;

	private:
		///
		/// Default constructor.
		///
		UI() noexcept = delete;

		///
		/// Internal function to process events to widgets across threads.
		///
		void processEvents() noexcept;

		///
		/// Internal update function to update widgets on another thread.
		///
		/// \param deltaTime Pointer to protected delta time.
		///
		void update(protostar::ProtectedDouble* deltaTime) noexcept;

	private:
		///
		/// Flag to mark UI as destroyed.
		///
		bool m_isDestroyed;

		///
		/// Widget ID counter.
		///
		unsigned int m_counter;

		///
		/// Holds list of free widget ids.
		///
		std::vector<unsigned int> m_free;

		///
		/// Holds list of widget pointers.
		///
		std::vector<celestial::WidgetPtr> m_widgets;

		///
		/// Protected visibility bool.
		///
		protostar::ProtectedBool m_visible;

		///
		/// Protected thread loop running bool.
		///
		protostar::ProtectedBool m_running;

		///
		/// Task that runs main UI loop on another thread.
		///
		protostar::Task m_mainLoop;

		///
		/// Mutex protecting widget access.
		///
		std::mutex m_widgetMutex;
		
		///
		/// Mutex protecting event access.
		///
		std::mutex m_eventMutex;

		///
		/// Pointer to delta time.
		///
		protostar::ProtectedDouble* m_dt;

		///
		/// Internal event manager to UI.
		///
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
		std::lock_guard<std::mutex> l_lock(m_widgetMutex);

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
		std::lock_guard<std::mutex> l_lock(m_eventMutex);
		m_uiEventManager.queue<Event>(std::forward<Args>(args)...);
	}
}

#endif
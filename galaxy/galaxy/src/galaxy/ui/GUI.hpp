///
/// GUI.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GUI_HPP_
#define GALAXY_GUI_HPP_

#include <starlight/Dispatcher.hpp>
#include <protostar/async/ThreadPool.hpp>

#include "galaxy/ui/Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Manages the GUI and a group of widgets for that GUI.
	///
	class GUI final
	{
	public:
		///
		/// Constructor.
		///
		GUI() noexcept;

		///
		/// Destructor.
		///
		~GUI() noexcept;

		///
		/// Sets up GUI thread, etc.
		///
		/// \param dt A pointer to delta time for update loops.
		/// \param pool Pool to assign GUI to.
		///
		void construct(protostar::ProtectedDouble* dt, protostar::ThreadPool* pool) noexcept;

		///
		/// \brief Render GUI.
		///
		/// This function should be called on the main thread.
		///
		/// \param camera Camera projection to apply to GUI.
		/// \param shader ID of shader in ShaderBook.
		///
		void render(qs::Camera& camera, const unsigned int shader) noexcept;

		///
		/// Create the theme.
		///
		/// \return Pointer to newly created theme.
		///
		Theme* addTheme() noexcept;

		///
		/// Add a widget to the GUI.
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
		/// Remove a widget.
		///
		/// \param id Widget id (widget->id()).
		///
		void remove(const unsigned int id) noexcept;

		///
		/// Destroy all widgets and clean up GUI.
		///
		void destroy() noexcept;

		///
		/// \brief Sets visibility of GUI.
		///
		/// Note: GUI will not process events/updates to widgets while not visible.
		///
		/// \param isVisible False to make invisible.
		///
		void setVisibility(const bool isVisible) noexcept;

		///
		/// \brief Get pointer to internal TaskPool task.
		///
		/// Pass this to taskpool last.
		///
		/// \return Pointer to a task.
		///
		protostar::Task* getTask() noexcept;

	private:
		///
		/// Internal function to process events to widgets across threads.
		///
		void processEvents() noexcept;

		///
		/// Internal update function to update widgets on another thread.
		///
		/// \param dt Pointer to protected delta time.
		///
		void update(protostar::ProtectedDouble* dt) noexcept;

	private:
		///
		/// Flag to mark GUI as destroyed.
		///
		bool m_isDestroyed;

		///
		/// Widget ID counter.
		///
		unsigned int m_counter;

		///
		/// Theme object.
		///
		std::unique_ptr<Theme> m_theme;

		///
		/// Pointer to delta time.
		///
		protostar::ProtectedDouble* m_dt;

		///
		/// Task that runs main GUI loop on another thread.
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
		/// Protected visibility bool.
		///
		protostar::ProtectedBool m_visible;

		///
		/// Protected thread loop running bool.
		///
		protostar::ProtectedBool m_running;

		///
		/// Internal event manager to GUI.
		///
		starlight::Dispatcher m_GUIEventManager;

		///
		/// Holds list of free widget ids.
		///
		std::vector<unsigned int> m_free;

		///
		/// Holds list of widget pointers.
		///
		std::vector<galaxy::WidgetPtr> m_widgets;
	};

	template<typename WidgetType, typename ...Args>
	inline WidgetType* GUI::add(Args&& ...args) noexcept
	{
		unsigned int id = 0;

		if (!m_free.empty())
		{
			id = m_free.back();

			m_free.pop_back();
			m_free.shrink_to_fit();
		}
		else
		{
			id = m_counter;
			m_counter++;
		}

		// Protect m_widgets.
		std::lock_guard<std::mutex> l_lock(m_widgetMutex);

		// Make sure widget is the right size.
		if (id >= m_widgets.size())
		{
			m_widgets.resize(id + 1);
		}

		// Forward arguments to std::vector's construct in place method.
		m_widgets[id] = std::make_unique<WidgetType>(m_theme.get(), std::forward<Args>(args)...);

		// Set ID.
		WidgetType* ref = dynamic_cast<WidgetType*>(m_widgets[id].get());
		ref->m_id = id;

		// Then return a pointer to object placed.
		return ref;
	}

	template<typename Event, typename WidgetType>
	inline void GUI::addEventToWidget(starlight::Callback<Event>&& func, WidgetType* widget) noexcept
	{
		m_GUIEventManager.add<Event>(std::bind(func, widget, std::placeholders::_1));
	}

	template<typename Event, typename ...Args>
	inline void GUI::queue(Args&& ...args) noexcept
	{
		std::lock_guard<std::mutex> l_lock(m_eventMutex);
		m_GUIEventManager.queue<Event>(std::forward<Args>(args)...);
	}
}

#endif
///
/// GUI.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_GUI_HPP_
#define GALAXY_UI_GUI_HPP_

#include "galaxy/error/Log.hpp"
#include "galaxy/events/dispatcher/Dispatcher.hpp"

#include "galaxy/ui/Widget.hpp"

namespace galaxy
{
	namespace ui
	{
		///
		/// Concept to ensure template type is of a widget.
		///
		template<typename Type>
		concept is_widget = std::derived_from<Type, Widget>;

		///
		/// \brief Graphical User Interface.
		///
		/// Creational pattern: Factory.
		/// Structural pattern: Composite.
		/// Behavioral pattern: Observer.
		///
		class GUI final
		{
		public:
			///
			/// Keeps track of the construction state of the UI.
			///
			enum class ConstructionState : short
			{
				///
				/// No construction other than creation has taken place.
				///
				DEFAULT = 0,

				///
				/// A theme has been set.
				///
				THEME_SET = 1
			};

			///
			/// Constructor.
			///
			GUI() noexcept;

			///
			/// Destructor.
			///
			~GUI();

			///
			/// Set the GUI theme.
			///
			/// \param theme Pointer to theme to use with GUI.
			///
			void set_theme(Theme* theme);

			///
			/// Create a widget and return a pointer to it.
			///
			template<is_widget WidgetDerived, typename... Args>
			[[maybe_unused]] WidgetDerived* create_widget(Args&&... args);

			///
			/// Create a tooltip and assign it to a widget.
			///
			template<is_widget WidgetDerived, typename... Args>
			[[nodiscard]] Tooltip* create_tooltip_for_widget(WidgetDerived* widget, Args&&... args);

			///
			/// Update widgets.
			///
			/// \param dt Delta Time from gameloop.
			///
			void update(const double dt);

			///
			/// Render widgets.
			///
			void render();

			///
			/// Registers a function to be called on the triggering of an event.
			///
			/// \param widget Widget to add function to.
			///
			template<meta::is_class Event, is_widget WidgetDerived>
			void add_event_to_widget(WidgetDerived* widget);

			///
			/// Triggers a UI event.
			///
			/// \param args Arguments to construct event to trigger.
			///
			template<meta::is_class Event, typename... Args>
			void trigger(Args&&... args);

			///
			/// Remove a widget.
			///
			/// \param id Widget id (widget->id()).
			///
			void remove(const unsigned int id);

			///
			/// Destroy all widgets and clean up GUI.
			///
			void destroy();

		private:
			///
			/// Copy constructor.
			///
			GUI(const GUI&) = delete;

			///
			/// Move constructor.
			///
			GUI(GUI&&) = delete;

			///
			/// Copy assignment operator.
			///
			GUI& operator=(const GUI&) = delete;

			///
			/// Move assignment operator.
			///
			GUI& operator=(GUI&&) = delete;

		private:
			///
			/// State of the UI.
			///
			ConstructionState m_state;

			///
			/// Internal counter for widget ids.
			///
			unsigned int m_id_counter;

			///
			/// Theme for GUI.
			///
			Theme* m_theme;

			///
			/// Holds list of free widget ids.
			///
			std::vector<unsigned int> m_free;

			///
			/// Holds widgets in contigous array.
			///
			std::vector<WidgetPtr> m_widgets;

			///
			/// Internal event manager to GUI.
			///
			events::Dispatcher m_event_manager;

			///
			/// Spritebatch shader.
			///
			graphics::Shader* m_batch_shader;
		};

		template<is_widget WidgetDerived, typename... Args>
		inline WidgetDerived* GUI::create_widget(Args&&... args)
		{
			WidgetDerived* ptr = nullptr;

			if (m_state >= ConstructionState::THEME_SET)
			{
				unsigned int id = 0;

				// Assigns previous value, then increments.
				// Unless there is a free id.
				if (!m_free.empty())
				{
					id = m_free.back();
					m_free.pop_back();
				}
				else
				{
					id = m_id_counter++;
				}

				if (id >= m_widgets.size())
				{
					m_widgets.resize(id + 1);
				}

				m_widgets[id] = std::make_unique<WidgetDerived>(std::forward<Args>(args)...);

				ptr          = static_cast<WidgetDerived*>(m_widgets[id].get());
				ptr->m_id    = id;
				ptr->m_theme = m_theme;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "You must construct the GUI first.");
			}

			return ptr;
		}

		template<is_widget WidgetDerived, typename... Args>
		inline Tooltip* GUI::create_tooltip_for_widget(WidgetDerived* widget, Args&&... args)
		{
			Tooltip* ptr = nullptr;

			if (m_state >= ConstructionState::THEME_SET)
			{
				if (widget != nullptr)
				{
					if (widget->m_tooltip != nullptr)
					{
						GALAXY_LOG(GALAXY_ERROR, "Attempted to add tooltip to widget that already has one.");
					}
					else
					{
						widget->m_tooltip = std::make_unique<Tooltip>(std::forward<Args>(args)...);
						ptr               = widget->m_tooltip.get();
						ptr->m_theme      = m_theme;
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Attempted to pass a nullptr widget to GUI::create_for_widget.");
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "You must construct the GUI first.");
			}

			return ptr;
		}

		template<meta::is_class Event, is_widget WidgetDerived>
		inline void GUI::add_event_to_widget(WidgetDerived* widget)
		{
			m_event_manager.subscribe<Event, WidgetDerived>(*widget);
		}

		template<meta::is_class Event, typename... Args>
		inline void GUI::trigger(Args&&... args)
		{
			m_event_manager.trigger<Event>(std::forward<Args>(args)...);
		}
	} // namespace ui
} // namespace galaxy

#endif
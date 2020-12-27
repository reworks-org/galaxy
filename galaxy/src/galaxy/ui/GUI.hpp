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
#include "galaxy/graphics/sprite/SpriteBatch.hpp"

#include "galaxy/ui/Widget.hpp"
#include "galaxy/ui/widgets/Slider.hpp"
#include "galaxy/ui/widgets/ProgressBar.hpp"
#include "galaxy/ui/widgets/Textbox.hpp"

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
			GUI();

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

			///
			/// Destructor.
			///
			~GUI();

			///
			/// Set the GUI theme.
			///
			/// \param theme Pointer to theme to use with GUI.
			///
			void set_theme(UITheme* theme);

			///
			/// Create a widget and return a pointer to it.
			///
			template<is_widget Widget, typename... Args>
			[[maybe_unused]] Widget* create_widget(Args&&... args);

			///
			/// Create a tooltip and assign it to a widget.
			///
			template<is_widget Widget, typename... Args>
			[[nodiscard]] Tooltip* create_tooltip_for_widget(Widget* widget, Args&&... args);

			///
			/// Update widgets.
			///
			/// \param dt Delta Time from gameloop.
			///
			void update(const double dt);

			///
			/// Render widgets.
			///
			/// \param camera View camera to use when rendering.
			///
			void render(graphics::Camera& camera);

			///
			/// Registers a function to be called on the triggering of an event.
			///
			/// \param func void function that takes a const Event&.
			/// \param widget Widget to add function to.
			///
			template<meta::is_class Event, is_widget Widget>
			void add_event_to_widget(Widget* widget);

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
			UITheme* m_theme;

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
			/// Spritebatch.
			///
			graphics::SpriteBatch m_sb;
		};

		template<is_widget Widget, typename... Args>
		inline Widget* GUI::create_widget(Args&&... args)
		{
			Widget* ptr = nullptr;

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
					id = m_id_counter;
					m_id_counter++;
				}

				if (id >= m_widgets.size())
				{
					m_widgets.resize(id + 1);
				}

				m_widgets[id] = std::make_unique<Widget>(std::forward<Args>(args)...);

				ptr          = dynamic_cast<Widget*>(m_widgets[id].get());
				ptr->m_id    = id;
				ptr->m_theme = m_theme;

				if constexpr (std::is_same<ui::Widget, ui::Slider>::value)
				{
					m_sb.add(&ptr->m_slider);
					m_sb.add(&ptr->m_marker);
				}
				else if constexpr (std::is_same<ui::Widget, ui::Progressbar>::value)
				{
					m_sb.add(&ptr->m_container);
					m_sb.add(&ptr->m_bar);
				}
				else if constexpr (std::is_same<ui::Widget, ui::Textbox>::value)
				{
					m_sb.add(ptr);
					m_sb.add(&ptr->m_indicator);
				}
				else if constexpr (std::is_base_of<graphics::BatchedSprite, ui::Widget>::value)
				{
					m_sb.add(ptr);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "You must construct the GUI first.");
			}

			return ptr;
		}

		template<is_widget Widget, typename... Args>
		inline Tooltip* GUI::create_tooltip_for_widget(Widget* widget, Args&&... args)
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

		template<meta::is_class Event, is_widget Widget>
		inline void GUI::add_event_to_widget(Widget* widget)
		{
			m_event_manager.subscribe<Event, Widget>(*widget);
		}

		template<meta::is_class Event, typename... Args>
		inline void GUI::trigger(Args&&... args)
		{
			m_event_manager.trigger<Event>(std::forward<Args>(args)...);
		}
	} // namespace ui
} // namespace galaxy

#endif
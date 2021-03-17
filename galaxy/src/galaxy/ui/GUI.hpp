///
/// GUI.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_GUI_HPP_
#define GALAXY_UI_GUI_HPP_

#include "galaxy/error/Log.hpp"
#include "galaxy/ui/widgets/Button.hpp"
#include "galaxy/ui/widgets/Image.hpp"
#include "galaxy/ui/Widgets/Label.hpp"
#include "galaxy/ui/widgets/ProgressBar.hpp"
#include "galaxy/ui/widgets/Slider.hpp"
#include "galaxy/ui/widgets/Textbox.hpp"
#include "galaxy/ui/widgets/TextInput.hpp"
#include "galaxy/ui/widgets/ToggleButton.hpp"

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
		class GUI final : public fs::Serializable
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
			/// Enable input.
			///
			void enable_input() noexcept;

			///
			/// Disable input.
			///
			void disable_input() noexcept;

			///
			/// Triggered upon mouse movement.
			///
			/// \param mme Mouse Moved Event.
			///
			void on_event(const events::MouseMoved& mme) noexcept;

			///
			/// Triggered upon mouse click.
			///
			/// \param mpe Mouse Pressed Event.
			///
			void on_event(const events::MousePressed& mpe) noexcept;

			///
			/// Triggered upon mouse release.
			///
			/// \param mre Mouse Released Event.
			///
			void on_event(const events::MouseReleased& mre) noexcept;

			///
			/// Triggered upon key down.
			///
			/// \param kde Key Down Event.
			///
			void on_event(const events::KeyDown& kde) noexcept;

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
			/// Remove a widget.
			///
			/// \param id Widget id (widget->id()).
			///
			void remove(const unsigned int id);

			///
			/// Destroy all widgets and clean up GUI.
			///
			void clear();

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

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
			/// Spritebatch shader.
			///
			graphics::Shader* m_batch_shader;

			///
			/// Event input flag.
			///
			bool m_allow_input;
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
	} // namespace ui
} // namespace galaxy

#endif
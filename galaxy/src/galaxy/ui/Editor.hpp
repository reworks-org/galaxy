///
/// Editor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EDITOR_HPP_
#define GALAXY_EDITOR_HPP_

#include <zep/imgui/editor_imgui.h>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Manages the ImGui editor gui that assists in gamedev.
	/// Allows for the creation of some really cool tools.
	///
    class Editor final
    {
    public:
		///
		/// Constructor.
		///
		Editor() noexcept;

		///
		/// \brief Argument constructor.
		///
		/// Calls init().
		///
		/// \param window Pointer to main RenderWindow.
		///
		explicit Editor(sf::RenderWindow* window);

		///
		/// Destructor.
		///
		~Editor() noexcept;

		///
		/// Init ImGui and SFML.
		///
		/// \param window Pointer to main RenderWindow.
		///
		void init(sf::RenderWindow* window);

		///
		/// Process imgui events.
		///
		/// \param event SFML event(s).
		///
		void event(const sf::Event& event);

		///
		/// Update Editor.
		///
		/// \param clock Main Application loop clock.
		///
		void update(sf::Clock& clock);

		///
		/// Draw editor.
		///
		void render();

		///
		/// De-inits ImGui and SFML.
		///
		void shutdown();

		///
		/// \brief Display editor on screen.
		///
		/// Call between update and render. Calls the functions that make up the main debug menu. ImGui functions mainly.
		///
		/// \param restart Pointer to boolean controlling if program should restart on exit.
		///
		void display(bool* restart);

	private:
		///
		/// Pointer to RenderWindow.
		///
		sf::RenderWindow* m_window;

		///
		/// Source code editor.
		///
		Zep::ZepEditor_ImGui m_zepEditor;
    };
}

#endif
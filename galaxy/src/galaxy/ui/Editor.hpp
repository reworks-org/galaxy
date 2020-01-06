///
/// Editor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EDITOR_HPP_
#define GALAXY_EDITOR_HPP_

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
		/// \param event ALLEGRO_EVENT object.
		///
		void event();

		///
		/// Draw interface.
		///
		void render();

		///
		/// De-inits ImGui and SFML.
		///
		void shutdown();

		///
		/// \brief Display menu on screen.
		///
		/// Call between update and render. Calls the functions that make up the main debug menu. ImGui functions mainly.
		///
		/// \param restart Pointer to boolean controlling if program should restart on exit.
		///
		void displayMenu(bool* restart);

	private:
		///
		/// Text editor instance for the script editor gui.
		///
		TextEditor m_editor;

		// Variables that are used internally and are required to use the gui.
		bool m_showLuaConsole = false;
		bool m_showScriptEditor = false;
		std::string m_buff = "";
		std::string m_stateBuff = "";

		int m_index = 0;
		bool m_showFilesToLoad = false;
		std::string m_currentScript = "";
		std::vector<std::string> m_files;
    };
}

#endif
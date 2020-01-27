///
/// ImGuiLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_IMGUILAYER_HPP_
#define GALAXY_IMGUILAYER_HPP_

#include "galaxy/ui/Editor.hpp"
#include "galaxy/types/Layer.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// ImGui render and event layer.
	///
	class ImGuiLayer final : public galaxy::Layer
	{
	public:
		///
		/// Constructor.
		///
		/// \param restart Restart flag.
		///
		ImGuiLayer(bool* restart) noexcept;

		///
		/// Destructor.
		///
		~ImGuiLayer() override;

		///
		/// \brief Process layer events.
		///
		/// Allows for std::exceptions.
		///
		/// \param event SFML structure containing polled events.
		///
		void event(const sf::Event & event) override;

		///
		/// \brief Process update (fixed timestep) logic.
		///
		/// Allows for std::exceptions.
		///
		/// \param dt Delta-Time from fixed timestep gameloop.
		///
		void update(sf::Time & dt) override;

		///
		/// \brief Render layer.
		///
		/// Does NOT allow std::exceptions!
		///
		void render() noexcept override;

	private:
		///
		/// Don't allow default construction.
		///
		ImGuiLayer() = delete;

	private:
		///
		/// Gamedev editor UI.
		///
		galaxy::Editor m_editor;

		///
		/// Restart flag.
		///
		bool* m_restart;
	};
}

#endif
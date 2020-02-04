///
/// RenderSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RENDERSYSTEM_HPP_
#define GALAXY_RENDERSYSTEM_HPP_

#include <solar/system/System.hpp>
#include <SFML/Graphics/VertexArray.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// System that handles rendering of entities with a SpriteComponent, TransformComponent, etc.
	///
	class RenderSystem final : public sr::System
	{
	public:
		///
		/// Constructor.
		///
		RenderSystem() noexcept;

		///
		/// Destructor.
		///
		~RenderSystem() noexcept override;

		///
		/// Render sprites / textures to screen.
		///
		/// \param atlas Pointer to texture atlas to draw from.
		/// 
		void render(galaxy::TextureAtlas* atlas);

	private:
		///
		/// Process events.
		///
		/// \param e Event object to pass to system.
		///
		void event(const sr::Event& e) override;

		///
		/// Update System.
		///
		/// \param time DeltaTime from gameloop.
		/// \param manager Entity manager.
		///
		void update(const sr::DeltaTime time, sr::Manager& manager) override;

	private:
		///
		/// Pointer to main rendering window.
		///
		sf::RenderWindow* m_window;

		///
		/// Avoid recreating each render pass.
		///
		sf::VertexArray m_verticies;
	};
}

#endif
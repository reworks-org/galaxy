///
/// AnimationComponent.hpp
/// starlight
///
/// Created by reworks on 16/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_ANIMATIONCOMPONENT_HPP_
#define STARLIGHT_ANIMATIONCOMPONENT_HPP_

#include <unordered_map>

#include "sl/graphics/Animation.hpp"

typedef struct _tmx_map tmx_map;
typedef struct _tmx_tile tmx_tile;

namespace entt { typedef std::uint32_t Entity; }

namespace sl
{
	class AnimationComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param entity Entity to attach to component fixtures.
		/// \param table sol::table containing data.
		///
		AnimationComponent(entt::Entity entity, const sol::table& table);

		///
		/// Constructor.
		///
		/// \param map Map data - needed to look up tiles.
		/// \param tile A tmx_tile containing animation data required to construct component.
		/// \param x xpos of original tilemap on atlas.
		/// \param y ypos of original tilemap on atlas.
		/// \param tileWidth Width of current tile.
		/// \param tileHeight Height of current tile.
		/// \param layerName Ensures unique ids by using layerName as a base.
		///
		AnimationComponent(tmx_map* map, tmx_tile* tile, int x, int y, int tileWidth, int tileHeight, const std::string& layerName);

		///
		/// Destructor.
		///
		~AnimationComponent();

		///
		/// Change the current animation.
		///
		/// \param animation The name of the animation type to change to as defined in lua file. E.g. "walking" -> "running"
		///
		void changeAnimation(std::string_view animation);

		///
		/// Play the animation.
		///
		void play();

		///
		/// Play a specific animation.
		///
		/// \param animation Animation to change to to play.
		///
		void play(std::string_view animation);
	
		///
		/// Pause animation.
		///
		void pause();

		///
		/// Rest animation frames to beginning.
		///
		void stop();

		///
		/// Default move assignment overload.
		///
		AnimationComponent& operator=(const AnimationComponent&);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		AnimationComponent() = delete;

	public:
		bool m_isPaused;
		double m_currentFrameTime;
		std::string_view m_activeAnimation;
		std::unordered_map<std::string_view, Animation> m_animations;
	};
}

#endif
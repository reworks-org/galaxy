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
#include "sl/libs/cereal/access.hpp"
#include "sl/libs/cereal/types/string.hpp"
#include "sl/libs/cereal/types/unordered_map.hpp"

typedef struct _tmx_map tmx_map;
typedef struct _tmx_tile tmx_tile;

namespace sl
{
	class AnimationComponent final
	{
		friend class cereal::access;

	public:
		///
		/// Animated Component Constructor.
		///
		/// \param table sol::table containing data.
		///
		AnimationComponent(const sol::table& table);

		///
		/// \brief Animated Tile Constructor.
		///
		/// Please note that because this is used to construct an animated tile,
		/// it only supports constructing ONE animation.
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
		void changeAnimation(const std::string& animation);

		///
		/// Play the animation.
		///
		void play();

		///
		/// Play a specific animation.
		///
		/// \param animation Animation to change to to play.
		///
		void play(const std::string& animation);
	
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

		///
		/// Cereal serialize function.
		///
		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(m_isPaused, m_currentFrameTime, m_activeAnimation, m_animations);
		}

	public:
		bool m_isPaused;
		double m_currentFrameTime;
		std::string m_activeAnimation;
		std::unordered_map<std::string, Animation> m_animations;
	};
}

#endif
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
#include "sl/libs/cereal/types/unordered_map.hpp"

typedef struct _tmx_map tmx_map;
typedef struct _tmx_tile tmx_tile;

namespace sl
{
	///
	/// Stores data for an entity about its animations.
	///
	class AnimationComponent final
	{
		friend class cereal::access;

	public:
		///
		/// \brief Default Constructor.
		///
		/// Contains empty data values.
		///
		AnimationComponent();

		///
		/// Animated Component Constructor.
		///
		/// \param table sol::table containing data.
		///
		explicit AnimationComponent(const sol::table& table);

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
		~AnimationComponent() noexcept;

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
		/// Stop the animation. If you call play() the animation starts from the beginning.
		///
		void stop();

	private:
		///
		/// Cereal serialize function.
		///
		template<class Archive>
		inline void serialize(Archive& ar)
		{
			ar(m_isPaused, m_currentFrameTime, m_activeAnimation, m_animations);
		}

	public:
		///
		/// Controls if the animation is paused or not.
		///
		bool m_isPaused;
		
		///
		/// Current amount of time spent on the frame.
		///
		double m_currentFrameTime;
		
		///
		/// Active animation being... animated.
		///
		std::string m_activeAnimation;
		
		///
		/// Storage of the animations.
		///
		std::unordered_map<std::string, Animation> m_animations;
	};
}

#endif
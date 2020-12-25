///
/// Sprite.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPRITE_HPP_
#define GALAXY_COMPONENTS_SPRITE_HPP_

#include "galaxy/graphics/sprite/Sprite.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Allows for an entity to be rendered.
		///
		class Sprite final
		{
		public:
			///
			/// Constructor.
			///
			Sprite();

			///
			/// Destructor.
			///
			~Sprite() = default;

			///
			/// Sprite object.
			///
			graphics::Sprite m_sprite;
		};
	} // namespace components
} // namespace galaxy

#endif
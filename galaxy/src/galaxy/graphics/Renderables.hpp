///
/// Renderables.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERABLES_HPP_
#define GALAXY_GRAPHICS_RENDERABLES_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Defines different types of renderable objects.
		///
		enum class Renderables : int
		{
			///
			/// Defines an entity to be rendered as a POINT.
			///
			POINT,

			///
			/// Defines an entity to be rendered as a LINE.
			///
			LINE,

			///
			/// Defines an entity to be rendered as a CIRCLE.
			///
			CIRCLE,

			///
			/// Defines an entity to be rendered as a SPRITE.
			///
			SPRITE,

			///
			/// Defines an entity to be rendered as TEXT.
			///
			TEXT,

			///
			/// Define an entity to be batch rendered.
			///
			BATCHED
		};
	} // namespace graphics
} // namespace galaxy

#endif
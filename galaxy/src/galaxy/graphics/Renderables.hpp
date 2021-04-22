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
			/// Defines an entity to be rendered as a LINE_LOOP.
			///
			LINE_LOOP,

			///
			/// Define an entity to be rendered as a BATCH.
			///
			BATCHED,

			///
			/// Define an entity to be rendered as TEXT.
			///
			TEXT,

			///
			/// Define an entity to be rendered as QUAD.
			///
			SPRITE
		};
	} // namespace graphics
} // namespace galaxy

#endif
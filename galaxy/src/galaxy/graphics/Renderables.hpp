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
			POINT = 0,

			///
			/// Defines an entity to be rendered as a LINE.
			///
			LINE = 1,

			///
			/// Defines an entity to be rendered as a LINE_LOOP.
			///
			LINE_LOOP = 2,

			///
			/// Define an entity to be rendered as a BATCH.
			///
			BATCHED = 3,

			///
			/// Define an entity to be rendered as TEXT.
			///
			TEXT = 4
		};
	} // namespace graphics
} // namespace galaxy

#endif
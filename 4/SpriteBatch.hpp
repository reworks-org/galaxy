///
/// SpriteBatch.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPRITEBATCH_HPP_
#define GALAXY_COMPONENTS_SPRITEBATCH_HPP_

#include "galaxy/graphics/sprite/BatchedSprite.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Component for a spritebatch.
		///
		class SpriteBatchComponent final
		{
		public:
			///
			/// Constructor.
			///
			SpriteBatchComponent();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			SpriteBatchComponent(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~SpriteBatchComponent() = default;

			///
			/// Batched sprite object.
			///
			graphics::BatchedSprite m_bs;
		};
	} // namespace components
} // namespace galaxy

#endif
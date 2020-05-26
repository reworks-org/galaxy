///
/// SpriteBatchComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SPRITEBATCHCOMPONENT_HPP_
#define GALAXY_SPRITEBATCHCOMPONENT_HPP_

#include <nlohmann/json_fwd.hpp>
#include <qs/graphics/SpriteBatch.hpp>
#include <qs/graphics/TextureAtlas.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Component for a spritebatch.
	///
	class SpriteBatchComponent final
	{
	public:
		///
		/// Default constructor.
		///
		SpriteBatchComponent() noexcept;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		SpriteBatchComponent(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		~SpriteBatchComponent() noexcept;

		///
		/// Set pointer to texture atlas.
		///
		/// \param atlas Pointer to atlas.
		///
		void setAtlas(qs::TextureAtlas* atlas) noexcept;

		///
		/// Uses JSON and atlas to create the sprite batch.
		///
		/// \param json JSON defining object.
		///
		void create(const nlohmann::json& json) noexcept;

	public:
		///
		/// SpriteBatch object.
		///
		qs::SpriteBatch m_spritebatch;

	private:
		///
		/// Pointer to texture atlas.
		///
		qs::TextureAtlas* m_atlas;
	};
}

#endif
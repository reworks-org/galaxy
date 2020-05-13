///
/// SpriteBatchComponent.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SPRITEBATCHCOMPONENT_HPP_
#define GALAXY_SPRITEBATCHCOMPONENT_HPP_

#include <qs/core/Shader.hpp>
#include <qs/graphics/SpriteBatch.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Component for a spritebatch.
	///
	class SpriteBatch final
	{
	public:
		///
		/// Default constructor.
		///
		SpriteBatch() noexcept;

		///
		/// Argument constructor.
		///
		/// \param shader Shader pointer to use.
		///
		SpriteBatch(qs::Shader* shader) noexcept;

		///
		/// Destructor.
		///
		~SpriteBatch() noexcept;

		///
		/// Stores pointer to a shader.
		///
		/// \param shader Shader pointer to use.
		///
		void setShader(qs::Shader* shader) noexcept;

	public:
		///
		/// Shader object.
		///
		qs::Shader* m_shader;

		///
		/// SpriteBatch object.
		///
		qs::SpriteBatch m_spritebatch;
	};
}

#endif
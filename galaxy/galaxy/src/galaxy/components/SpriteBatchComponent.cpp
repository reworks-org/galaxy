///
/// SpriteBatchComponent.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include "SpriteBatchComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	SpriteBatch::SpriteBatch() noexcept
		:m_shader(nullptr)
	{
	}

	SpriteBatch::SpriteBatch(qs::Shader* shader) noexcept
		:m_shader(shader)
	{
	}

	void SpriteBatch::setShader(qs::Shader* shader) noexcept
	{
		m_shader = shader;
	}
}
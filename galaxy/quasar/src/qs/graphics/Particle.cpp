///
/// Particle.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Particle.hpp"

///
/// Core namespace.
///
namespace qs
{
	Particle::Particle()
		: VertexData{}, Texture{}, m_amount{1}
	{
	}

	Particle::Particle(pr::positive_integer auto amount)
		: VertexData{}, Texture{}, m_amount{amount}
	{
	}

	void Particle::set_z_level(const pr::positive_integer auto z_level) noexcept
	{
		m_z_level = z_level;
	}

	void Particle::bind() noexcept
	{
		m_va.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void Particle::unbind() noexcept
	{
		m_va.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	const unsigned int Particle::amount() const noexcept
	{
		return m_amount;
	}
} // namespace qs
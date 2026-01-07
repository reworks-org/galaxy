///
/// Sampler.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Sampler.hpp"

namespace galaxy
{
	Sampler::Sampler() noexcept
		: m_id {0}
	{
		glCreateSamplers(1, &m_id);
	}

	Sampler::Sampler(Sampler&& s) noexcept
	{
		if (this->m_id != 0)
		{
			glDeleteSamplers(1, &m_id);
		}

		this->m_id = s.m_id;
		s.m_id     = 0;
	}

	Sampler& Sampler::operator=(Sampler&& s) noexcept
	{
		if (this != &s)
		{
			if (this->m_id != 0)
			{
				glDeleteSamplers(1, &m_id);
			}

			this->m_id = s.m_id;
			s.m_id     = 0;
		}

		return *this;
	}

	Sampler::~Sampler() noexcept
	{
		glDeleteSamplers(1, &m_id);
	}

	void Sampler::set(const unsigned int param, const int value) const
	{
		glSamplerParameteri(m_id, param, value);
	}

	void Sampler::setf(const unsigned int param, const float value) const
	{
		glSamplerParameterf(m_id, param, value);
	}

	void Sampler::bind(const unsigned int texture_unit) const
	{
		glBindSampler(texture_unit, m_id);
	}

	void Sampler::unbind(const unsigned int texture_unit) const
	{
		glBindSampler(texture_unit, 0);
	}

	unsigned int Sampler::id() const noexcept
	{
		return m_id;
	}
} // namespace galaxy

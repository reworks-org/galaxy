///
/// Picker.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include "Picker.hpp"

namespace sc
{
	Picker::Picker() noexcept
	    : m_world_pointer {nullptr}, m_body {nullptr}
	{
	}

	Picker::Picker(Picker&& p) noexcept
	{
		this->m_world_pointer = p.m_world_pointer;
		this->m_body          = p.m_body;

		p.m_body = nullptr;
	}

	Picker& Picker::operator=(Picker&& p) noexcept
	{
		if (this != &p)
		{
			this->m_world_pointer = p.m_world_pointer;
			this->m_body          = p.m_body;

			p.m_body = nullptr;
		}

		return *this;
	}

	Picker::~Picker()
	{
		if (m_body != nullptr)
		{
			m_world_pointer->DestroyBody(m_body);
			m_body = nullptr;
		}
	}
} // namespace sc
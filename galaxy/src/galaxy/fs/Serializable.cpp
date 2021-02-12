///
/// Serializable.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace fs
	{
		SerializableWrapper::SerializableWrapper() noexcept
		    : m_obj {nullptr}
		{
		}

		SerializableWrapper::~SerializableWrapper() noexcept
		{
			m_obj = nullptr;
		}

		Serializable::Serializable(Serializable* s) noexcept
		{
			m_wrapper.m_obj = s;
		}

		Serializable::Serializable() noexcept
		{
			m_wrapper.m_obj = this;
		}
	} // namespace fs
} // namespace galaxy
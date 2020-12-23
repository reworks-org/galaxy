///
/// Serializable.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/Serializer.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Serializable::~Serializable()
	{
		auto* s = SL_HANDLE.serializer();
		s->remove(m_id);
	}

	Serializable::Serializable()
	    : m_id {0}
	{
		auto* s = SL_HANDLE.serializer();
		m_id    = s->register_obj(this);
	}
} // namespace galaxy
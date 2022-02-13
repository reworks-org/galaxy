///
/// Serializable.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/Serializer.hpp"

namespace galaxy
{
	namespace fs
	{
		Serializable::Serializable(Serializable* obj)
		{
			auto& serializer = core::ServiceLocator<fs::Serializer>::ref();
			serializer.add(obj);
		}
	} // namespace fs
} // namespace galaxy
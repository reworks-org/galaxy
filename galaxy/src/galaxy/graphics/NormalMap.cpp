///
/// NormalMap.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "NormalMap.hpp"

namespace galaxy
{
	namespace graphics
	{
		NormalMap::NormalMap()
			: m_normal_mapped {false}
			, m_nm_texture_handle {0u}
		{
		}

		NormalMap::NormalMap(NormalMap&& nm)
		{
			this->m_normal_mapped     = nm.m_normal_mapped;
			this->m_nm_texture_handle = nm.m_nm_texture_handle;

			nm.m_nm_texture_handle = 0u;
		}

		NormalMap& NormalMap::operator=(NormalMap&& nm)
		{
			if (this != &nm)
			{
				this->m_normal_mapped     = nm.m_normal_mapped;
				this->m_nm_texture_handle = nm.m_nm_texture_handle;

				nm.m_nm_texture_handle = 0u;
			}

			return *this;
		}

		NormalMap::~NormalMap()
		{
		}

		bool NormalMap::is_normal_mapped() const
		{
			return m_normal_mapped;
		}

		unsigned int NormalMap::normal_map_texture() const
		{
			return m_nm_texture_handle;
		}

	} // namespace graphics
} // namespace galaxy
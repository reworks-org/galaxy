///
/// NineSlice.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "NineSlice.hpp"

///
/// Core namespace.
///
namespace qs
{
	NineSlice::NineSlice(const NineSlice& ns)
	{
		this->m_cell_size = ns.m_cell_size;
	}

	NineSlice::NineSlice(NineSlice&& ns)
	{
		this->m_cell_size = ns.m_cell_size;
	}

	NineSlice& NineSlice::operator=(const NineSlice& ns)
	{
		this->m_cell_size = ns.m_cell_size;

		return *this;
	}

	NineSlice& NineSlice::operator=(NineSlice&& ns)
	{
		if (this != &ns)
		{
			this->m_cell_size = ns.m_cell_size;
		}

		return *this;
	}
} // namespace qs
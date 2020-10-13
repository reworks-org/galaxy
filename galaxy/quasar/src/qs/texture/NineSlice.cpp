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
		this->m_bottom       = ns.m_bottom;
		this->m_bottom_left  = ns.m_bottom_left;
		this->m_bottom_right = ns.m_bottom_right;
		this->m_left         = ns.m_left;
		this->m_middle       = ns.m_middle;
		this->m_right        = ns.m_right;
		this->m_top          = ns.m_top;
		this->m_top_left     = ns.m_top_left;
		this->m_top_right    = ns.m_top_right;
	}

	NineSlice::NineSlice(NineSlice&& ns)
	{
		this->m_bottom       = std::move(ns.m_bottom);
		this->m_bottom_left  = std::move(ns.m_bottom_left);
		this->m_bottom_right = std::move(ns.m_bottom_right);
		this->m_left         = std::move(ns.m_left);
		this->m_middle       = std::move(ns.m_middle);
		this->m_right        = std::move(ns.m_right);
		this->m_top          = std::move(ns.m_top);
		this->m_top_left     = std::move(ns.m_top_left);
		this->m_top_right    = std::move(ns.m_top_right);
	}

	NineSlice& NineSlice::operator=(const NineSlice& ns)
	{
		this->m_bottom       = ns.m_bottom;
		this->m_bottom_left  = ns.m_bottom_left;
		this->m_bottom_right = ns.m_bottom_right;
		this->m_left         = ns.m_left;
		this->m_middle       = ns.m_middle;
		this->m_right        = ns.m_right;
		this->m_top          = ns.m_top;
		this->m_top_left     = ns.m_top_left;
		this->m_top_right    = ns.m_top_right;

		return *this;
	}

	NineSlice& NineSlice::operator=(NineSlice&& ns)
	{
		if (this != &ns)
		{
			this->m_bottom       = std::move(ns.m_bottom);
			this->m_bottom_left  = std::move(ns.m_bottom_left);
			this->m_bottom_right = std::move(ns.m_bottom_right);
			this->m_left         = std::move(ns.m_left);
			this->m_middle       = std::move(ns.m_middle);
			this->m_right        = std::move(ns.m_right);
			this->m_top          = std::move(ns.m_top);
			this->m_top_left     = std::move(ns.m_top_left);
			this->m_top_right    = std::move(ns.m_top_right);
		}

		return *this;
	}
} // namespace qs
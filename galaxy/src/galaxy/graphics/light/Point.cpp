///
/// Point.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Point.hpp"

namespace galaxy
{
	namespace light
	{
		Point::Point() noexcept
		    : Light {}, m_pos {0.0f, 0.0f, 0.0f}
		{
			set_attunement(Ranges::DIST_7);
		}

		void Point::set_attunement(const Ranges dist) noexcept
		{
			m_dist = dist;
			switch (m_dist)
			{
				case Ranges::DIST_7:
					m_linear    = 0.7;
					m_quadratic = 1.8;
					break;

				case Ranges::DIST_13:
					m_linear    = 0.35;
					m_quadratic = 0.44;
					break;

				case Ranges::DIST_20:
					m_linear    = 0.22;
					m_quadratic = 0.20;
					break;

				case Ranges::DIST_32:
					m_linear    = 0.14;
					m_quadratic = 0.07;
					break;

				case Ranges::DIST_50:
					m_linear    = 0.09;
					m_quadratic = 0.032;
					break;

				case Ranges::DIST_65:
					m_linear    = 0.07;
					m_quadratic = 0.017;
					break;

				case Ranges::DIST_100:
					m_linear    = 0.045;
					m_quadratic = 0.0075;
					break;

				case Ranges::DIST_160:
					m_linear    = 0.027;
					m_quadratic = 0.0028;
					break;

				case Ranges::DIST_200:
					m_linear    = 0.022;
					m_quadratic = 0.0019;
					break;

				case Ranges::DIST_325:
					m_linear    = 0.014;
					m_quadratic = 0.0007;
					break;

				case Ranges::DIST_600:
					m_linear    = 0.007;
					m_quadratic = 0.0002;
					break;

				case Ranges::DIST_3250:
					m_linear    = 0.0014;
					m_quadratic = 0.000007;
					break;
			}
		}

		const double Point::get_linear() const noexcept
		{
			return m_linear;
		}

		const double Point::get_quadratic() const noexcept
		{
			return m_quadratic;
		}
	} // namespace light
} // namespace galaxy
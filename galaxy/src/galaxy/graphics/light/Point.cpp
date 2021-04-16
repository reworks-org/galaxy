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
					m_linear    = 0.7f;
					m_quadratic = 1.8f;
					break;

				case Ranges::DIST_13:
					m_linear    = 0.35f;
					m_quadratic = 0.44f;
					break;

				case Ranges::DIST_20:
					m_linear    = 0.22f;
					m_quadratic = 0.20f;
					break;

				case Ranges::DIST_32:
					m_linear    = 0.14f;
					m_quadratic = 0.07f;
					break;

				case Ranges::DIST_50:
					m_linear    = 0.09f;
					m_quadratic = 0.032f;
					break;

				case Ranges::DIST_65:
					m_linear    = 0.07f;
					m_quadratic = 0.017f;
					break;

				case Ranges::DIST_100:
					m_linear    = 0.045f;
					m_quadratic = 0.0075f;
					break;

				case Ranges::DIST_160:
					m_linear    = 0.027f;
					m_quadratic = 0.0028f;
					break;

				case Ranges::DIST_200:
					m_linear    = 0.022f;
					m_quadratic = 0.0019f;
					break;

				case Ranges::DIST_325:
					m_linear    = 0.014f;
					m_quadratic = 0.0007f;
					break;

				case Ranges::DIST_600:
					m_linear    = 0.007f;
					m_quadratic = 0.0002f;
					break;

				case Ranges::DIST_3250:
					m_linear    = 0.0014f;
					m_quadratic = 0.000007f;
					break;
			}
		}

		const float Point::get_linear() const noexcept
		{
			return m_linear;
		}

		const float Point::get_quadratic() const noexcept
		{
			return m_quadratic;
		}
	} // namespace light
} // namespace galaxy
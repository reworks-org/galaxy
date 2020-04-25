///
/// UI.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_UI_HPP_
#define CELESTIAL_UI_HPP_

namespace celestial
{
	class UI final
	{
	public:
		UI() noexcept;
		~UI() noexcept;

		void setDTPtr(const double* deltaTime) noexcept;

		void sendEvents();

	private:
		void update() noexcept;
	};
}

#endif
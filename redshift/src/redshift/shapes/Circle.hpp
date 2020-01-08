///
/// Circle.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_CIRCLE_HPP_
#define REDSHIFT_CIRCLE_HPP_

namespace rs
{
	class Circle
	{
	public:
		Circle();
		~Circle();
		Circle(int radius, int center);

		void setRadius();
		void setCenter();

		void getRadius();
		void getCenter();
		
	private: 
	};
}

#endif
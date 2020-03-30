///
/// Transform.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TRANSFORM_HPP_
#define QUASAR_TRANSFORM_HPP_

namespace qs
{
	class Transform
	{
	public:
		void translate();
		void rotate();

		//glm::mat4* getMatrix();

	private:
		//glm::mat4* matrix;
	};
}

#endif
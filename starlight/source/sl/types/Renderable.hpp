///
/// Renderable.hpp
/// starlight
///
/// Created by reworks on 22/04/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RENDERABLE_HPP_
#define STARLIGHT_RENDERABLE_HPP_

namespace sl
{
	class Renderable
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual ~Renderable() = default;

	protected:
		///
		/// Default constructor.
		/// This is protected to prevent initialization of this class.
		///
		Renderable() = default;

	public:
		unsigned int m_layer;

	};
}

#endif
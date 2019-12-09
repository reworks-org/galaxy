///
/// ObjectLayer.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_OBJECTLAYER_HPP_
#define STARMAP_OBJECTLAYER_HPP_

#include "starmap/layer/Layer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Specialized type of layer.
	///
	class ObjectLayer : public starmap::Layer<ObjectLayer>
	{
	public:

	private:
		///
		/// topdown (default) or index.
		///
		std::string m_drawOrder;
	};
}

#endif
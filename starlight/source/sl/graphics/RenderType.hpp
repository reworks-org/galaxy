///
/// RenderType.hpp
/// starlight
///
///  Created by reworks on 13/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RENDERTYPE_HPP_
#define STARLIGHT_RENDERTYPE_HPP_

namespace sl
{
	///
	/// Render Types. 
	/// Struct over enum so you can inherit.
	/// 0-3 reserved by library.
	///
	struct RenderTypes
	{
		static constexpr unsigned int SPRITE = 0;
		static constexpr unsigned int TEXT = 1;
		static constexpr unsigned int PARTICLE = 2;
		static constexpr unsigned int MAP = 3;
	};

	typedef int RenderType;
}

#endif
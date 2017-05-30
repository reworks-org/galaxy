//
//  PostEffect.hpp
//  REngine3
//
//  Created by reworks on 29/5/17.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_POSTEFFECT_HPP_
#define RENGINE3_POSTEFFECT_HPP_

namespace re
{
    class PostEffect
    {
	public:
		virtual ~PostEffect();

		virtual void apply() = 0;
		virtual void activate() = 0;
    };
}

#endif

//
//  Renderable.hpp
//  rework
//
//  Created by reworks on 25/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_RENDERABLE_HPP_
#define REWORK_RENDERABLE_HPP_

#include "entityx/Entity.h"

namespace re
{
	friend class Layer;

	class Renderable
	{
	public:
		///
		/// Destructor.
		///
		virtual inline ~Renderable() {}

	protected:
		///
		/// Render Component.
		///
		virtual void render() = 0;

	public:
		int m_layer;

	protected:
		entityx::Entity m_entity;
	};
}

#endif
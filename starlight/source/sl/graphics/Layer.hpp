///
///  Layer.hpp
///  starlight
///
///  Created by reworks on 11/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_LAYER_HPP_
#define STARLIGHT_LAYER_HPP_

namespace re
{
	class Layer
	{
	public:
		///
		/// Constructor.
		///
		/// \param defaultAlloc Amount of space reserved in containers to prevent constant reallocation.
		///
		Layer(unsigned int defaultAlloc = 20);
	
		///
		/// Destructor.
		///
		~Layer();

		///
		/// Render renderables on this layer.
		///
		void render();

		///
		/// Insert a sprite into this layer.
		///
		/// \param sc SpriteComponent to insert.
		///
		void insert(Renderable* rc);

		///
		/// Sort containers.
		///
		/// void sort();
		
		///
		/// Remove data from containers.
		///
		void clean();

	private:
		unsigned int m_defaultAlloc;
		std::vector<Renderable*> m_renderables;

	private:
		///
		/// Default Constructor.
		/// Deletd.
		///
		RenderSystem() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		RenderSystem(const RenderSystem&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		RenderSystem(RenderSystem&&) = delete;
	};
}

#endif
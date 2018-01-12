///
///  Box2DManager.hpp
///  starlight
///
///  Created by reworks on 12/11/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_BOX2DMANAGER_HPP_
#define STARLIGHT_BOX2DMANAGER_HPP_

#include <map>
#include <functional>

#include "Box2D/Box2D.h"
#include "sl/types/ServiceLocator.hpp"

namespace sl
{
	class Box2DManager : public ServiceLocator<Box2DManager>
	{
	public:
		///
		/// Constructor. Sets up Box2D systems.
		///
		/// \param gravity A gravity value for Box2D to use in simulations.
		///
		Box2DManager(float32 gravity);

		///
		/// Destructor
		///
		~Box2DManager() override;

		///
		/// Removes any collision functions in the map.
		///
		void clean();

	public:
		std::unique_ptr<b2World> m_world;
		std::map<std::pair<const std::string&, const std::string&>, std::function<void(const std::string&, const std::string&)>> m_collisionFunctions;

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		Box2DManager() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		Box2DManager(const Box2DManager&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		Box2DManager(Box2DManager&&) = delete;
	};
}

#endif
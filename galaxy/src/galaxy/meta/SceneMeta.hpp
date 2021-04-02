///
/// SceneMeta.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_SCENEMETA_HPP_
#define GALAXY_META_SCENEMETA_HPP_

#include <type_traits>

#include "galaxy/scenes/Scene2D.hpp"
#include "galaxy/scenes/Scene3D.hpp"

namespace galaxy
{
	namespace meta
	{
		///
		/// Concept to ensure template parameter is a Scene.
		///
		template<typename Type>
		concept is_scene = (std::is_same<Type, scene::Scene2D>::value ||
				    std::is_same<Type, scene::Scene3D>::value);
	} // namespace meta
} // namespace galaxy

#endif
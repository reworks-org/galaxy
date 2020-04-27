///
/// TransformComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TRANSFORMCOMPONENT_HPP_
#define GALAXY_TRANSFORMCOMPONENT_HPP_

#include <SFML/Graphics/Transform.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Allows for an entities sprite to be transformed.
	///
	class TransformComponent final
	{
	public:
		///
		/// Default constructor.
		///
		TransformComponent() noexcept = default;

		///
		/// Destructor.
		///
		~TransformComponent() noexcept = default;

	public:
		///
		/// Transformation object.
		///
		sf::Transform m_transform;
	};
}

#endif
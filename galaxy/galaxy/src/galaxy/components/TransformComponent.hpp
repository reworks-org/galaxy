///
/// TransformComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_TRANSFORMCOMPONENT_HPP_
#define GALAXY_TRANSFORMCOMPONENT_HPP_

#include <nlohmann/json_fwd.hpp>
#include <qs/core/Transform.hpp>

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
		TransformComponent() noexcept;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		TransformComponent(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		~TransformComponent() noexcept = default;

	public:
		///
		/// Transformation object.
		///
		qs::Transform m_transform;
	};
}

#endif
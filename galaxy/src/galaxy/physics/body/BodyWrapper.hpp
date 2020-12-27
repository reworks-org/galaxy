///
/// BodyWrapper.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_BODY_BODYWRAPPER_HPP_
#define GALAXY_PHYSICS_BODY_BODYWRAPPER_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/physics/body/Body.hpp"

namespace galaxy
{
	namespace physics
	{
		///
		/// Flag component used to determine if an entity is "alive" or "dead".
		///
		class BodyWrapper final
		{
		public:
			///
			/// Constructor.
			///
			BodyWrapper();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			BodyWrapper(const nlohmann::json& json);

			///
			/// Copy constructor.
			///
			BodyWrapper(const BodyWrapper&) = delete;

			///
			/// Copy assignment operator.
			///
			BodyWrapper& operator=(const BodyWrapper&) = delete;

			///
			/// Destructor.
			///
			~BodyWrapper();

			///
			/// Physics body.
			///
			std::shared_ptr<Body> m_body;
		};
	} // namespace physics
} // namespace galaxy

#endif
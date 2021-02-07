///
/// Physics.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_PHYSICS_HPP_
#define GALAXY_COMPONENTS_PHYSICS_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/physics/Box2DIntegration.hpp"

namespace galaxy
{
	namespace core
	{
		class World;
	} // namespace core

	namespace components
	{
		///
		/// Physics component.
		///
		class Physics final
		{
			friend class core::World;

		public:
			///
			/// Constructor.
			///
			Physics() noexcept;

			///
			/// Move constructor.
			///
			Physics(Physics&&) noexcept;

			///
			/// Move assignment operator.
			///
			Physics& operator=(Physics&&) noexcept;

			///
			/// Destructor.
			///
			~Physics();

			///
			/// Create the internal box2d body from a config structure.
			///
			/// \param config BodyConfig structure.
			///
			void create(const physics::BodyConfig& config);

			///
			/// Create this component and body from JSON.
			///
			/// \param file JSON file on disk in VFS.
			///
			void create_from_json(std::string_view file);

			///
			/// Get internal box2d body pointer.
			///
			/// \return Const pointer to internal body.
			///
			[[nodiscard]] b2Body* const body();

		private:
			///
			/// Copy constructor.
			///
			Physics(const Physics&) = delete;

			///
			/// Copy assignment operator.
			///
			Physics& operator=(const Physics&) = delete;

			///
			/// Internal function to parse the JSON object.
			///
			/// \param json JSON object to parse.
			///
			void parse_json(const nlohmann::json& json);

		private:
			///
			/// Allows body to interact with world.
			///
			b2World* m_world_pointer;

			///
			/// Box2D body.
			///
			b2Body* m_body;
		};
	} // namespace components
} // namespace galaxy

#endif
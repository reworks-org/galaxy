///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENE_WORLD_HPP_
#define GALAXY_SCENE_WORLD_HPP_

#include "galaxy/scene/Scene.hpp"
#include "galaxy/state/StateMachine.hpp"

namespace galaxy
{
	///
	/// Scene, Entity and global game management.
	///
	class World final : public StateMachine<Scene>
	{
	public:
		///
		/// Constructor.
		///
		World() noexcept;

		///
		/// Move constructor.
		///
		World(World&&);

		///
		/// Move assignment operator.
		///
		World& operator=(World&&);

		///
		/// Destructor.
		///
		virtual ~World();

		///
		/// Process events and updates.
		///
		void update() override;

		///
		/// Render scenes.
		///
		void render();

		///
		/// Removes all data.
		///
		void clear();

	private:
		///
		/// Copy constructor.
		///
		World(const World&) = delete;

		///
		/// Copy assignment operator.
		///
		World& operator=(const World&) = delete;

	private:
		///
		/// Entity data.
		///
		Registry m_registry;
	};
} // namespace galaxy

/*
			///
			/// \brief Load app data file into scene manager.
			///
			/// Will remove any other loaded scenes and adjust the currently loaded scene.
			///
			/// \param appdata_file Zlib + Base64 encoded json data file. Should have .galaxy extension.
			///
			void load_app(const std::string& appdata_file);

			///
			/// Save all active scenes and sub data within those scenes.
			///
			/// \param file File to save data to.
			///
			void save_app(const std::string& file);

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]]
			nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;
*/

#endif

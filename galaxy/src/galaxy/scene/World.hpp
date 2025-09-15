///
/// World.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENE_WORLD_HPP_
#define GALAXY_SCENE_WORLD_HPP_

#include <ankerl/unordered_dense.h>

#include "galaxy/scene/Scene.hpp"

namespace galaxy
{
	///
	/// Scene saving/loading, pushing, popping, creating,
	/// Entity management.
	///
	class World final
	{
		using SceneMap   = ankerl::unordered_dense::map<std::uint64_t, std::shared_ptr<Scene>>;
		using SceneStack = std::vector<std::shared_ptr<Scene>>;

	public:
		///
		/// Constructor.
		///
		World() noexcept;

		///
		/// Destructor.
		///
		~World() noexcept;

		///
		/// Add a new scene.
		///
		/// \param name Scene name.
		///
		/// \return Shared pointer to added scene.
		///
		[[maybe_unused]]
		std::shared_ptr<Scene> add(const std::string& name);

		///
		/// Get a specific scene.
		///
		/// \param name Name of scene.
		///
		/// \return Shared pointer to scene.
		///
		[[nodiscard]]
		std::shared_ptr<Scene> get(const std::string& name) noexcept;

		///
		/// Remove a specific scene.
		///
		/// \param name Name of scene.
		///
		void remove(const std::string& name);

		///
		/// Does a scene exist.
		///
		/// \param name Name of scene.
		///
		/// \return True if exists.
		///
		[[nodiscard]]
		bool has(const std::string& name) noexcept;

		///
		/// Push a scene onto the top of stack.
		///
		/// \param name Name of scene.
		///
		void push(const std::string& name) noexcept;

		///
		/// Remove scene on top of stack.
		///
		void pop() noexcept;

		///
		/// Remove all scenes in stack.
		///
		void pop_all() noexcept;

		///
		/// Get top scene in stack.
		///
		/// \return Shared pointer to top scene.
		///
		[[nodiscard]]
		std::shared_ptr<Scene> top() const noexcept;

		///
		/// Process events and updates.
		///
		void update();

		///
		/// Render scenes.
		///
		void render();

		///
		/// Removes all data.
		///
		void clear();

		///
		/// Get Scene storage.
		///
		/// \return Reference.
		///
		[[nodiscard]]
		SceneMap& get_scenes() noexcept;

		///
		/// Get Scene stack.
		///
		/// \return Reference.
		///
		[[nodiscard]]
		SceneStack& get_scene_stack() noexcept;

	private:
		///
		/// Entity data.
		///
		Registry m_registry;

		///
		/// Scene storage.
		///
		SceneMap m_scenes;

		///
		/// Active scenes.
		///
		SceneStack m_scene_stack;
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

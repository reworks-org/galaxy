///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_SCENE_HPP_
#define GALAXY_STATE_SCENE_HPP_

#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Musical.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Sounds.hpp"
#include "galaxy/state/LayerStack.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// Typedef for shader cache.
		///
		using ShaderCache = resource::Cache<graphics::Shader, resource::Shaders>;

		///
		/// Typedef for sound cache.
		///
		using SoundCache = resource::Cache<audio::Sound, resource::Sounds>;

		///
		/// Typedef for music cache.
		///
		using MusicCache = resource::Cache<audio::Music, resource::Musical>;

		///
		/// Represents a scene in a game. Like the menu, game, etc.
		///
		class Scene final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Scene() noexcept;

			///
			/// Destructor.
			///
			virtual ~Scene() noexcept;

			///
			/// \brief Load data required by this scene.
			///
			/// Displays a loading screen overriding whatever is currently rendering.
			///
			void load();

			///
			/// Unload scene data.
			///
			void unload();

			///
			/// Process Scene events.
			///
			void events();

			///
			/// Process update (fixed timestep) logic.
			///
			void update();

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			void pre_render();

			///
			/// Render Scene.
			///
			void render();

			///
			/// Set Scene name.
			///
			/// \param name String name for debug purposes.
			///
			void set_name(const std::string& name) noexcept;

			///
			/// Get Scene name.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Get layer stack.
			///
			/// \return Reference to scene stack.
			///
			[[nodiscard]] LayerStack& get_stack() noexcept;

			///
			/// Get shader cache.
			///
			/// \return Ref to shader cache.
			///
			[[nodiscard]] ShaderCache& shader_cache() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Scene name for debug purposes.
			///
			std::string m_name;

			///
			/// Controls current application layer.
			///
			LayerStack m_layer_stack;

			///
			/// Shader cache.
			///
			ShaderCache m_shaders;

			///
			/// Sound cache.
			///
			SoundCache m_sounds;

			///
			/// Music cache.
			///
			MusicCache m_music;

			///
			/// Backup of deserialization JSON.
			///
			nlohmann::json m_deserialization_json;
		};
	} // namespace state
} // namespace galaxy

#endif
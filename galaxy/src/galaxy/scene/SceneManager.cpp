///
/// SceneManager.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/math/Base64.hpp"
#include "galaxy/math/ZLib.hpp"
#include "galaxy/scripting/JSON.hpp"

#include "SceneManager.hpp"

namespace galaxy
{
	namespace scene
	{
		SceneManager::SceneManager() noexcept
		{
		}

		SceneManager::~SceneManager() noexcept
		{
			clear();
		}

		std::shared_ptr<Scene> SceneManager::create(const std::string& scene)
		{
			const auto hash = math::fnv1a(scene.c_str());
			if (!m_scenes.contains(hash))
			{
				m_scenes[hash] = std::make_shared<Scene>(scene);
				return m_scenes[hash];
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create scene '{0}'.", scene);
				return nullptr;
			}
		}

		void SceneManager::set(const std::string& scene)
		{
			pop_all();
			push(scene);
		}

		void SceneManager::push(const std::string& scene)
		{
			const auto hash = math::fnv1a(scene.c_str());
			m_stack.push_back(m_scenes[hash]);
			m_stack.back()->load();
		}

		void SceneManager::pop()
		{
			if (m_stack.size() > 0)
			{
				m_stack.back()->unload();
				m_stack.pop_back();
			}
		}

		void SceneManager::pop_all()
		{
			while (m_stack.size() > 0)
			{
				pop();
			}
		}

		std::shared_ptr<Scene> SceneManager::top() noexcept
		{
			return m_stack.back();
		}

		void SceneManager::update()
		{
			for (auto&& scene : m_stack)
			{
				scene->update();
			}
		}

		void SceneManager::render()
		{
			for (auto&& scene : m_stack)
			{
				scene->render();
			}
		}

		void SceneManager::load_app(const std::string& appdata_file)
		{
			std::ifstream in(appdata_file, std::ifstream::in);

			std::stringstream buffer;
			buffer << in.rdbuf();

			const auto data = buffer.str();
			if (!data.empty())
			{
				const auto decoded_zlib = math::decode_zlib(data);
				if (!decoded_zlib.empty())

				{
					const auto decoded_base64 = math::decode_base64(decoded_zlib);
					if (!decoded_base64.empty())
					{
						const auto parsed = nlohmann::json::parse(decoded_base64);

						if (!parsed.empty())
						{
							deserialize(parsed);
						}
						else
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to parse scenemanger JSON data from memory.");
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to decode base64 appdata '{0}'.", appdata_file);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to decode zlib appdata '{0}'.", appdata_file);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load appdata '{0}'.", appdata_file);
			}
		}

		void SceneManager::save_app(const std::string& file)
		{
			const auto json = serialize();

			if (!json::write(file, json))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to save '{0}' to disk.", file);
			}
		}

		void SceneManager::clear()
		{
			m_stack.clear();
			m_scenes.clear();
		}

		bool SceneManager::empty() const noexcept
		{
			return m_scenes.size() == 0;
		}

		nlohmann::json SceneManager::serialize()
		{
			nlohmann::json json = "{\"scenes\":{}}"_json;

			for (auto& [id, scene] : m_scenes)
			{
				json["scenes"][scene->name()] = scene->serialize();
			}

			for (auto i = 0; i < m_stack.size(); i++)
			{
				json["stack"][std::to_string(i)] = m_stack[i]->name();
			}

			return json;
		}

		void SceneManager::deserialize(const nlohmann::json& json)
		{
			pop_all();
			clear();

			const auto& scenes = json.at("scenes");
			m_scenes.reserve(scenes.size());
			for (const auto& [name, data] : scenes.items())
			{
				auto scene = create(name);
				if (scene)
				{
					scene->deserialize(data);
				}
			}

			const auto& stack = json.at("stack");
			m_stack.reserve(stack.size());
			for (const auto& [index, name] : stack.items())
			{
				const auto hash = math::fnv1a(name.get<std::string>().c_str());
				m_stack.insert(m_stack.begin() + std::stoi(index), m_scenes[hash]);
			}
		}
	} // namespace scene
} // namespace galaxy

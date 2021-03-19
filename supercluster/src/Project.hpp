///
/// Project.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PROJECT_HPP_
#define SUPERCLUSTER_PROJECT_HPP_

#include <galaxy/fs/Serializable.hpp>

#include "instances/Game.hpp"

using namespace galaxy;

namespace sc
{
	///
	/// Project data for editor to load.
	///
	class Project final : public fs::Serializable
	{
	public:
		Project() noexcept;
		Project(std::string_view path) noexcept;
		~Project();

		void new_project(std::string_view path);
		void load(std::string_view path);
		void save();
		void export_game();

		void set_path(std::string_view path) noexcept;

		const std::string& get_path() const noexcept;
		std::shared_ptr<core::Scene> get_top_scene();

		[[nodiscard]] nlohmann::json serialize() override;
		void deserialize(const nlohmann::json& json) override;

	public:
		std::unique_ptr<Game> m_game_instance;

	private:
		std::string m_path;
	};
} // namespace sc

#endif
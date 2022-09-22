///
/// Settings.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_SETTINGS_HPP_
#define SUPERCLUSTER_EDITOR_SETTINGS_HPP_

#include <span>

#include <nlohmann/json.hpp>
#include <robin_hood.h>

namespace sc
{
	class Settings final
	{
	public:
		Settings() noexcept;
		~Settings() noexcept;

		void load() noexcept;
		void render();
		void save() noexcept;

	private:
		void do_object(nlohmann::json& json);
		void do_array(nlohmann::json& json);
		void new_object(nlohmann::json& json);
		void add_to_array(nlohmann::json& json);

	private:
		int m_counter;
		nlohmann::json m_root;
	};
} // namespace sc

#endif
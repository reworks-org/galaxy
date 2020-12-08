///
/// JsonEditor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_JSONEDITOR_HPP_
#define GALAXY_JSONEDITOR_HPP_

#include <span>

#include <nlohmann/json.hpp>
#include <robin_hood.h>

///
/// Core namespace.
///
namespace galaxy
{
	class JsonEditor final
	{
	public:
		JsonEditor();
		~JsonEditor() = default;

		void create_new();

		void load_file(std::string_view file);
		void load_mem(std::span<char> memory);
		void load_json(nlohmann::json* json);

		void save(std::string_view path);

		void parse_and_display();

		[[nodiscard]] const bool is_loaded() const;

	private:
		unsigned long long m_counter;
		bool m_loaded;

		void do_object(nlohmann::json& json);
		void do_array(nlohmann::json& json);

		void new_object(nlohmann::json& json);
		void add_to_array(nlohmann::json& json);

		nlohmann::json m_root;
		nlohmann::json* m_external;
	};
} // namespace galaxy

#endif
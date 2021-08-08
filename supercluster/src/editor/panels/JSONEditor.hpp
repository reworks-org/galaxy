///
/// JSONEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_JSONEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_JSONEDITOR_HPP_

#include <span>

#include <nlohmann/json.hpp>
#include <robin_hood.h>

namespace sc
{
	namespace panel
	{
		class JSONEditor final
		{
		public:
			JSONEditor() noexcept;
			~JSONEditor() noexcept;

			void create_new();

			void load_file(std::string_view file);
			void load_mem(std::span<char> memory);
			void load_json(nlohmann::json* json);

			void save(std::string_view path);

			void render();

			[[nodiscard]] const bool is_loaded() const;

		private:
			void do_object(nlohmann::json& json);
			void do_array(nlohmann::json& json);
			void new_object(nlohmann::json& json);
			void add_to_array(nlohmann::json& json);

		private:
			int m_counter;
			bool m_loaded;
			nlohmann::json m_root;
			nlohmann::json* m_external;
		};
	} // namespace panel
} // namespace sc

#endif
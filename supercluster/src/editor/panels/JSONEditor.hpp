///
/// JSONEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_JSONEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_JSONEDITOR_HPP_

#include <span>

#include <ankerl/unordered_dense.h>
#include <nlohmann/json.hpp>

namespace sc
{
	namespace panel
	{
		class JSONEditor final
		{
		  public:
			JSONEditor();
			~JSONEditor();

			void create_new();

			void load(const std::string& file);
			void load_json(nlohmann::json* json);

			void save(const std::string& path);

			void render();

			[[nodiscard]] const bool is_loaded() const;

		  private:
			void do_object(nlohmann::json& json);
			void do_array(nlohmann::json& json);
			void new_object(nlohmann::json& json);
			void add_to_array(nlohmann::json& json);

		  private:
			int             m_counter;
			bool            m_loaded;
			nlohmann::json  m_root;
			nlohmann::json* m_external;
		};
	} // namespace panel
} // namespace sc

#endif

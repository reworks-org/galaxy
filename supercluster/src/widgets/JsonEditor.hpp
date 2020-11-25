///
/// JsonEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_JSONEDITOR_HPP_
#define SUPERCLUSTER_JSONEDITOR_HPP_

#include <span>

#include <nlohmann/json.hpp>

namespace sc
{
	class JsonEditor final
	{
	public:
		JsonEditor();

		void load(std::string_view file);
		void load(std::span<char> memory);

		void save(std::string_view path);

		void parse_and_display();

		[[nodiscard]] const bool is_loaded() const;

	private:
		bool m_loaded;

		void do_object(nlohmann::json& json);
		void do_array(nlohmann::json& json);

		nlohmann::json m_root;
	};

} // namespace sc

#endif
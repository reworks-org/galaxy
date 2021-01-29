///
/// JSON.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_JSON_HPP_
#define SUPERCLUSTER_EDITOR_JSON_HPP_

#include <span>

#include <nlohmann/json.hpp>
#include <robin_hood.h>

namespace sc
{
	namespace editor
	{
		///
		/// Parses json and provides imgui widgets to manipulate it.
		///
		class JSON final
		{
		public:
			///
			/// Constructor.
			///
			JSON();

			///
			/// Destructor.
			///
			~JSON() = default;

			///
			/// Create a blank json object.
			///
			void create_new();

			///
			/// Load external json.
			///
			/// \param file Filepath to load.
			///
			void load_file(std::string_view file);

			///
			/// Load from memory instead.
			///
			/// \param memory Load json from memory instead of disk.
			///
			void load_mem(std::span<char> memory);

			///
			/// Load an existing json object.
			///
			/// \param json Pointer to existing json object. Ownership is not taken.
			///
			void load_json(nlohmann::json* json);

			///
			/// Save JSON to disk.
			///
			/// \param file File to save file at.
			///
			void save(std::string_view path);

			///
			/// Parse loaded json and display using ImGui.
			///
			void parse_and_display();

			///
			/// Check if json has been loaded before displaying.
			///
			/// \return Const bool. True if loaded.
			///
			[[nodiscard]] const bool is_loaded() const;

		private:
			///
			/// Counter to identify arrays with.
			///
			unsigned long long m_counter;

			///
			/// Is loaded flag.
			///
			bool m_loaded;

			///
			/// Internal function to process a json object.
			///
			/// \param json Json data.
			///
			void do_object(nlohmann::json& json);

			///
			/// Internal function to process a json array.
			///
			/// \param json Json data.
			///
			void do_array(nlohmann::json& json);

			///
			/// Internal function to create a new object in json.
			///
			/// \param json Json data.
			///
			void new_object(nlohmann::json& json);

			///
			/// Internal function to add a new value to a json array.
			///
			/// \param json Json data.
			///
			void add_to_array(nlohmann::json& json);

			///
			/// Loaded json object being manipulated.
			///
			nlohmann::json m_root;

			///
			/// Only used when json is loaded from external object.
			///
			nlohmann::json* m_external;
		};
	} // namespace editor
} // namespace sc

#endif
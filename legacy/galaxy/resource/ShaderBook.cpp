///
/// ShaderBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "ShaderBook.hpp"

namespace galaxy
{
	namespace res
	{
		ShaderBook::ShaderBook()
			: Serializable {this}
			, m_vert_ext {".vs"}
			, m_frag_ext {".fs"}
		{
		}

		ShaderBook::ShaderBook(std::string_view file)
			: Serializable {this}
		{
			create_from_json(file);
		}

		ShaderBook::~ShaderBook() noexcept
		{
			clear();
		}

		void ShaderBook::create_from_json(std::string_view file)
		{
			const auto json_opt = json::parse_from_disk(file);
			if (json_opt == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file: {0}, for Shaderbook.", file);
			}
			else
			{
				deserialize(json_opt.value());
			}
		}

		void ShaderBook::clear() noexcept
		{
			m_resources.clear();
		}

		nlohmann::json ShaderBook::serialize()
		{
			nlohmann::json json        = "{\"shaderbook\":[]}"_json;
			json["vertex-extension"]   = m_vert_ext;
			json["fragment-extension"] = m_frag_ext;

			for (const auto& [name, shader] : m_resources)
			{
				json["shaderbook"].push_back(name);
			}

			return json;
		}

		void ShaderBook::deserialize(const nlohmann::json& json)
		{
			clear();

			m_vert_ext = json.at("vertex-extension");
			m_frag_ext = json.at("fragment-extension");

			const auto& arr = json.at("shaderbook");
			for (const std::string& filename : arr)
			{
				create(filename, filename + m_vert_ext, filename + m_frag_ext);
			}
		}
	} // namespace res
} // namespace galaxy
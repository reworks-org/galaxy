///
/// RML.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "RML.hpp"

namespace galaxy
{
	namespace components
	{
		RML::RML()
			: Serializable {}
			, m_file {""}
			, m_doc {nullptr}
		{
		}

		RML::RML(const nlohmann::json& json)
			: Serializable {}
			, m_file {""}
			, m_doc {nullptr}
		{
			deserialize(json);
		}

		RML::RML(RML* ptr)
			: Serializable {}
			, m_file {""}
			, m_doc {nullptr}
		{
			const auto& copy = *ptr;

			this->m_file = copy.m_file;
			this->m_doc  = copy.m_doc;
		}

		RML::RML(RML&& t)
			: Serializable {}
			, m_file {""}
			, m_doc {nullptr}
		{
			this->m_file = std::move(t.m_file);
			this->m_doc  = t.m_doc;

			t.m_doc = nullptr;
		}

		RML& RML::operator=(RML&& t)
		{
			if (this != &t)
			{
				this->m_file = std::move(t.m_file);
				this->m_doc  = t.m_doc;

				t.m_doc = nullptr;
			}

			return *this;
		}

		RML::~RML()
		{
		}

		nlohmann::json RML::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["file"]        = m_file;

			return json;
		}

		void RML::deserialize(const nlohmann::json& json)
		{
			m_file = json.at("file");
		}
	} // namespace components
} // namespace galaxy
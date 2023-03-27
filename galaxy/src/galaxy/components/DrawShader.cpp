///
/// DrawShader.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/Shaders.hpp"

#include "DrawShader.hpp"

namespace galaxy
{
	namespace components
	{
		DrawShader::DrawShader()
			: Serializable {}
			, m_shader {nullptr}
		{
		}

		DrawShader::DrawShader(const nlohmann::json& json)
			: Serializable {}
			, m_shader {nullptr}
		{
			deserialize(json);
		}

		DrawShader::DrawShader(DrawShader&& ds)
			: Serializable {}
			, m_shader {nullptr}
		{
			if (this->m_shader != nullptr)
			{
				this->m_shader = nullptr;
			}

			this->m_shader = ds.m_shader;
			ds.m_shader    = nullptr;
		}

		DrawShader& DrawShader::operator=(DrawShader&& ds)
		{
			if (this != &ds)
			{
				if (this->m_shader != nullptr)
				{
					this->m_shader = nullptr;
				}

				this->m_shader = ds.m_shader;
				ds.m_shader    = nullptr;
			}

			return *this;
		}

		DrawShader::~DrawShader()
		{
			m_shader = nullptr;
		}

		void DrawShader::set_shader(const std::string& id)
		{
			auto& shaders = core::ServiceLocator<resource::Shaders>::ref();

			m_shader = shaders.get(id);
			m_id     = id;
		}

		const std::string& DrawShader::id() const
		{
			return m_id;
		}

		nlohmann::json DrawShader::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["shader"]      = m_id;

			return json;
		}

		void DrawShader::deserialize(const nlohmann::json& json)
		{
			set_shader(json.at("shader"));
		}
	} // namespace components
} // namespace galaxy
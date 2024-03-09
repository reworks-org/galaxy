///
/// RenderCommand.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "RenderCommand.hpp"

namespace galaxy
{
	namespace components
	{
		RenderCommand::RenderCommand()
			: Serializable {}
		{
		}

		RenderCommand::RenderCommand(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		RenderCommand::RenderCommand(RenderCommand&& r)
			: Serializable {}
		{
			this->m_command.count              = r.m_command.count;
			this->m_command.instances          = r.m_command.instances;
			this->m_command.layer              = r.m_command.layer;
			this->m_command.mode               = r.m_command.mode;
			this->m_command.offset             = r.m_command.offset;
			this->m_command.vao                = r.m_command.vao;
			this->m_command.uniforms.colour    = std::move(r.m_command.uniforms.colour);
			this->m_command.uniforms.entity    = r.m_command.uniforms.entity;
			this->m_command.uniforms.handle    = r.m_command.uniforms.handle;
			this->m_command.uniforms.point     = r.m_command.uniforms.point;
			this->m_command.uniforms.transform = std::move(r.m_command.uniforms.transform);
		}

		RenderCommand& RenderCommand::operator=(RenderCommand&& r)
		{
			if (this != &r)
			{
				this->m_command.count              = r.m_command.count;
				this->m_command.instances          = r.m_command.instances;
				this->m_command.layer              = r.m_command.layer;
				this->m_command.mode               = r.m_command.mode;
				this->m_command.offset             = r.m_command.offset;
				this->m_command.vao                = r.m_command.vao;
				this->m_command.uniforms.colour    = std::move(r.m_command.uniforms.colour);
				this->m_command.uniforms.entity    = r.m_command.uniforms.entity;
				this->m_command.uniforms.handle    = r.m_command.uniforms.handle;
				this->m_command.uniforms.point     = r.m_command.uniforms.point;
				this->m_command.uniforms.transform = std::move(r.m_command.uniforms.transform);
			}

			return *this;
		}

		RenderCommand::~RenderCommand()
		{
		}

		nlohmann::json RenderCommand::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["layer"]       = m_command.layer;

			return json;
		}

		void RenderCommand::deserialize(const nlohmann::json& json)
		{
			m_command.layer = json.at("layer");
		}
	} // namespace components
} // namespace galaxy

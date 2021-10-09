///
/// ShadowMap.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/Config.hpp"

#include "ShadowMap.hpp"

///
/// ShadowMap vertex shader.
///
constexpr const char* const shadowmap_vert = R"(
	#version 450 core
	layout(location = 0) in vec3 l_pos;

	struct DirLight
	{
		vec3 ambient_intensity;
		vec3 diffuse_intensity;
		vec3 specular_intensity;

		vec3 dir;

		mat4 view;
		mat4 proj;

		float near;
		float far;
    };

	layout (std430, binding = 1) readonly buffer dir_light_data
    {
		DirLight b_dir_light;
	};

	void main()
	{
		gl_Position = b_dir_light.proj * b_dir_light.view * vec4(l_pos, 1.0);
	}
)";

///
/// ShadowMap vertex shader.
///
constexpr const char* const shadowmap_frag = R"(
	#version 450 core

	void main()
	{
		gl_FragDepth = gl_FragCoord.z;
	}
)";

namespace galaxy
{
	namespace graphics
	{
		ShadowMap::ShadowMap()
		{
			m_shadow_size = SL_HANDLE.config()->get<int>("shadow-size");

			glGenFramebuffers(1, &m_fbo);
			glGenTextures(1, &m_depth_tex);

			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			glBindTexture(GL_TEXTURE_2D, m_depth_tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_shadow_size, m_shadow_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glGenerateMipmap(GL_TEXTURE_2D);

			if (SL_HANDLE.config()->get<bool>("trilinear-filtering"))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(SL_HANDLE.config()->get<int>("ansio-filter")));

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_tex, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				std::string reason = "";
				switch (status)
				{
					case GL_FRAMEBUFFER_UNDEFINED:
						reason = "GL_FRAMEBUFFER_UNDEFINED";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
						break;
					case GL_FRAMEBUFFER_UNSUPPORTED:
						reason = "GL_FRAMEBUFFER_UNSUPPORTED";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
						reason = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
						break;
				}

				GALAXY_LOG(GALAXY_FATAL, "Failed to complete shadowmap framebuffer: {0}.", reason);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			m_shadowmap_pass.load_raw(shadowmap_vert, shadowmap_frag);
		}

		ShadowMap::~ShadowMap()
		{
			glDeleteFramebuffers(1, &m_fbo);
			glDeleteTextures(1, &m_depth_tex);
		}

		void ShadowMap::prepare() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void ShadowMap::bind() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			glDisable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			glViewport(0, 0, m_shadow_size, m_shadow_size);

			m_shadowmap_pass.bind();
		}

		void ShadowMap::unbind() noexcept
		{
			m_shadowmap_pass.unbind();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		const unsigned int ShadowMap::get_texture() const noexcept
		{
			return m_depth_tex;
		}
	} // namespace graphics
} // namespace galaxy
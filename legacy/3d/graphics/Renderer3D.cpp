///
/// Renderer3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/gtc/matrix_inverse.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/graphics/Mesh.hpp"
#include "galaxy/graphics/Skybox.hpp"
#include "galaxy/graphics/light/Material.hpp"
#include "galaxy/graphics/light/Spot.hpp"
#include "galaxy/graphics/light/Object.hpp"
#include "galaxy/graphics/Shader.hpp"

#include "Renderer3D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Renderer3D::Renderer3D() noexcept
		    : m_hbao_context {nullptr}, m_screen_vbo {0}, m_screen_vao {0}, m_bound_fb {0}
		{
		}

		Renderer3D::~Renderer3D() noexcept
		{
			clean_up();
		}

		Renderer3D& Renderer3D::inst() noexcept
		{
			static Renderer3D s_inst;
			return s_inst;
		}

		void Renderer3D::init(const int width, const int height)
		{
			// clang-format off
			constexpr const std::array<float, 20> verticies =
			{
				// First 3 are Pos, last 2 are Texels.
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
			};
			// clang-format on

			glGenVertexArrays(1, &m_screen_vao);
			glGenBuffers(1, &m_screen_vbo);
			glBindVertexArray(m_screen_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);
			glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Configure HBAO+.
			m_hbao_heap.new_    = ::operator new;
			m_hbao_heap.delete_ = ::operator delete;

			m_hbao_funcs.glad_glActiveTexture           = glActiveTexture;
			m_hbao_funcs.glad_glAttachShader            = glAttachShader;
			m_hbao_funcs.glad_glBindBuffer              = glBindBuffer;
			m_hbao_funcs.glad_glBindBufferBase          = glBindBufferBase;
			m_hbao_funcs.glad_glBindFramebuffer         = glBindFramebuffer;
			m_hbao_funcs.glad_glBindFragDataLocation    = glBindFragDataLocation;
			m_hbao_funcs.glad_glBindTexture             = glBindTexture;
			m_hbao_funcs.glad_glBindVertexArray         = glBindVertexArray;
			m_hbao_funcs.glad_glBlendColor              = glBlendColor;
			m_hbao_funcs.glad_glBlendEquationSeparate   = glBlendEquationSeparate;
			m_hbao_funcs.glad_glBlendFuncSeparate       = glBlendFuncSeparate;
			m_hbao_funcs.glad_glBufferData              = glBufferData;
			m_hbao_funcs.glad_glBufferSubData           = glBufferSubData;
			m_hbao_funcs.glad_glColorMaski              = glColorMaski;
			m_hbao_funcs.glad_glCompileShader           = glCompileShader;
			m_hbao_funcs.glad_glCreateShader            = glCreateShader;
			m_hbao_funcs.glad_glCreateProgram           = glCreateProgram;
			m_hbao_funcs.glad_glDeleteBuffers           = glDeleteBuffers;
			m_hbao_funcs.glad_glDeleteFramebuffers      = glDeleteFramebuffers;
			m_hbao_funcs.glad_glDeleteProgram           = glDeleteProgram;
			m_hbao_funcs.glad_glDeleteShader            = glDeleteShader;
			m_hbao_funcs.glad_glDeleteTextures          = glDeleteTextures;
			m_hbao_funcs.glad_glDeleteVertexArrays      = glDeleteVertexArrays;
			m_hbao_funcs.glad_glDisable                 = glDisable;
			m_hbao_funcs.glad_glDrawBuffers             = glDrawBuffers;
			m_hbao_funcs.glad_glEnable                  = glEnable;
			m_hbao_funcs.glad_glDrawArrays              = glDrawArrays;
			m_hbao_funcs.glad_glFramebufferTexture      = glFramebufferTexture;
			m_hbao_funcs.glad_glFramebufferTexture2D    = glFramebufferTexture2D;
			m_hbao_funcs.glad_glFramebufferTextureLayer = glFramebufferTextureLayer;
			m_hbao_funcs.glad_glGenBuffers              = glGenBuffers;
			m_hbao_funcs.glad_glGenFramebuffers         = glGenFramebuffers;
			m_hbao_funcs.glad_glGenTextures             = glGenTextures;
			m_hbao_funcs.glad_glGenVertexArrays         = glGenVertexArrays;
			m_hbao_funcs.glad_glGetError                = glGetError;
			m_hbao_funcs.glad_glGetBooleani_v           = glGetBooleani_v;
			m_hbao_funcs.glad_glGetFloatv               = glGetFloatv;
			m_hbao_funcs.glad_glGetIntegerv             = glGetIntegerv;
			m_hbao_funcs.glad_glGetIntegeri_v           = glGetIntegeri_v;
			m_hbao_funcs.glad_glGetProgramiv            = glGetProgramiv;
			m_hbao_funcs.glad_glGetProgramInfoLog       = glGetProgramInfoLog;
			m_hbao_funcs.glad_glGetShaderiv             = glGetShaderiv;
			m_hbao_funcs.glad_glGetShaderInfoLog        = glGetShaderInfoLog;
			m_hbao_funcs.glad_glGetString               = glGetString;
			m_hbao_funcs.glad_glGetUniformBlockIndex    = glGetUniformBlockIndex;
			m_hbao_funcs.glad_glGetUniformLocation      = glGetUniformLocation;
			m_hbao_funcs.glad_glGetTexLevelParameteriv  = glGetTexLevelParameteriv;
			m_hbao_funcs.glad_glIsEnabled               = glIsEnabled;
			m_hbao_funcs.glad_glIsEnabledi              = glIsEnabledi;
			m_hbao_funcs.glad_glLinkProgram             = glLinkProgram;
			m_hbao_funcs.glad_glPolygonOffset           = glPolygonOffset;
			m_hbao_funcs.glad_glShaderSource            = glShaderSource;
			m_hbao_funcs.glad_glTexImage2D              = glTexImage2D;
			m_hbao_funcs.glad_glTexImage3D              = glTexImage3D;
			m_hbao_funcs.glad_glTexParameteri           = glTexParameteri;
			m_hbao_funcs.glad_glTexParameterfv          = glTexParameterfv;
			m_hbao_funcs.glad_glUniform1i               = glUniform1i;
			m_hbao_funcs.glad_glUniformBlockBinding     = glUniformBlockBinding;
			m_hbao_funcs.glad_glUseProgram              = glUseProgram;
			m_hbao_funcs.glad_glViewport                = glViewport;

			GFSDK_SSAO_Status status;
			status = GFSDK_SSAO_CreateContext_GL(&m_hbao_context, &m_hbao_funcs, &m_hbao_heap);
			if (status != GFSDK_SSAO_OK)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to create HBAO+ context.");
			}
			else
			{
				const auto fov    = SL_HANDLE.config()->get<float>("camera-fov");
				const float ratio = static_cast<float>(static_cast<double>(SL_HANDLE.window()->get_width()) / static_cast<double>(SL_HANDLE.window()->get_height()));
				const auto near   = SL_HANDLE.config()->get<float>("camera-near");
				const auto far    = SL_HANDLE.config()->get<float>("camera-far");

				m_hbao_inputdata.DepthData.DepthTextureType              = GFSDK_SSAO_HARDWARE_DEPTHS;
				m_hbao_inputdata.DepthData.FullResDepthTexture.Target    = GL_TEXTURE_2D;
				m_hbao_inputdata.DepthData.FullResDepthTexture.TextureId = m_gbuffer.get_depth_tex();
				m_hbao_inputdata.DepthData.MetersToViewSpaceUnits        = 1.0f;
				m_hbao_inputdata.DepthData.ProjectionMatrix.Data         = GFSDK_SSAO_Float4x4(glm::value_ptr(glm::perspective(glm::radians(fov), ratio, near, far)));
				m_hbao_inputdata.DepthData.ProjectionMatrix.Layout       = GFSDK_SSAO_ROW_MAJOR_ORDER;
				m_hbao_inputdata.DepthData.Viewport.Enable               = false;

				m_hbao_inputdata.NormalData.Enable = false;

				m_hbao_params.BackgroundAO.Enable              = true;
				m_hbao_params.BackgroundAO.BackgroundViewDepth = SL_HANDLE.config()->get<float>("camera-far");
				m_hbao_params.Bias                             = 0.0f;
				m_hbao_params.Blur.Enable                      = true;
				m_hbao_params.Blur.Radius                      = GFSDK_SSAO_BLUR_RADIUS_4;
				m_hbao_params.Blur.Sharpness                   = 0.0f;
				m_hbao_params.DepthClampMode                   = GFSDK_SSAO_CLAMP_TO_EDGE;
				m_hbao_params.DepthStorage                     = GFSDK_SSAO_FP32_VIEW_DEPTHS;
				m_hbao_params.DepthThreshold.Enable            = false;
				m_hbao_params.ForegroundAO.Enable              = false;
				m_hbao_params.LargeScaleAO                     = 1.0f;
				m_hbao_params.PowerExponent                    = 2.0f;
				m_hbao_params.Radius                           = 4.0f;
				m_hbao_params.SmallScaleAO                     = 1.0f;

				m_hbao_output.Blend.Mode = GFSDK_SSAO_OVERWRITE_RGB;
				m_hbao_context->PreCreateFBOs(m_hbao_params, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height());
			}
		}

		void Renderer3D::add_lighting_shader(const std::string& vs, const std::string& fs)
		{
			m_light_pass.load_raw(vs, fs);
			m_light_pass.bind();
			m_light_pass.set_uniform("u_pos", 0);
			m_light_pass.set_uniform("u_diffuse", 1);
			m_light_pass.set_uniform("u_specular", 2);
			m_light_pass.set_uniform("u_normals", 3);
			m_light_pass.set_uniform("u_shadowmap", 4);
			m_light_pass.unbind();
		}

		void Renderer3D::prepare() noexcept
		{
			m_shadowmap.prepare();
			m_gbuffer.prepare();
		}

		void Renderer3D::bind_shadowpass() noexcept
		{
			m_shadowmap.bind();
		}

		void Renderer3D::bind_geompass() noexcept
		{
			m_gbuffer.bind();
		}

		void Renderer3D::unbind_passes() noexcept
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glUseProgram(0);
		}

		void Renderer3D::render()
		{
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_bound_fb);
			m_hbao_output.OutputFBO = m_bound_fb;
			m_hbao_context->RenderAO(m_hbao_inputdata, m_hbao_params, m_hbao_output, GFSDK_SSAO_RENDER_AO);

			glBindVertexArray(m_screen_vao);
			const auto& attachments = m_gbuffer.get_attachments();

			// Lighting Texture Pass.
			int index = 0;
			for (auto& texture : attachments)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(GL_TEXTURE_2D, texture);

				index++;
			}

			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, m_shadowmap.get_texture());
			index++;

			// Lighting Shader Pass.
			m_light_pass.bind();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			while (index >= 0)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(GL_TEXTURE_2D, 0);

				index--;
			}

			glUseProgram(0);
			glBindVertexArray(0);

			// Copy depth buffer info.
			// clang-format off
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gbuffer.get_fbo());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_bound_fb);
			glBlitFramebuffer(0, 0, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(),
				0, 0, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height(),
				GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, m_bound_fb);
			// clang-format on
		}

		void Renderer3D::resize(const int width, const int height)
		{
			m_gbuffer.resize(width, height);
			m_hbao_context->PreCreateFBOs(m_hbao_params, width, height);
		}

		void Renderer3D::reserve_ssbo(const unsigned int index, const unsigned int size)
		{
			bind_ssbo(index);

			GLint64 buffer_size = 0;
			glGetBufferParameteri64v(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &buffer_size);

			if (buffer_size != size)
			{
				glInvalidateBufferData(m_buffers[index]);
				glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			}

			unbind_ssbo();
		}

		void Renderer3D::do_forward_render()
		{
			for (const auto& call : m_forward_calls)
			{
				call();
			}

			m_forward_calls.clear();
		}

		void Renderer3D::reserve_ubo(const unsigned int index, const unsigned int size)
		{
			bind_ubo(index);

			GLint64 buffer_size = 0;
			glGetBufferParameteri64v(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &buffer_size);

			if (buffer_size != size)
			{
				glInvalidateBufferData(m_buffers[index]);
				glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			}

			unbind_ubo();
		}

		void Renderer3D::bind_ubo(const unsigned int index)
		{
			if ((static_cast<std::size_t>(index) + 1) > m_buffers.size())
			{
				unsigned int buff = 0;

				glGenBuffers(1, &buff);
				glBindBuffer(GL_UNIFORM_BUFFER, buff);
				glBindBufferBase(GL_UNIFORM_BUFFER, index, buff);

				m_buffers.resize(static_cast<std::size_t>(index) + 1, 0);
				m_buffers[index] = buff;
			}
			else
			{
				glBindBuffer(GL_UNIFORM_BUFFER, m_buffers[index]);
			}
		}

		void Renderer3D::unbind_ubo()
		{
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void Renderer3D::bind_ssbo(const unsigned int index)
		{
			if ((static_cast<std::size_t>(index) + 1) > m_buffers.size())
			{
				unsigned int buff = 0;

				glGenBuffers(1, &buff);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, buff);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buff);

				m_buffers.resize(static_cast<std::size_t>(index) + 1, 0);
				m_buffers[index] = buff;
			}
			else
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffers[index]);
			}
		}

		void Renderer3D::unbind_ssbo()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

		void Renderer3D::draw_mesh_deferred(Mesh* mesh, light::Material* material)
		{
			mesh->bind();

			if (material->m_use_diffuse_texture)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, material->m_diffuse.gl_texture());
				m_gbuffer.m_shader.set_uniform("material.diffuse", 0);
			}

			if (material->m_use_specular_texture)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, material->m_specular.gl_texture());
				m_gbuffer.m_shader.set_uniform("material.specular", 1);
			}

			if (material->m_use_normal_texture)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, material->m_normal.gl_texture());
				m_gbuffer.m_shader.set_uniform("material.normal", 2);
			}

			m_gbuffer.m_shader.set_uniform("material.shininess", material->m_shininess);
			m_gbuffer.m_shader.set_uniform("material.diffuse_colours", material->m_diffuse_colours);
			m_gbuffer.m_shader.set_uniform("material.specular_colours", material->m_specular_colours);
			m_gbuffer.m_shader.set_uniform("material.use_diffuse_texture", material->m_use_diffuse_texture);
			m_gbuffer.m_shader.set_uniform("material.use_specular_texture", material->m_use_specular_texture);
			m_gbuffer.m_shader.set_uniform("material.use_normal_texture", material->m_use_normal_texture);

			glDrawElements(GL_TRIANGLES, mesh->index_count(), GL_UNSIGNED_INT, nullptr);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Renderer3D::draw_mesh_shadows(Mesh* mesh)
		{
			mesh->bind();
			glDrawElements(GL_TRIANGLES, mesh->index_count(), GL_UNSIGNED_INT, nullptr);
		}

		void Renderer3D::draw(Skybox* skybox, Shader* shader)
		{
			shader->bind();
			skybox->bind();

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		void Renderer3D::draw(light::Object* light, Shader* shader)
		{
			SL_HANDLE.window()->disable_culling();

			shader->bind();
			shader->set_uniform("u_transform", light->get_transform());

			light->bind();
			glDrawArrays(GL_TRIANGLES, 0, light->get_count());
		}

		void Renderer3D::clean_up()
		{
			if (!m_buffers.empty())
			{
				glDeleteBuffers(m_buffers.size(), m_buffers.data());
				m_buffers.clear();
			}

			m_gbuffer.destroy();

			glDeleteVertexArrays(1, &m_screen_vao);
			glDeleteBuffers(1, &m_screen_vbo);

			m_screen_vbo = 0;
			m_screen_vao = 0;

			if (m_hbao_context != nullptr)
			{
				m_hbao_context->Release();
				m_hbao_context = nullptr;
			}
		}

		GeomBuffer& Renderer3D::get_gbuffer() noexcept
		{
			return m_gbuffer;
		}
	} // namespace graphics
} // namespace galaxy
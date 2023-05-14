///
/// Video.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/DefaultShaders.hpp"

#include "Video.hpp"

thread_local const float vertices[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

thread_local const unsigned int indices[] = {0, 1, 2, 0, 2, 3};

namespace galaxy
{
	namespace media
	{
		Video::Video()
			: m_plm {nullptr}
			, m_texture_y {0}
			, m_texture_cb {0}
			, m_texture_cr {0}
		{
			glCreateVertexArrays(1, &m_vao);
			glCreateBuffers(1, &m_vbo);
			glCreateBuffers(1, &m_ibo);

			glNamedBufferData(m_vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glNamedBufferData(m_ibo, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, 2 * sizeof(float));
			glVertexArrayElementBuffer(m_vao, m_ibo);

			glEnableVertexArrayAttrib(m_vao, 0);
			glVertexArrayAttribFormat(m_vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(m_vao, 0, 0);
		}

		Video::~Video()
		{
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(1, &m_vbo);
			glDeleteBuffers(1, &m_ibo);

			if (m_plm)
			{
				plm_destroy(m_plm);
				m_plm = nullptr;
			}

			if (m_texture_y != 0)
			{
				glDeleteTextures(1, &m_texture_y);
			}

			if (m_texture_cb != 0)
			{
				glDeleteTextures(1, &m_texture_cb);
			}

			if (m_texture_cr != 0)
			{
				glDeleteTextures(1, &m_texture_cr);
			}
		}

		void Video::load(const std::string& file)
		{
			auto info = core::ServiceLocator<fs::VirtualFileSystem>::ref().find(file);
			if (info.code == fs::FileCode::FOUND)
			{
				m_plm = plm_create_with_filename(info.string.c_str());
				if (m_plm)
				{
					plm_set_video_decode_callback(
						m_plm,
						[](plm_t* self, plm_frame_t* frame, void* user) {
							auto video = reinterpret_cast<Video*>(user);

							video->update_texture(GL_TEXTURE0, video->m_texture_y, &frame->y);
							video->update_texture(GL_TEXTURE1, video->m_texture_cb, &frame->cb);
							video->update_texture(GL_TEXTURE2, video->m_texture_cr, &frame->cr);
						},
						this);

					plm_set_audio_decode_callback(
						m_plm,
						[](plm_t* self, plm_samples_t* samples, void* user) {
						},
						this);

					plm_set_loop(m_plm, false);
					plm_set_audio_enabled(m_plm, false);
					plm_set_audio_stream(m_plm, 0);

					m_texture_y  = create_texture(0, "texture_y");
					m_texture_cb = create_texture(1, "texture_cb");
					m_texture_cr = create_texture(2, "texture_cr");
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load video '{0}'.", file);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find video '{0}' because '{1}'.", file, magic_enum::enum_name(info.code));
			}
		}

		void Video::enable_loop(const bool loop) const
		{
			if (m_plm)
			{
				plm_set_loop(m_plm, loop);
			}
		}

		void Video::update()
		{
			if (m_plm)
			{
				plm_decode(m_plm, GALAXY_DT);
			}
		}

		void Video::render()
		{
			m_shader.bind();
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
		}

		bool Video::is_finished() const
		{
			if (m_plm)
			{
				return plm_has_ended(m_plm);
			}

			return true;
		}

		double Video::get_framerate() const
		{
			if (m_plm)
			{
				return plm_get_framerate(m_plm);
			}

			return 0.0;
		}

		int Video::get_samplerate() const
		{
			if (m_plm)
			{
				return plm_get_samplerate(m_plm);
			}

			return 0;
		}

		double Video::get_duration() const
		{
			if (m_plm)
			{
				return plm_get_duration(m_plm);
			}

			return 0.0;
		}

		unsigned int Video::create_texture(unsigned int index, const char* uniform)
		{
			unsigned int texture;
			glCreateTextures(GL_TEXTURE_2D, 1, &texture);

			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			GLint swizzleMask[] = {GL_ZERO, GL_ZERO, GL_ZERO, GL_RED};
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

			glBindTexture(GL_TEXTURE_2D, 0);

			m_shader.set_uniform(uniform, index);

			return texture;
		}

		void Video::update_texture(unsigned int unit, unsigned int texture, plm_plane_t* plane)
		{
			glActiveTexture(unit);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, plane->width, plane->height, 0, GL_RED, GL_UNSIGNED_BYTE, plane->data);
		}
	} // namespace media
} // namespace galaxy
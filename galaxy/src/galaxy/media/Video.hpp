///
/// Video.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MEDIA_VIDEO_HPP_
#define GALAXY_MEDIA_VIDEO_HPP_

#include <miniaudio.h>
#include <pl_mpeg.h>

#include "galaxy/graphics/Shader.hpp"
#include "galaxy/media/RingBuffer.hpp"

namespace galaxy
{
	namespace media
	{
		///
		/// \brief Plays an MPEG-1 video.
		///
		/// No audio support yet.
		///
		class Video final
		{
		  public:
			///
			/// Constructor.
			///
			Video();

			///
			/// Destructor.
			///
			~Video();

			///
			/// Load a MPEG-1 from video file.
			///
			/// \param file File to load.
			///
			void load(const std::string& file);

			///
			/// Build opengl data.
			///
			void build();

			///
			/// Update video decoder.
			///
			void update();

			///
			/// Render video.
			///
			void render();

			///
			/// Check if video is finished.
			///
			/// \return True if finished.
			///
			[[nodiscard]] bool is_finished() const;

			///
			/// Get current playtime.
			///
			/// \return In seconds.
			///
			[[nodiscard]] double get_time() const;

			///
			/// Get video framerate.
			///
			/// \return Double.
			///
			[[nodiscard]] double get_framerate() const;

			///
			/// Get video sample rate.
			///
			/// \return Int.
			///
			[[nodiscard]] int get_samplerate() const;

			///
			/// Get video duration.
			///
			/// \return Double.
			///
			[[nodiscard]] double get_duration() const;

		  private:
			///
			/// Internal function to generate video frame texture.
			///
			/// \param index Sampler index.
			/// \param uniform Shader uniform index.
			///
			/// \return Returns created texture id from OpenGL.
			///
			unsigned int create_texture(unsigned int index, const char* uniform);

			///
			/// Internal function to update video frame data.
			///
			/// \param unit Texture unit.
			/// \param texture GL texture handle.
			/// \param plane PLM decoder data.
			///
			void update_texture(unsigned int unit, unsigned int texture, const plm_plane_t* plane);

		  private:
			///
			/// PLM decoder data.
			///
			plm_t* m_plm;

			///
			/// Audio data.
			///
			ma_device* m_audio;

			///
			/// Video data buffer.
			///
			meta::vector<uint8_t> m_buffer;

			///
			/// Buffer for audio decoding.
			///
			std::unique_ptr<RingBuffer> m_audio_buffer;

			///
			/// PLM video frame y.
			///
			unsigned int m_texture_y;

			///
			/// PLM video frame cb.
			///
			unsigned int m_texture_cb;

			///
			/// PLM video frame cr.
			///
			unsigned int m_texture_cr;

			///
			/// PLM video frame vertex array object.
			///
			unsigned int m_vao;

			///
			/// PLM video frame vertex buffer.
			///
			unsigned int m_vbo;

			///
			/// PLM video frame index buffer.
			///
			unsigned int m_ibo;

			///
			/// PLM video shader.
			///
			graphics::Shader m_shader;
		};
	} // namespace media
} // namespace galaxy

#endif

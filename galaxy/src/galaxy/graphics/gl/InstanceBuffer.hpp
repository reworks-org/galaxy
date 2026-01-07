///
/// InstanceBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_INSTANCEBUFFER_HPP_
#define GALAXY_GRAPHICS_GL_INSTANCEBUFFER_HPP_

#include <span>

#include <glm/mat4x4.hpp>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Abstraction for OpenGL vertex buffer objects.
		///
		class InstanceBuffer final
		{
		  public:
			///
			/// Constructor.
			///
			InstanceBuffer();

			///
			/// Move constructor.
			///
			InstanceBuffer(InstanceBuffer&&);

			///
			/// Move assignment operator.
			///
			InstanceBuffer& operator=(InstanceBuffer&&);

			///
			/// Destructor.
			///
			~InstanceBuffer();

			///
			/// Create instance buffer.
			///
			/// \param transforms Transform to apply to each instance.
			///
			void buffer(std::span<glm::mat4> transforms);

			///
			/// Sub-buffer instances.
			///
			/// \param index Offset to start from. 0 = first element.
			/// \param transforms Transforms to upload.
			///
			void sub_buffer(const unsigned int index, std::span<glm::mat4> transforms);

			///
			/// Clear buffer data.
			///
			void clear();

			///
			/// Get the amount of instances.
			///
			/// \return Integer.
			///
			[[nodiscard]] int amount() const;

			///
			/// Get OpenGL handle.
			///
			/// \return Unsigned integer.
			///
			[[nodiscard]] unsigned int id() const;

		  private:
			///
			/// Copy constructor.
			///
			InstanceBuffer(const InstanceBuffer&) = delete;
			///
			/// Copy assignment operator.
			///
			InstanceBuffer& operator=(const InstanceBuffer&) = delete;

		  private:
			///
			/// ID returned by OpenGL when generating buffer.
			///
			unsigned int m_id;

			///
			/// Number of instances.
			///
			int m_amount;
		};
	} // namespace graphics
} // namespace galaxy

#endif

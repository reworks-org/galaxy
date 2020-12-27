///
/// InstanceBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_INSTANCEBUFFER_HPP_
#define GALAXY_GRAPHICS_VERTEX_INSTANCEBUFFER_HPP_

#include <span>

#include <glm/vec3.hpp>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Abstraction for OpenGL instance buffers.
		///
		class InstanceBuffer final
		{
		public:
			///
			/// Default constructor.
			///
			InstanceBuffer();

			///
			/// Copy constructor.
			///
			InstanceBuffer(const InstanceBuffer&) = delete;

			///
			/// Move constructor.
			///
			InstanceBuffer(InstanceBuffer&&);

			///
			/// Copy assignment operator.
			///
			InstanceBuffer& operator=(const InstanceBuffer&) = delete;

			///
			/// Move assignment operator.
			///
			InstanceBuffer& operator=(InstanceBuffer&&);

			///
			/// Create instance buffer object.
			///
			/// \param offsets Offset for each instance. Size should be total number of instances. Z is opacity.
			/// \param divisor set to 1 for once per instance.
			///
			void create(std::span<glm::vec3> offsets, unsigned int divisor);

			///
			/// Update internal instance buffer.
			///
			/// \param offsets Offset for each instance. Size should be total number of instances. Z is opacity.
			///
			void update(std::span<glm::vec3> offsets);

			///
			/// Destroys buffer.
			///
			~InstanceBuffer();

			///
			/// Bind the current instance buffer to current GL context.
			///
			void bind();

			///
			/// Unbind the current instance buffer to current GL context.
			///
			void unbind();

			///
			/// Get OpenGL handle.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int id() const;

			///
			/// Get Attribute Divisor.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int divisor() const;

		private:
			///
			/// ID returned by OpenGL when generating buffer.
			///
			unsigned int m_id;

			///
			/// Divisor.
			///
			unsigned int m_divisor;
		};
	} // namespace graphics
} // namespace galaxy

#endif
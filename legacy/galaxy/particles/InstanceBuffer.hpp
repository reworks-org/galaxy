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
			InstanceBuffer() noexcept;

			///
			/// Move constructor.
			///
			InstanceBuffer(InstanceBuffer&&) noexcept;

			///
			/// Move assignment operator.
			///
			InstanceBuffer& operator=(InstanceBuffer&&) noexcept;

			///
			/// Destroys buffer.
			///
			~InstanceBuffer() noexcept;

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
			/// Bind the current instance buffer to current GL context.
			///
			void bind() noexcept;

			///
			/// Unbind the current instance buffer to current GL context.
			///
			void unbind() noexcept;

			///
			/// Get OpenGL handle.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int id() const noexcept;

			///
			/// Get Attribute Divisor.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int divisor() const noexcept;

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
			/// Divisor.
			///
			unsigned int m_divisor;
		};
	} // namespace graphics
} // namespace galaxy

#endif
///
/// InstanceBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_INSTANCEBUFFER_HPP_
#define GALAXY_GRAPHICS_INSTANCEBUFFER_HPP_

#include <span>

#include <glm/vec2.hpp>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Abstraction for OpenGL instance buffer objects.
		///
		class InstanceBuffer final
		{
		public:
			///
			/// Constructor.
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
			/// \param vertices Vertices to assign.
			///
			void create(std::span<glm::vec2> vertices);

			///
			/// Bind the current instance buffer to current GL context.
			///
			void bind() noexcept;

			///
			/// Unbind the current instance buffer to current GL context.
			///
			void unbind() noexcept;

			///
			/// \brief Destroy Instance Buffer Object.
			///
			/// Also called by destructor, you do not have to call this.
			///
			void destroy() noexcept;

			///
			/// Get OpenGL handle.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int id() const noexcept;

			///
			/// Get current instance object count.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int instance_count() const noexcept;

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
			unsigned int m_ibo;

			///
			/// Number of instances this buffer has been allocated.
			///
			unsigned int m_instance_count;
		};
	} // namespace graphics
} // namespace galaxy

#endif
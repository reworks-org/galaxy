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
			/// Create instance buffer object.
			///
			/// \param offsets Offset position for each vertex.
			///
			void create(std::span<glm::vec2> offsets);

			///
			/// Clear all data from buffer.
			///
			void clear();

			///
			/// \brief Destroy Instance Buffer Object.
			///
			/// Also called by destructor, you do not have to call this.
			///
			void destroy();

			///
			/// Get OpenGL handle.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] unsigned int id() const;

			///
			/// Get current instance object count.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] unsigned int instance_count() const;

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
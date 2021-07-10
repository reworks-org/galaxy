///
/// VertexAttribute.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEXATTRIBUTE_HPP_
#define GALAXY_GRAPHICS_VERTEXATTRIBUTE_HPP_

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Enum of different vertex attribute types.
		///
		enum class VertexAttributes : short
		{
			///
			/// Specifies "position" vertex attribute type at compile time.
			///
			POSITION,

			///
			/// Specifies "texel" vertex attribute type at compile time.
			///
			TEXEL,

			///
			/// Specifies "colour" vertex attribute type at compile time.
			///
			COLOUR
		};

		///
		/// Specifys the attributes of a vertex array for its verticies.
		///
		class VertexAttribute final
		{
		public:
			///
			/// Default constructor.
			///
			VertexAttribute() noexcept;

			///
			/// \brief Creation constructor.
			///
			/// See: http://docs.gl/gl4/glVertexAttribPointer
			///
			/// \param size Number of components for each vertex attribute.
			/// \param type Data type of the verticies. I.e. GL_FLOAT or GL_INT.
			/// \param normalized Is this vertex normalized? I.e. GL_FALSE or GL_TRUE.
			/// \param offset The offset of this attribute in the verticies.
			///
			VertexAttribute(const unsigned int size, const unsigned int type, unsigned char normalized, const unsigned int offset) noexcept;

			///
			/// Move constructor.
			///
			VertexAttribute(VertexAttribute&&) noexcept;

			///
			/// Move assignment operator.
			///
			VertexAttribute& operator=(VertexAttribute&&) noexcept;

			///
			/// Destructor.
			///
			~VertexAttribute() noexcept = default;

		private:
			///
			/// Copy constructor.
			///
			VertexAttribute(const VertexAttribute&) = delete;

			///
			/// Copy assignment operator.
			///
			VertexAttribute& operator=(const VertexAttribute&) = delete;

		public:
			///
			/// Components for each vertex attribute.
			///
			unsigned int m_size;

			///
			/// Data type. I.e. GL_FLOAT or GL_INT.
			///
			unsigned int m_type;

			///
			/// Is this vertex normalized? I.e. GL_FALSE or GL_TRUE.
			///
			unsigned char m_normalized;

			///
			/// The offset of this attribute in the verticies.
			///
			std::size_t m_offset;
		};
	} // namespace graphics
} // namespace galaxy

#endif
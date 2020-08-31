///
/// InstanceVertex.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_INSTANCEVERTEX_HPP_
#define QUASAR_INSTANCEVERTEX_HPP_

#include <glm/vec2.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Represents a single InstanceVertex of a primitive.
	///
	class InstanceVertex final
	{
	public:
		///
		/// Default constructor.
		///
		InstanceVertex() noexcept;

		///
		/// Argument constructor.
		///
		/// \param x X pos.
		/// \param y Y pos.
		/// \param u Texel x pos.
		/// \param v Texel y pos.
		/// \param off_x Instance offset x pos.
		/// \param off_y Instance offset y pos.
		///
		InstanceVertex(const float x, const float y, const float u, const float v, const float off_x, const float off_y) noexcept;

		///
		/// Copy constructor.
		///
		InstanceVertex(const InstanceVertex&);

		///
		/// Move constructor.
		///
		InstanceVertex(InstanceVertex&&);

		///
		/// Copy assignment operator.
		///
		InstanceVertex& operator=(const InstanceVertex&);

		///
		/// Move assignment operator.
		///
		InstanceVertex& operator=(InstanceVertex&&);

		///
		/// Destructor.
		///
		~InstanceVertex() noexcept = default;

		///
		/// Screen position.
		///
		glm::vec2 m_pos;

		///
		/// Texture positions.
		///
		glm::vec2 m_texels;

		///
		/// Instance offset.
		///
		glm::vec2 m_offset;
	};
} // namespace qs

#endif
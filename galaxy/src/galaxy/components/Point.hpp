///
/// Point.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHAPES_POINT_HPP_
#define GALAXY_GRAPHICS_SHAPES_POINT_HPP_

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Transform.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Point graphic definition.
		///
		class Point final : public VertexData, public Transform
		{
		public:
			///
			/// Constructor.
			///
			Point();

			///
			/// Constructor.
			///
			/// \param x X position.
			/// \param y Y position.
			/// \param size Size of the point.
			/// \param colour Colour.
			///
			Point(const float x, const float y, const unsigned int size, graphics::Colour& colour);

			///
			/// Destructor.
			///
			virtual ~Point() = default;

			///
			/// Create the point.
			///
			/// \param x X position.
			/// \param y Y position.
			/// \param size Size of the point.
			/// \param colour Colour.
			///
			void create(const float x, const float y, const unsigned int size, graphics::Colour& colour);

			///
			/// Bind as active VA.
			///
			void bind();

			///
			/// Unbind as active VA.
			///
			void unbind();

			///
			/// Get size of point.
			///
			/// \return Const integer.
			///
			[[nodiscard]] const int get_size() const;

		private:
			///
			/// Size of point.
			///
			int m_size;
		};
	} // namespace graphics
} // namespace galaxy

#endif
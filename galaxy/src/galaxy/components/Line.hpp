///
/// Line.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_LINE_HPP_
#define GALAXY_COMPONENTS_LINE_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/vertex/VertexData.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Line definition for renderer.
		///
		class Line final : public graphics::VertexData
		{
		public:
			///
			/// Constructor.
			///
			Line() noexcept = default;

			///
			/// Constructor.
			///
			/// \param col Colour of line.
			/// \param x1 X1 position.
			/// \param y1 Y1 position.
			/// \param x2 X2 position.
			/// \param y2 Y2 position.
			///
			Line(const graphics::Colour& col, const float x1, const float y1, const float x2, const float y2) noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Line(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Line(Line&&) noexcept;

			///
			/// Move assignment operator.
			///
			Line& operator=(Line&&) noexcept;

			///
			/// Destructor.
			///
			~Line() noexcept = default;

			///
			/// Create the Line.
			///
			/// \param col Colour of line.
			/// \param x1 X1 position.
			/// \param y1 Y1 position.
			/// \param x2 X2 position.
			/// \param y2 Y2 position.
			/// \param thickness Sets the line thickness, defaulting to 1.
			///
			void create(const graphics::Colour& col, const float x1, const float y1, const float x2, const float y2);

			///
			/// Bind as active VA.
			///
			void bind() noexcept;

			///
			/// Unbind as active VA.
			///
			void unbind() noexcept;

		private:
			///
			/// Copy assignment operator.
			///
			Line& operator=(const Line&) = delete;

			///
			/// Copy constructor.
			///
			Line(const Line&) = delete;
		};
	} // namespace components
} // namespace galaxy

#endif
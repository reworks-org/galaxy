///
/// FontContext.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXT_FONTCONTEXT_HPP_
#define GALAXY_GRAPHICS_TEXT_FONTCONTEXT_HPP_

#include <msdfgl.h>

#undef ERROR
#undef DELETE
#undef OPAQUE
#undef TRANSPARENT

namespace galaxy
{
	namespace graphics
	{
		///
		/// MSDF OpenGL configuration.
		///
		class FontContext final
		{
		  public:
			///
			/// Constructor.
			///
			FontContext();

			///
			/// Destructor.
			///
			~FontContext();

			///
			/// Set font DPI.
			///
			/// \param x Font DPI scale for x axis.
			/// \param y Font DPI scale for y axis.
			///
			void set_dpi(const float x, const float y) const;

			///
			/// Get font context object.
			///
			/// \return msdfgl_context_t pointer.
			///
			[[nodiscard]] msdfgl_context_t context();

		  private:
			///
			/// Copy constructor.
			///
			FontContext(const FontContext&) = delete;

			///
			/// Move constructor.
			///
			FontContext(FontContext&&) = delete;

			///
			/// Copy assignment operator.
			///
			FontContext& operator=(const FontContext&) = delete;

			///
			/// Move assignment operator.
			///
			FontContext& operator=(FontContext&&) = delete;

		  private:
			///
			/// Library context object.
			///
			msdfgl_context_t m_context;
		};
	} // namespace graphics
} // namespace galaxy

#endif

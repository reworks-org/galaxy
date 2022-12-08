///
/// FontContext.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "FontContext.hpp"

namespace galaxy
{
	namespace graphics
	{
		FontContext::FontContext()
			: m_context {nullptr}
		{
			m_context = msdfgl_create_context("460 core");
			if (!m_context)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to create font context.");
			}
			else
			{
				msdfgl_set_missing_glyph_callback(m_context, msdfgl_generate_glyph, nullptr);
			}
		}

		FontContext::~FontContext() noexcept
		{
			if (m_context != nullptr)
			{
				msdfgl_destroy_context(m_context);
			}
		}

		msdfgl_context_t FontContext::context() noexcept
		{
			return m_context;
		}

		void FontContext::set_dpi(const float x, const float y) noexcept
		{
			msdfgl_set_dpi(m_context, x, y);
		}
	} // namespace graphics
} // namespace galaxy
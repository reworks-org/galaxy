///
/// FontContext.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"

#include "FontContext.hpp"

namespace galaxy
{
	namespace graphics
	{
		FontContext::FontContext()
			: m_context {nullptr}
		{
			m_context = msdfgl_create_context(nullptr);
			if (!m_context)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to create font context.");
			}
			else
			{
				msdfgl_set_missing_glyph_callback(m_context, msdfgl_generate_glyph, nullptr);

				auto& w = core::ServiceLocator<core::Window>::ref();

				const auto scale = w.get_content_scale();
				set_dpi(scale.x, scale.y);
			}
		}

		FontContext::~FontContext()
		{
			if (m_context != nullptr)
			{
				msdfgl_destroy_context(m_context);
			}
		}

		void FontContext::set_dpi(const float x, const float y) const
		{
			msdfgl_set_dpi(m_context, x, y);
		}

		msdfgl_context_t FontContext::context()
		{
			return m_context;
		}
	} // namespace graphics
} // namespace galaxy

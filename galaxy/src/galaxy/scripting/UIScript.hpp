///
/// UIScript.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_UISCRIPT_HPP_
#define GALAXY_UI_UISCRIPT_HPP_

#include <Nuklear.hpp>
#include <sol/sol.hpp>

namespace galaxy
{
	namespace scripting
	{
		///
		/// Script for running an active UI.
		///
		class UIScript final
		{
		public:
			///
			/// Constructor.
			///
			UIScript();

			///
			/// Argument constructor.
			///
			/// \param file Script file to load.
			/// \param ctx Nuklear UI context.
			///
			UIScript(const std::string& file, nk_context* ctx);

			///
			/// Destructor.
			///
			~UIScript();

			///
			/// Load script and set context.
			///
			/// \param file Script file to load.
			/// \param ctx Nuklear UI context.
			///
			void load(const std::string& file, nk_context* ctx);

		public:
			///
			/// The script object (table) returned by a lua script.
			///
			sol::table m_self;

			///
			/// The update function belonging to the lua table.
			///
			sol::function m_update;

			///
			/// Is this script active on screen?
			///
			bool m_active;
		};
	} // namespace scripting
} // namespace galaxy

#endif
///
/// RenderableFlag.hpp
/// starlight
///
///  Created by reworks on 12/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RENDERABLEFLAG_HPP_
#define STARLIGHT_RENDERABLEFLAG_HPP_

namespace sl
{
	class RenderableFlag final
	{
	public:
		///
		/// Default Constructor.
		///
		RenderableFlag() = default;

		///
		/// Move Constructor.
		///
		RenderableFlag(RenderableFlag&&) = default;

		///
		/// Destructor.
		///
		~RenderableFlag() = default;

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		RenderableFlag() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		RenderableFlag(const RenderableFlag&) = delete;
	};
}

#endif
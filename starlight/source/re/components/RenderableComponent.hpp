///
/// RenderableComponent.hpp
/// starlight
///
///  Created by reworks on 12/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RENDERABLECOMPONENT_HPP_
#define STARLIGHT_RENDERABLECOMPONENT_HPP_

namespace sl
{
	class RenderableComponent final
	{
	public:
		///
		/// Default Constructor.
		///
		RenderableComponent() = default;

		///
		/// Move Constructor.
		///
		RenderableComponent(RenderableComponent&&) = default;

		///
		/// Destructor.
		///
		~RenderableComponent() = default;

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		RenderableComponent() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		RenderableComponent(const RenderableComponent&) = delete;
	};
}

#endif
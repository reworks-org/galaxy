///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LAYER_HPP_
#define GALAXY_CORE_LAYER_HPP_

#include "galaxy/error/Log.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Represents a "layer" in the engine, encapsulating things like events and rendering.
		///
		class Layer
		{
		public:
			///
			/// Default virtual destructor.
			///
			virtual ~Layer() = default;

			///
			/// Process layer events.
			///
			virtual void events() = 0;

			///
			/// Process update (fixed timestep) logic.
			///
			/// \param dt Delta-Time from fixed timestep gameloop.
			///
			virtual void update(const double dt) = 0;

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			virtual void pre_render() = 0;

			///
			/// Render layer.
			///
			virtual void render() = 0;

			///
			/// Set layer name.
			///
			/// \param name Name in std::string format.
			///
			void set_name(std::string_view name);

			///
			/// Get layer name.
			///
			/// \return Const std::string.
			///
			const std::string& get_name();

		protected:
			///
			/// Default constructor.
			///
			Layer();

			///
			/// Argument constructor.
			///
			/// \param name Gives the layer a name.
			///
			explicit Layer(std::string_view name);

		protected:
			///
			/// Layer name. Useful to have.
			///
			std::string m_name;
		};
	} // namespace core
} // namespace galaxy

#endif
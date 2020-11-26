///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LAYER_HPP_
#define GALAXY_LAYER_HPP_

#include <string_view>

#include <pulsar/Log.hpp>

///
/// Core namespace.
///
namespace galaxy
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

		///
		/// Copy constructor.
		///
		Layer(const Layer&) = default;

		///
		/// Move constructor.
		///
		Layer(Layer&&) = default;

		///
		/// Copy assignment operator.
		///
		Layer& operator=(const Layer&) = default;

		///
		/// Move assignment operator.
		///
		Layer& operator=(Layer&&) = default;

	protected:
		///
		/// Layer name. Useful to have.
		///
		std::string m_name;
	};
} // namespace galaxy

#endif
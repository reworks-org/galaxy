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
#include <qs/graphics/Camera.hpp>

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
		virtual ~Layer() noexcept = default;

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
		/// \param camera View camera to render with.
		///
		virtual void render(qs::Camera& camera) = 0;

		///
		/// Set layer name.
		///
		/// \param name Name in std::string format.
		///
		void set_name(std::string_view name) noexcept;

		///
		/// Get layer name.
		///
		/// \return Const std::string.
		///
		const std::string& get_name() noexcept;

	protected:
		///
		/// Default constructor.
		///
		Layer() noexcept;

		///
		/// Argument constructor.
		///
		/// \param name Gives the layer a name.
		///
		explicit Layer(std::string_view name) noexcept;

		///
		/// Copy constructor.
		///
		Layer(const Layer&) noexcept = default;

		///
		/// Move constructor.
		///
		Layer(Layer&&) noexcept = default;

		///
		/// Copy assignment operator.
		///
		Layer& operator=(const Layer&) noexcept = default;

		///
		/// Move assignment operator.
		///
		Layer& operator=(Layer&&) noexcept = default;

	protected:
		///
		/// Layer name. Useful to have.
		///
		std::string m_name;
	};
} // namespace galaxy

#endif
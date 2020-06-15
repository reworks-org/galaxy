///
/// ECSLayer.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_ECSLAYER_HPP_
#define SUPERCLUSTER_ECSLAYER_HPP_

#include <qs/core/Window.hpp>
#include <galaxy/core/World.hpp>
#include <qs/graphics/Camera.hpp>
#include <galaxy/layer/Layer.hpp>

namespace sc
{
	class ECSLayer final : public galaxy::Layer
	{
	public:
		///
		/// Constructor.
		///
		ECSLayer() noexcept;

		///
		/// Virtual destructor.
		///
		virtual ~ECSLayer() noexcept;

		///
		/// Process layer events.
		///
		void events() noexcept override;

		///
		/// Process update (fixed timestep) logic.
		///
		/// \param deltaTime Delta-Time from fixed timestep gameloop.
		///
		void update(protostar::ProtectedDouble* deltaTime) noexcept override;

		///
		/// \brief Render layer.
		///
		/// Does NOT allow std::exceptions!
		///
		void render() noexcept override;

	private:
		galaxy::World* m_world;
		qs::Window* m_window;
		qs::Camera m_camera;
	};
}

#endif
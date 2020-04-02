///
/// Renderer.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERER_HPP_
#define QUASAR_RENDERER_HPP_

#include <qs/core/Shader.hpp>
#include <qs/transforms/Camera.hpp>	
#include <qs/vertex/VertexArray.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	///
	///
	class Renderer
	{
	public:
		///
		///
		///
		Renderer();

		///
		///
		///
		Renderer(qs::Camera* camera);

		///
		///
		///
		~Renderer();

		///
		///
		///
		void setCamera();

		///
		///
		///
		void submit(const qs::VertexArray& va, const qs::Shader& shader);
	};
}

#endif
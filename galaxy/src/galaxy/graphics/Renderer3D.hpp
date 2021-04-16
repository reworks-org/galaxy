///
/// Renderer3D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER3D_HPP_
#define GALAXY_GRAPHICS_RENDERER3D_HPP_

#include "galaxy/graphics/Camera3D.hpp"

#define RENDERER_3D galaxy::graphics::Renderer3D::inst

namespace galaxy
{
	namespace components
	{
	} // namespace components

	namespace light
	{
		class Directional;
		class Point;
		class Spot;
		class Object;
	} // namespace light

	namespace graphics
	{
		class Model;
		class Shader;
		class Skybox;

		///
		/// OpenGL 3D renderer.
		///
		class Renderer3D final
		{
		public:
			///
			/// Destructor.
			///
			~Renderer3D() noexcept;

			///
			/// Retrieve Renderer instance.
			///
			/// \return Reference to renderer.
			///
			[[nodiscard]] static Renderer3D& inst() noexcept;

			void draw_model(Model* model, light::Point* point, light::Directional* dir, light::Spot* spot, Camera3D& camera, Shader* shader);
			void draw_light_object(light::Object* light, Camera3D& camera, Shader* shader);
			void draw_skybox(Skybox* skybox, Camera3D& camera, Shader* shader);

		private:
			///
			/// Constructor.
			///
			Renderer3D() noexcept;
		};
	} // namespace graphics
} // namespace galaxy

#endif
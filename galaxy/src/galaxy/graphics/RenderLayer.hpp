///
/// RenderLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERLAYER_HPP_
#define GALAXY_GRAPHICS_RENDERLAYER_HPP_

#include <vector>

#include "galaxy/graphics/Renderable.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A layer of renderables to draw to the screen.
		///
		class RenderLayer final
		{
			friend class Renderer2D;
			using BatchMap = robin_hood::unordered_flat_map<unsigned int, SpriteBatch>;

		public:
			///
			/// Argument constructor.
			///
			/// \param layer Numeric depth of this layer. Back-To-Front ordering.
			///
			RenderLayer(const int layer);

			///
			/// Move constructor.
			///
			RenderLayer(RenderLayer&&) noexcept;

			///
			/// Move assignment operator.
			///
			RenderLayer& operator=(RenderLayer&&) noexcept;

			///
			/// Destructor.
			///
			~RenderLayer();

			///
			/// Submit a renderable to this layer.
			///
			/// \param renderable Usually constructed by Renderer2D::submit().
			///
			void submit(Renderable& renderable);

			///
			/// Submit all batched sprites in this layer to the renderer.
			///
			/// \param batch_shader Shader used to draw sprite batches.
			///
			void submit_batched_sprites(Shader& batch_shader);

			///
			/// Clear all data.
			///
			void clear();

			///
			/// Draw everything to screen.
			///
			void draw();

			///
			/// Get numeric layer (depth).
			///
			/// \return Const integer. Back-To-Front ordering.
			///
			[[nodiscard]] const int get_layer() const noexcept;

		private:
			///
			/// Constructor.
			///
			RenderLayer() = delete;

			///
			/// Copy constructor.
			///
			RenderLayer(const RenderLayer&) = delete;

			///
			/// Copy assignment operator.
			///
			RenderLayer& operator=(const RenderLayer&) = delete;

		private:
			///
			/// Layer depth.
			///
			int m_layer;

			///
			/// Layer data.
			///
			std::vector<Renderable> m_data;

			///
			/// Layer spritebatches.
			///
			BatchMap m_batches;
		};
	} // namespace graphics
} // namespace galaxy

#endif
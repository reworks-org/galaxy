///
/// Batch.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_BATCH_HPP_
#define GALAXY_COMPONENTS_BATCH_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Renderable.hpp"
#include "galaxy/resource/TextureAtlas.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Batched vertex data for a series of sprites, like a tilemap.
		///
		class Batch final : public graphics::Renderable
		{
		public:
			///
			/// Constructor.
			///
			Batch() noexcept;

			///
			/// Constructor.
			///
			/// \param Batch Batch for entity.
			///
			Batch(std::string_view Batch) noexcept;

			///
			/// Move constructor.
			///
			Batch(Batch&&) noexcept;

			///
			/// Move assignment operator.
			///
			Batch& operator=(Batch&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Batch() noexcept;

			///
			/// Creates the internal vertex array.
			///
			/// \param info Texture atlas data.
			/// \param vertices Position of each quad.
			/// \param layer Rendering layer.
			/// \param opacity Opacity from 0.0f to 1.0f.
			///
			void create(resource::TextureAtlas::Info& info, std::span<graphics::Vertex> vertices, const int layer, const float opacity = 1.0f);

			///
			/// Set opacity.
			///
			/// \param opacity Opacity from 0.0f to 1.0f.
			///
			void set_opacity(const float opacity) noexcept;

			///
			/// Get opacity.
			///
			/// \return float. In range 0.0f - 1.0f.
			///
			[[nodiscard]] float get_opacity() const noexcept;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float get_width() const noexcept;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float get_height() const noexcept;

		private:
			///
			/// Copy assignment operator.
			///
			Batch& operator=(const Batch&) = delete;

			///
			/// Copy constructor.
			///
			Batch(const Batch&) = delete;

			///
			/// Configure renderable.
			///
			void configure() noexcept override;

		private:
			///
			/// Vertex array.
			///
			graphics::VertexArray m_vao;

			///
			/// Sprite opacity.
			///
			float m_opacity;

			///
			/// Cached texture width.
			///
			float m_width;

			///
			/// Cached texture height.
			///
			float m_height;
		};
	} // namespace components
} // namespace galaxy

#endif
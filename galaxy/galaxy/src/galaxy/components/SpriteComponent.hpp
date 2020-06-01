///
/// SpriteComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SpriteComponent_HPP_
#define GALAXY_SpriteComponent_HPP_

#include <nlohmann/json_fwd.hpp>
#include <qs/core/VertexData.hpp>
#include <qs/texture/Texture.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Allows for an entity to be rendered.
	///
	class SpriteComponent : public qs::VertexData, public qs::Texture
	{
	public:
		///
		/// Constructor.
		///
		SpriteComponent() noexcept;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		SpriteComponent(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		virtual ~SpriteComponent() noexcept = default;

		///
		/// \brief Creates the internal vertex array.
		///
		/// BufferType Fixed or dynamic buffer.
		///
		template<typename BufferType>
		void create() noexcept;

		///
		/// Set opacity.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void setOpacity(float opacity) noexcept;

		///
		/// Get opacity.
		///
		/// \return Const float.
		///
		const float getOpacity() const noexcept;

		///
		/// Activate SpriteComponent context.
		///
		void bind() noexcept override;

		///
		/// Deactivate SpriteComponent context.
		///
		void unbind() noexcept override;

	protected:
		///
		/// Opacity of SpriteComponent.
		///
		float m_opacity;
	};

	template<typename BufferType>
	inline void SpriteComponent::create() noexcept
	{
		auto v1 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f, 0.0f, 0.0f, m_opacity);
		auto v2 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f, 0.0f + m_width, 0.0f, m_opacity);
		auto v3 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f + m_height, 0.0f + m_width, 0.0f + m_height, m_opacity);
		auto v4 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f + m_height, 0.0f, 0.0f + m_height, m_opacity);

		m_vertexBuffer.create<qs::SpriteVertex, BufferType>({ v1, v2, v3, v4 });
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::SpriteVertex, qs::VATypePosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeTexel>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeOpacity>(1);

		m_vertexArray.create<qs::SpriteVertex>(m_vertexBuffer, m_indexBuffer, m_layout);

		setOpacity(1.0f);
	}
}

#endif
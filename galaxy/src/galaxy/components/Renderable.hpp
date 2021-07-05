///
/// Renderable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_RENDERABLE_HPP_
#define GALAXY_COMPONENTS_RENDERABLE_HPP_

#include "galaxy/math/AABB.hpp"
#include "galaxy/math/Rect.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Renderables.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// How to render this entity.
		///
		class Renderable final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Renderable() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Renderable(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Renderable(Renderable&&) noexcept;

			///
			/// Move assignment operator.
			///
			Renderable& operator=(Renderable&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Renderable() noexcept = default;

			///
			/// Update the internal AABB box.
			///
			/// \param rect New bounds for AABB.
			///
			/// \return Returns internal aabb after updating, for convenience.
			///
			[[maybe_unused]] const math::AABB& update_aabb(const math::Rect<float>& bounds);

			///
			/// Get AABB.
			///
			/// \return Reference to body AABB.
			///
			[[nodiscard]] math::AABB& get_aabb() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy assignment operator.
			///
			Renderable& operator=(const Renderable&) = delete;

			///
			/// Copy constructor.
			///
			Renderable(const Renderable&) = delete;

		public:
			///
			/// ID of a shader in the shader resource cache.
			///
			graphics::Renderables m_type;

		private:
			///
			/// AABB bounding box.
			///
			math::AABB m_aabb;
		};
	} // namespace components
} // namespace galaxy

#endif
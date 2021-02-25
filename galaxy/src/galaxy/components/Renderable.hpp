///
/// Renderable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_RENDERABLE_HPP_
#define GALAXY_COMPONENTS_RENDERABLE_HPP_

#include <nlohmann/json_fwd.hpp>

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
			/// Argument constructor.
			///
			/// \param type The type of renderable this entity is.
			/// \param z_level Z-Level to draw entity at.
			///
			Renderable(const graphics::Renderables type, const int z_level) noexcept;

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

		public:
			///
			/// ID of a shader in the shader resource cache.
			///
			graphics::Renderables m_type;

			///
			/// Z level of entity.
			///
			int m_z_level;

		private:
			///
			/// Copy assignment operator.
			///
			Renderable& operator=(const Renderable&) = delete;

			///
			/// Copy constructor.
			///
			Renderable(const Renderable&) = delete;
		};
	} // namespace components
} // namespace galaxy

#endif
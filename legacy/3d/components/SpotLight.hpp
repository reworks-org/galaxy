///
/// SpotLight.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SPOTLIGHT_HPP_
#define GALAXY_COMPONENTS_SPOTLIGHT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/light/Spot.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Spot light component.
		///
		class SpotLight final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			SpotLight() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			SpotLight(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			SpotLight(SpotLight&&) noexcept;

			///
			/// Move assignment operator.
			///
			SpotLight& operator=(SpotLight&&) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~SpotLight() noexcept = default;

			///
			/// Set intensity of ambient light.
			///
			/// \param x X axis intensity.
			/// \param y Y axis intensity.
			/// \param z Z axis intensity.
			///
			void set_ambient_intensity(const float x, const float y, const float z) noexcept;

			///
			/// Set intensity of diffuse light.
			///
			/// \param x X axis intensity.
			/// \param y Y axis intensity.
			/// \param z Z axis intensity.
			///
			void set_diffuse_intensity(const float x, const float y, const float z) noexcept;

			///
			/// Set intensity of specular light.
			///
			/// \param x X axis intensity.
			/// \param y Y axis intensity.
			/// \param z Z axis intensity.
			///
			void set_specular_intensity(const float x, const float y, const float z) noexcept;

			///
			/// Set position of the light.
			///
			/// \param x X axis.
			/// \param y Y axis.
			/// \param z Z axis.
			///
			void set_pos(const float x, const float y, const float z) noexcept;

			///
			/// Set direction of the light.
			///
			/// \param x X axis.
			/// \param y Y axis.
			/// \param z Z axis.
			///
			void set_dir(const float x, const float y, const float z) noexcept;

			///
			/// Set inner cutoff angle.
			///
			/// \param angle Inner cutoff angle.
			///
			void set_inner_cutoff(const float angle) noexcept;

			///
			/// Set outer cutoff angle.
			///
			/// \param angle Inner cutoff angle.
			///
			void set_outer_cutoff(const float angle) noexcept;

			///
			/// Set dirty flag.
			///
			void set_dirty() noexcept;

			///
			/// \brief Get spotlight data.
			///
			/// Resets IsDirty() flag.
			///
			/// \return Reference.
			///
			[[nodiscard]] light::Spot& get_data() noexcept;

			///
			/// Has data been modified.
			///
			/// \return Const bool.
			///
			[[nodiscard]] const bool is_dirty() const noexcept;

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
			/// Copy constructor.
			///
			SpotLight(const SpotLight&) = delete;

			///
			/// Copy assignment operator.
			///
			SpotLight& operator=(const SpotLight&) = delete;

		private:
			///
			/// Is modified flag.
			///
			bool m_dirty;

			///
			/// GLSL compatible object.
			///
			light::Spot m_light;
		};
	} // namespace components
} // namespace galaxy

#endif
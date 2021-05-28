///
/// PointLight.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_POINTLIGHT_HPP_
#define GALAXY_COMPONENTS_POINTLIGHT_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/light/Point.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Point light component.
		///
		class PointLight final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			PointLight() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			PointLight(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			PointLight(PointLight&&) noexcept;

			///
			/// Move assignment operator.
			///
			PointLight& operator=(PointLight&&) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~PointLight() noexcept = default;

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
			/// Set dirty flag.
			///
			void set_dirty() noexcept;

			///
			/// \brief Get pointlight data.
			///
			/// Resets IsDirty() flag.
			///
			/// \return Reference.
			///
			[[nodiscard]] light::Point& get_data() noexcept;

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
			PointLight(const PointLight&) = delete;

			///
			/// Copy assignment operator.
			///
			PointLight& operator=(const PointLight&) = delete;

		private:
			///
			/// Is modified flag.
			///
			bool m_dirty;

			///
			/// GLSL compatible object.
			///
			light::Point m_light;
		};
	} // namespace components
} // namespace galaxy

#endif
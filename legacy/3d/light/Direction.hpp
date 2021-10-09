///
/// DirectionLight.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_DIRECTION_HPP_
#define GALAXY_GRAPHICS_LIGHT_DIRECTION_HPP_

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "galaxy/fs/Serializable.hpp"

#undef near
#undef far

namespace galaxy
{
	namespace light
	{
		///
		/// Directional light in a shader.  GLSL compatible.
		///
		struct Directional final
		{
			///
			/// Intensity of ambient light.
			///
			alignas(16) glm::vec3 m_ambient_intensity;

			///
			/// Intensity of diffuse light.
			///
			alignas(16) glm::vec3 m_diffuse_intensity;

			///
			/// Intensity of specular light.
			///
			alignas(16) glm::vec3 m_specular_intensity;

			///
			/// Direction of the light.
			///
			alignas(16) glm::vec3 m_dir;

			///
			/// Light view.
			///
			alignas(16) glm::mat4 m_view;

			///
			/// Light projection.
			///
			alignas(16) glm::mat4 m_proj;

			///
			/// Light "near" plane.
			///
			alignas(4) float m_near;

			///
			/// Light "far" plane.
			///
			alignas(4) float m_far;
		};
	} // namespace light

	namespace graphics
	{
		///
		/// Directional light component.
		///
		class DirectionLight final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			DirectionLight() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			DirectionLight(const nlohmann::json& json);

			///
			/// Virtual destructor.
			///
			virtual ~DirectionLight() noexcept = default;

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
			/// Set direction of the light.
			///
			/// \param x X axis.
			/// \param y Y axis.
			/// \param z Z axis.
			///
			void set_dir(const float x, const float y, const float z) noexcept;

			///
			/// Set near ortho plane.
			///
			/// \param near Near ortho value.
			///
			void set_near(const float near) noexcept;

			///
			/// Set far ortho plane.
			///
			/// \param far Far ortho value.
			///
			void set_far(const float far) noexcept;

			///
			/// Set what light is looking at (target).
			///
			/// \param x X axis.
			/// \param y Y axis.
			/// \param z Z axis.
			///
			void set_target(const float x, const float y, const float z) noexcept;

			///
			/// Set light FoV.
			///
			/// \param fov Light FoV.
			///
			void set_fov(const float fov) noexcept;

			///
			/// Get light target.
			///
			/// \return Const reference to glm::vec3.
			///
			[[nodiscard]] const glm::vec3& get_target() const noexcept;

			///
			/// Get light FoV.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_fov() const noexcept;

			///
			/// \brief Get pointlight data.
			///
			/// Resets IsDirty() flag.
			///
			/// \return Const reference.
			///
			[[nodiscard]] const light::Directional& get_data() noexcept;

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
			DirectionLight(const DirectionLight&) = delete;

			///
			/// Copy assignment operator.
			///
			DirectionLight& operator=(const DirectionLight&) = delete;

		private:
			///
			/// Is modified flag.
			///
			bool m_dirty;

			///
			/// What the light is pointing to ("focus").
			///
			glm::vec3 m_target;

			///
			/// Light FoV.
			///
			float m_fov;

			///
			/// GLSL compatible object.
			///
			light::Directional m_data;
		};
	} // namespace graphics
} // namespace galaxy

#endif
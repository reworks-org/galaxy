///
/// ShaderComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SHADERCOMPONENT_HPP_
#define GALAXY_SHADERCOMPONENT_HPP_

#include <nlohmann/json_fwd.hpp>
#include <qs/core/Shader.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Adds a shader to an entity.
	///
	class ShaderComponent final
	{
	public:
		///
		/// Constructor.
		///
		ShaderComponent();

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		ShaderComponent(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		ShaderComponent(const ShaderComponent&) noexcept = delete;

		///
		/// Move constructor.
		///
		ShaderComponent(ShaderComponent&&);

		///
		/// Copy assignment operator.
		///
		ShaderComponent& operator=(const ShaderComponent&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		ShaderComponent& operator=(ShaderComponent&&);

		///
		/// Destructor.
		///
		~ShaderComponent() noexcept = default;

		///
		/// Shader.
		///
		qs::Shader m_shader;
	};
} // namespace galaxy

#endif
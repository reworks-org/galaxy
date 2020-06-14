///
/// ShaderComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SHADERCOMPONENT_HPP_
#define GALAXY_SHADERCOMPONENT_HPP_

#include <nlohmann/json.hpp>
#include <qs/core/Shader.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Adds a shader to an entity.
	///
	struct ShaderComponent final
	{
		///
		/// Constructor.
		///
		ShaderComponent() noexcept;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		ShaderComponent(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		virtual ~ShaderComponent() noexcept = default;

		///
		/// Shader.
		///
		qs::Shader m_shader;
	};
}

#endif
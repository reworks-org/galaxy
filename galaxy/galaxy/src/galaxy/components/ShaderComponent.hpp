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
		ShaderComponent() noexcept = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON defining object.
		///
		ShaderComponent(const nlohmann::json& json);

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
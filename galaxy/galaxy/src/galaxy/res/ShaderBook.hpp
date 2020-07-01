///
/// ShaderBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SHADERBOOK_HPP_
#define GALAXY_SHADERBOOK_HPP_

#include <qs/core/Shader.hpp>
#include <protostar/system/ResourceCache.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Resource manager for shaders.
	///
	class ShaderBook final : public protostar::ResourceCache<qs::Shader>
	{
	public:
		///
		/// Default constructor.
		///
		ShaderBook() noexcept = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON file to load.
		///
		explicit ShaderBook(const std::string& json) noexcept;

		///
		/// Destructor.
		///
		~ShaderBook() noexcept;

		///
		/// Create ShaderBook from JSON.
		///
		/// \param json JSON file to load.
		///
		void createFromJSON(const std::string& json);

		///
		/// Clean up.
		///
		void clear() noexcept override;
	};
}

#endif
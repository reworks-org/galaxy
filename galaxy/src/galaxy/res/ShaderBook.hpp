///
/// ShaderBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SHADERBOOK_HPP_
#define GALAXY_SHADERBOOK_HPP_

#include <galaxy/res/ResourceCache.hpp>
#include <qs/core/Shader.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Resource manager for shaders.
	///
	class ShaderBook final : public pr::ResourceCache<qs::Shader>
	{
	public:
		///
		/// Default constructor.
		///
		ShaderBook() = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON file to load.
		///
		explicit ShaderBook(std::string_view json);

		///
		/// Copy constructor.
		///
		ShaderBook(const ShaderBook&) = delete;

		///
		/// Move constructor.
		///
		ShaderBook(ShaderBook&&) = delete;

		///
		/// Copy assignment operator.
		///
		ShaderBook& operator=(const ShaderBook&) = delete;

		///
		/// Move assignment operator.
		///
		ShaderBook& operator=(ShaderBook&&) = delete;

		///
		/// Destructor.
		///
		virtual ~ShaderBook();

		///
		/// Create ShaderBook from JSON.
		///
		/// \param json JSON file to load.
		///
		void create_from_json(std::string_view json);

		///
		/// Clean up.
		///
		void clear() override;
	};
} // namespace galaxy

#endif
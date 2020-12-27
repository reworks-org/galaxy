///
/// ShaderBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RES_SHADERBOOK_HPP_
#define GALAXY_RES_SHADERBOOK_HPP_

#include "galaxy/graphics/Shader.hpp"
#include "galaxy/res/ResourceCache.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Resource manager for shaders.
		///
		class ShaderBook final : public ResourceCache<graphics::Shader>
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
	} // namespace res
} // namespace galaxy

#endif
///
///  ShaderLibrary.hpp
///  rework
///
///  Created by reworks on 14/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_SHADERLIBRARY_HPP_
#define REWORK_SHADERLIBRARY_HPP_

#include "re/graphics/Shader.hpp"
#include "re/types/ResourceCache.hpp"
#include "re/types/ServiceLocator.hpp"

namespace re
{
	class ShaderLibrary final : public ServiceLocator<ShaderLibrary>, public ResourceCache<Shader>
	{
	public:
		///
		/// \brief Construct ShaderLibrary and add shaders.
		///
		/// \param script Script file containing shader info to parse.
		///
		ShaderLibrary(const std::string& script);

		///
		/// Destructor. Cleans up shaders.
		///
		~ShaderLibrary() override;

		///
		/// Clean up resources.
		///
		void clean() override;

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		ShaderLibrary() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ShaderLibrary(const ShaderLibrary&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		ShaderLibrary(ShaderLibrary&&) = delete;
	};
}

#endif
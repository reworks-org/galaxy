//
//  ShaderManager.hpp
//  rework
//
//  Created by reworks on 14/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SHADERMANAGER_HPP_
#define REWORK_SHADERMANAGER_HPP_

#include <unordered_map>

#include "re/types/Service.hpp"
#include "re/graphics/Shader.hpp"

namespace re
{
	class ShaderManager : public Service
	{
	public:
		///
		/// \brief Construct ShaderManager and add shaders.
		///
		/// \param script Script file containing shader info to parse.
		///
		ShaderManager(const std::string& script);

		///
		/// Destructor. Cleans up shaders.
		///
		~ShaderManager();

		///
		/// Retrieve a shader.
		///
		/// \param name The name of the shader to retrieve.
		///
		/// \return Returns a pointer to the shader.
		///
		Shader* get(const std::string& name);
		
	private:
		std::unordered_map<std::string, Shader> m_shaderMap;
	};
}

#endif
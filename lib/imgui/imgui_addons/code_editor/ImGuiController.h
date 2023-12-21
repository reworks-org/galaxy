#pragma once

#include <string>

namespace scex::ImGuiController
{
	void Setup(const std::string& root);
	void Load(const std::string& file);
	bool HasControl();
	void OnPathsDropped(const char** paths, int pathCount);
	void Tick(double deltaTime);
}
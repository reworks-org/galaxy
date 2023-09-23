#pragma once

#include <string>

namespace scex::ImGuiController
{
	void Setup(const std::string& root);
	bool HasControl();
	void Load(const std::string& file);
	void OnPathsDropped(const char** paths, int pathCount);
	void Tick(double deltaTime);
} // namespace scex::ImGuiController
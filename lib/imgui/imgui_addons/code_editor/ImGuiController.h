#pragma once

#include <string>

namespace ste::CodeEditor
{
	void Setup(const std::string& root);
	bool HasControl();
	void Tick();
	void Load(const std::string& file);
} // namespace ste::CodeEditor
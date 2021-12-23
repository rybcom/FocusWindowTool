#pragma once
#include <string>

namespace commands
{
	void focus_to_window(std::string_view name);

	void unminimize_focused_window();
}
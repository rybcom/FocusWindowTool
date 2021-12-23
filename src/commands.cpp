
#include <Windows.h>
#include <algorithm>
#include <iostream>;
#include "commands.h"

namespace commands
{
	namespace
	{

		static HWND focused_window;
		static std::string searched_title;

		std::string to_lower(std::string_view text)
		{
			std::string data(text);

			std::transform(data.begin(), data.end(), data.begin(),
				[](unsigned char c) { return std::tolower(c); });

			return data;
		}

		BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam)
		{
			int length = GetWindowTextLength(hWnd);
			char* buffer = new char[length + 1];
			GetWindowTextA(hWnd, buffer, length + 1);
			std::string windowTitle(to_lower(buffer));

			if (IsWindowVisible(hWnd) && length != 0)
			{
				if (windowTitle.find(to_lower(searched_title)) != std::string::npos)
				{
					focused_window = hWnd;
					std::cout << "activating window [" << windowTitle << " ]\n";
				}
			}
			return TRUE;
		}

	}

	void focus_to_window(std::string_view name)
	{
		searched_title = name;
		focused_window = {};

		EnumWindows(enumWindowCallback, NULL);

		if (focused_window)
		{
			::SetForegroundWindow(focused_window);
			::SetActiveWindow(focused_window);
		}
	}

	void unminimize_focused_window()
	{
		if (focused_window)
		{
			DWORD dwStyle = (DWORD)GetWindowLong(focused_window, GWL_STYLE);

			if (dwStyle & WS_MINIMIZE)
			{
				::ShowWindowAsync(focused_window, SW_NORMAL);
				std::cout << "unminimizing window \n";
			}
		}
	}
}


#include <iostream>
#include "cxxopts.hpp"
#include "commands.h"   

struct UserRequest
{
	bool show_always{ false };
	std::string title_part;
};

UserRequest parse(int argc, const char* argv[])
{
	UserRequest request;

	try
	{
		cxxopts::Options options(argv[0], " - example command line options");

		options
			.add_options()
			("help", "Print help")
			("s,show", "Show even though it is minimized", cxxopts::value<bool>(request.show_always))
			("t,title", "Search part of the window title", cxxopts::value<std::string>(request.title_part));

		auto result = options.parse(argc, argv);

		if (result.count("help"))
		{
			std::cout << options.help({ "", "Group" }) << std::endl;
			exit(EXIT_SUCCESS);
		}

		return request;
	}
	catch (const cxxopts::OptionException& e)
	{
		std::cout << "error parsing options: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, const char* argv[])
{
	auto request = parse(argc, argv);

	commands::focus_to_window(request.title_part);
	
	if (request.show_always)
	{
		commands::unminimize_focused_window();
	}

	return EXIT_SUCCESS;
}

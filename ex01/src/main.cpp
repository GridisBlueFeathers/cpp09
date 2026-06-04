#include "RPN.hpp"
#include <iostream>

int	main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Invalid amount of arguments" << std::endl;
		return (1);
	}

	RPN	r;

	try {
		r.run(argv[1]);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	
}

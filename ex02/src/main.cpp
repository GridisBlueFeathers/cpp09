#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv) {
	PmergeMe p;
	if (argc < 2) {
		std::cerr << "Not enough arguments" << std::endl;
		return (1);
	}

	std::string inputString;
	for (int i = 1; i < argc; i++) {
		inputString += argv[i];
		inputString += " ";
	}
	try {
		p.init(inputString);
		p.run();

	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
}

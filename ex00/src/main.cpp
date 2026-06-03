#include "BitcoinExchange.hpp"
#include <iostream>

int main (int argc, char **argv) {
	BitcoinExchange	exchange;

	if (argc != 2) {
		std::cerr << "Please provide the path to input file" << std::endl;
	}

	try {
		exchange.init();
		exchange.execute(argv[1]);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

#include "BitcoinExchange.hpp"
#include <fstream>
#include <string>
#include <iostream>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(BitcoinExchange &other) {
	(void)other;
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange	&BitcoinExchange::operator=(BitcoinExchange &other) {
	if (this != &other)
		(void)other;
	return (*this);
}

const char *BitcoinExchange::DBNotAccessibleException::what() const throw() {
	return ("Database is not accessible");
}

void	BitcoinExchange::init() {
	std::ifstream file("data.csv");

	if (!file.is_open())
		throw DBNotAccessibleException();

	for (std::string line; std::getline(file, line);) {
		std::cout << line << std::endl;
	}


}

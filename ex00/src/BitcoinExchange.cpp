#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

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

const char *BitcoinExchange::DBWrongHeaderException::what() const throw() {
	return ("Database has wrong header");
}

static std::string	deserializeDate(uint32_t date) {
	int	year = date / 10000;
	int month = (date - year * 10000) / 100;
	int day = (date - year * 10000 - month * 100);

	std::string	res;
	std::stringstream stream;
	
	stream << year;
	stream << "-";
	if (month < 10)
		stream << "0";
	stream << month;
	stream << "-";
	if (day < 10)
		stream << "0";
	stream << day;
	stream >> res;

	return (res);
}

static uint32_t	serializeDate(std::string &rawDate) {
	uint32_t			res;
	std::string			year, month, day;
	std::istringstream	split(rawDate);
	
	std::getline(split, year, '-');
	std::getline(split, month, '-');
	std::getline(split, day, '-');
	
	res = atoi(year.c_str()) * 10000
		+ atoi(month.c_str()) * 100
		+ atoi(day.c_str());
	
	return (res);
}

void	BitcoinExchange::init() {
	std::ifstream	file("data.csv");
	std::string		line;

	if (!file.is_open())
		throw DBNotAccessibleException();

	std::getline(file, line);

	if (line != "date,exchange_rate")
		throw DBWrongHeaderException();

	for (std::string line; std::getline(file, line);) {
		uint32_t			date;
		double				rate;
		std::string			rawDate, rawRate;
		std::istringstream	split(line);

		std::getline(split, rawDate, ',');
		date = serializeDate(rawDate);

		
		std::getline(split, rawRate, ',');
		std::istringstream	rateStream(rawRate);
		rateStream >> rate;

		_map[date] = rate;
	}
	file.close();
}

const char *BitcoinExchange::InputNotAccessibleException::what() const throw() {
	return ("Input file is not accessible");
}

const char *BitcoinExchange::InputWrongHeaderException::what() const throw() {
	return ("Input file has wrong header");
}

static bool	staticStringValidation(std::string &rawDate) {
	bool err = true;

	if (rawDate.length() != 11) 
		return (err);

	for (int i = 0; i < 11; i++) {
		if ((i < 4 || i == 5 || i == 6 || i == 8 || i == 9)
			&& !std::isdigit(rawDate[i]))
			return (err);
		if ((i == 4 || i == 7)
			&& rawDate[i] != '-')
			return (err);
		if ((i == 11)
			&& rawDate[i] != ' ')
			return (err);
	}
	return (!err);
}

static bool	isLeapYear(int year) {
	if (year % 4 == 0) {
		if (year % 100 == 0)
			return (year % 400 == 0);
		return (true);
	}
	return (false);
}

static bool	dateValidation(std::string &rawDate) {
	uint32_t	date = serializeDate(rawDate);
	int			year = date / 10000;
	int			month = (date - year * 10000) / 100;
	int			day = date - year * 10000 - month * 100;
	bool		err = true;

	if (month < 0 || month > 12)
		return (err);
	if (day < 0 || day > 31)
		return (err);
	if ((month == 4 || month == 6 || month == 9 || month == 11)
		&& day > 30)
		return (err);
	if ((month == 2) && isLeapYear(year)
		&& day > 28)
		return (err);
	if ((month == 2) && !isLeapYear(year)
		&& day > 27)
		return (err);
	return (!err);
}

static uint32_t	validateDate(std::string &rawDate) {
	bool	err = false;

	err = staticStringValidation(rawDate);	
	if (!err)
		err = dateValidation(rawDate);
	
	if (err) {
		std::cerr << "Error: bad input => " << rawDate << std::endl;
		throw std::exception();
	}
	return (serializeDate(rawDate));
}

static double	validateAmount(std::string &rawAmount) {
	double	amount;
	std::istringstream	split(rawAmount);

	if (!(split >> amount)) {
		std::cerr << "Error: bad input => " << rawAmount << std::endl;
		throw std::exception();
	}
	if (amount < 0) {
		std::cerr << "Error: not a positive number." << std::endl;
		throw std::exception();
	}
	if (amount > 1000) {
		std::cerr << "Error: too large a number." << std::endl;
		throw std::exception();
	}
	return (amount);
}

void	BitcoinExchange::convert(uint32_t date, double amount) {
	std::map<uint32_t, float>::iterator entry = _map.find(date);

	(void)amount;
	if (entry != _map.end()) {
		std::cout << deserializeDate(date) << " => "
			<< amount << " = "
			<< amount * entry->second << std::endl;
		return ;
	}

	entry = _map.lower_bound(date);

	if (entry != _map.begin())
		entry--;
	std::cout << deserializeDate(date) << " => "
		<< amount << " = "
		<< amount * entry->second << std::endl;
}

void	BitcoinExchange::execute(const char *filename) {
	std::ifstream	file(filename);
	std::string		line;

	if (!file.is_open())
		throw	InputNotAccessibleException();

	std::getline(file, line);

	if (line != "date | value")
		throw InputWrongHeaderException();

	for (std::string line; std::getline(file, line);) {
		try {
			uint32_t			date;
			double				amount;
			std::string			rawDate, rawAmount;
			std::istringstream	split(line);

			std::getline(split, rawDate, '|');
			std::getline(split, rawAmount, '|');

			date = validateDate(rawDate);
			amount = validateAmount(rawAmount);
			convert(date, amount);
		} catch (std::exception &e) {};
	}
	file.close();
}

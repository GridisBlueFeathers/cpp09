#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <cstdint>
# include <map>
# include <string>

class BitcoinExchange {
	private:
		std::map<uint32_t, float>	_map;

	public:
		BitcoinExchange();
		BitcoinExchange(BitcoinExchange &other);
		~BitcoinExchange();

		BitcoinExchange	&operator=(BitcoinExchange &other);

		void	init();
		void	execute(std::string filename);
		void	convert(uint32_t date, double amount);

		class DBNotAccessibleException: public std::exception {
			const char * what() const throw();
		};

		class DBWrongHeaderException: public std::exception {
			const char * what() const throw();
		};

		class InputNotAccessibleException: public std::exception {
			const char * what() const throw();
		};

		class InputWrongHeaderException: public std::exception {
			const char * what() const throw();
		};
};

#endif

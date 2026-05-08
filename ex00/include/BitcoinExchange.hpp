#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <string>

class BitcoinExchange {
	private:
		std::map<std::string, float> _map;

	public:
		BitcoinExchange();
		BitcoinExchange(BitcoinExchange &other);
		~BitcoinExchange();

		BitcoinExchange	&operator=(BitcoinExchange &other);

		static void	init();
		static void	perform(std::string filename);

		class DBNotAccessibleException: public std::exception {
			const char * what() const throw();
		};
};

#endif

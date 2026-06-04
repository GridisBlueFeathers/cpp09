#ifndef RPN_HPP
# define RPN_HPP

#include <stack>
#include <list>
#include <string>

typedef enum e_op {
	MUL = '*',
	ADD = '+',
	SUB = '-',
	DIV = '/'
}	t_op;

class RPN {
	private:
		std::stack<double, std::list<double>>	_stack;

		RPN(const RPN &other);

		RPN	&operator=(const RPN &other);

	public:
		RPN();
		~RPN();
		
		void	run(const std::string &args);
		void	performOp(const char op);
};

#endif

#include "RPN.hpp"
#include <sstream>
#include <string>
#include <iostream>

RPN::RPN() {}
RPN::RPN(const RPN &other) {(void)other;}
RPN::~RPN() {}
RPN	&RPN::operator=(const RPN &other) {(void)other; return (*this);}

void	RPN::performOp(const char op) {
	double	first;
	double	second;

	if (_stack.size() < 2)
		throw std::runtime_error("Invalid argument");
	
	first = _stack.top();
	_stack.pop();
	second = _stack.top();
	_stack.pop();

	switch (op) {
		case MUL:
			_stack.push(second * first);
			break ;
		case ADD:
			_stack.push(second + first);
			break ;
		case SUB:
			_stack.push(second - first);
			break ;
		case DIV:
			if (!first)
				throw std::runtime_error("Invalid argument");
			_stack.push(second / first);
			break ;
	}
}

void	RPN::run(const std::string &arg) {
	std::istringstream	split(arg);
	std::string			token;

	while (std::getline(split, token, ' ')) {
		if (token.size() != 1)
			throw std::runtime_error("Invalid argument");
		switch (token[0]) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				_stack.push(token[0] - 48);
				break ;
			case '*':
			case '+':
			case '-':
			case '/':
				performOp(token[0]);
				break ;
			default:
				throw std::runtime_error("Invalid argument");
		}
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Invalid argument");
	std::cout << _stack.top() << std::endl;
}

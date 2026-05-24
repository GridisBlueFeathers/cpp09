#include "PmergeMe.hpp"
#include <cmath>
#include <sstream>
#include <iostream>

PmergeMe::PmergeMe() {
#if DEBUG
	std::cerr << "PmergeMe was constructed (default)" << std::endl;
#endif
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	(void)other;
#if DEBUG
	std::cerr << "PmergeMe was constructed (copy)" << std::endl;
#endif
}

PmergeMe::~PmergeMe() {
#if DEBUG
	std::cerr << "PmergeMe was deconstructed" << std::endl;
#endif
}

PmergeMe	&PmergeMe::operator=(const PmergeMe &other) {
	if (this != &other) {
		(void)other;
	}
	return (*this);
}

void	PmergeMe::init(std::string &input) {
	std::istringstream	split(input);
	int num;

	for (; split >> num;) {
		if (num < 0)
			throw std::runtime_error("Error: sequence contains negative numbers");
		_mainVec.push_back(num);
		_mainDeque.push_back(num);
		_size++;
	}
	if (!split.eof())
		throw std::runtime_error("Error: sequence contains non-numeric value");
}

static void	printBefore(std::vector<int> &vec) {
	std::cout << "Before:";

	for (std::vector<int>::iterator i = vec.begin(); i != vec.end(); i++) {
		std::cout << " " << *i;
	}
	std::cout << std::endl;
}

static void	printAfter(std::vector<int> &vec) {
	std::cout << "After: ";

	for (std::vector<int>::iterator i = vec.begin(); i != vec.end(); i++) {
		std::cout << " " << *i;
	}
	std::cout << std::endl;
}

static std::ostream	&operator<<(std::ostream &out, std::vector<int> &vec) {
	for (std::vector<int>::iterator i = vec.begin(); i != vec.end(); i++) {
		out << " " << *i;
	}
	return (out);
}

static std::ostream	&operator<<(std::ostream &out, std::vector<size_t> &vec) {
	for (std::vector<size_t>::iterator i = vec.begin(); i != vec.end(); i++) {
		out << " " << *i;
	}
	return (out);
}

void	PmergeMe::printStepVec(const std::string &step) {
	std::cerr << step << std::endl
		<< "mainVec:" << _mainVec << std::endl
		<< "idxVec:" << _idxVec << std::endl
		<< "tmpVec:" << _tmpVec << std::endl
		<< "auxVec:" << _auxVec << std::endl
		<< "subcontSizes: " << _subcontSizes << std::endl;
	std::cerr << "\n" << std::endl;
}

void	PmergeMe::splitSubcontainers() {
	for (size_t i = 0; i < _subcontSizes.size(); i += 2) {
		size_t	withoutLeftower = _subcontSizes[i] / 2;
		size_t	withLeftower = withoutLeftower + (_subcontSizes[i] % 2);

		_subcontSizes[i] = withLeftower;
		std::cerr << "hello " << _subcontSizes[i] << std::endl;
		_subcontSizes.insert(_subcontSizes.begin() + i, withoutLeftower);
	}

	_subcontSizes.insert(_subcontSizes.begin(), _tmpVec.size());
}

void	PmergeMe::mergeSubcontainers() {
	for (size_t i = 0; i < _subcontSizes.size(); i++) {
		_subcontSizes[i] = _subcontSizes[i] + _subcontSizes[i + 1];
		_subcontSizes.erase(_subcontSizes.begin() + i + 1);
	}
}

void	PmergeMe::splitVec(int depth) {
	size_t						curSize = _mainVec.size();
	int							idxOffset = 0;;
	(void)depth;

	if (_mainVec.size() != 1 && _mainVec.size() % 2) {
		_tmpVec.push_back(*(_mainVec.end() - 1));
		_mainVec.pop_back();
	}

	for (size_t i = 0; i < curSize / 2; i++) {
		_vecComp++;
		if (*(_mainVec.begin() + i) < *(_mainVec.begin() + i + 1)) {
			_tmpVec.insert(_tmpVec.begin() + i, *(_mainVec.begin() + i));
			_mainVec.erase(_mainVec.begin() + i);
			_idxVec.push_back(i + idxOffset);
			_idxVec.insert(_idxVec.begin() + i, i + 1 + idxOffset);
		} else {
			_tmpVec.insert(_mainVec.begin() + i + 1, *(_mainVec.begin() + i + 1));
			_mainVec.erase(_mainVec.begin() + i + 1);
			_idxVec.push_back(i + 1 + idxOffset);
			_idxVec.insert(_idxVec.begin() + i, i + idxOffset);
		}
		idxOffset++;
	}


	splitSubcontainers();
}

void	PmergeMe::allignVec(int depth) {
	size_t	subContNum = pow(2, depth) - 1;
	std::vector<int>	auxCopy;

	for (size_t i = 0; i < subContNum; i++) {
		auxCopy.insert(
			auxCopy.begin(),
			_auxVec.begin() + i * _idxVec.size(),
			_auxVec.begin() + i * _idxVec.size() + _idxVec.size());
		for (size_t j = 0; j < auxCopy.size(); j++)
			_auxVec[j + i * auxCopy.size()] = auxCopy[_idxVec[j]];
		auxCopy.clear();
		if (auxCopy.size() != _subcontSizes[i])
			i++;
	}
}

void	PmergeMe::pushVec(void) {
	_auxVec.insert(_auxVec.begin(), _tmpVec.begin(), _tmpVec.end());
	_tmpVec.clear();
	_idxVec.clear();
}

size_t	PmergeMe::binarySearchVec(int high, int val) {
	int	low = 0;
	int	mid;

	std::cerr << "hello " << high << std::endl;
	// std::cerr << target << std::endl;
	while (low <= high) {
	 	mid = low + (high - low) / 2;
		std::cerr << "low: " << low << " mid: " << mid << " high: " << high << std::endl;
		if (*(_mainVec.begin() + mid) < val) {
			_vecComp++;
			low = mid + 1;
		} else if (*(_mainVec.begin() + mid) > val) {
			_vecComp++;
			high = mid - 1;
		} else
			high = mid - 1;

	}
	// std::cerr << _mainVec << std::endl;
	return (low);
}

static std::vector<size_t>	getInsertionOrder(size_t size) {
	std::vector<size_t>	res;

	res.push_back(0);
	size_t	k = 1;
	size_t	prevJT = 0;
	size_t	curJT = (pow(2, k+1) + pow(-1, k)) / 3;
	size_t	num = curJT;

	while (res.size() < size) {
		if (num >= size) {
			num--;
			continue ;
		}

		if (num == prevJT) {
			prevJT = curJT;
			k++;
			curJT = (pow(2, k + 1) + pow(-1, k)) / 3;
			num = curJT;
			continue ;
		}

		res.push_back(num);
		num--;
	}

	 std::cerr << "Insertion sequence: ";
	 for (std::vector<size_t>::iterator i = res.begin(); i != res.end(); i++)
	 	std::cerr << " " << *i;
	 std::cerr << std::endl;

	return (res);
}

void	PmergeMe::mergeInsertVec() {
	std::vector<size_t>	order = getInsertionOrder(_tmpVec.size());
	size_t				inserted = 1;
	size_t				idToInsert;
	size_t				boundary;
	int					toInsert;
	
	for (std::vector<size_t>::iterator i = order.begin(); i != order.end(); i++) {

		toInsert = *(_tmpVec.begin() + *i);
		std::cerr << "toInsert: " << toInsert << std::endl;

		if (_mainVec.size() - inserted < *i)
			boundary = _mainVec.size() - 1;
		else
			boundary = inserted + *i - 1;

		if (*i)
			idToInsert = binarySearchVec(boundary, toInsert);
		else
			idToInsert = 0;
		std::cerr << "idToInsert: " << idToInsert << std::endl;

		_mainVec.insert(_mainVec.begin() + idToInsert, toInsert);
		std::cerr << "right after insertion" << _mainVec << std::endl;

		_idxVec.insert(_idxVec.begin() + idToInsert, _idxVec.size());
		inserted++;
	}
	_tmpVec.clear();
}	

void	PmergeMe::mergeVec(int depth) {
	if (depth) {
		while (_tmpVec.size() != _subcontSizes[0]) {
			_tmpVec.push_back(*(_auxVec.begin()));
			_auxVec.erase(_auxVec.begin());
		}
		_subcontSizes.erase(_subcontSizes.begin());
	}
	else
		_tmpVec.swap(_auxVec);
	for (size_t i = 0; i < _mainVec.size(); i++)
		_idxVec.push_back(i);
	printStepVec("2.a");

	mergeInsertVec();
	printStepVec("2.b");
	if (depth) {
		allignVec(depth);
		mergeSubcontainers();
	}
	printStepVec("2.c");
	_idxVec.clear();
}

void	PmergeMe::runDepthVec(int depth) {
	std::cerr << "depth: " << depth << std::endl;
	splitVec(depth);
	printStepVec("1.a");
	allignVec(depth);
	printStepVec("1.b");
	pushVec();
	printStepVec("1.c");

	if (_mainVec.size() != 1)
		runDepthVec(depth + 1);

	mergeVec(depth);
}

void	PmergeMe::sortVec() {
	runDepthVec(0);
}

void	PmergeMe::run(void) {
	(void)_vecComp;
	(void)_dequeComp;
	printBefore(_mainVec);

	sortVec();

	printAfter(_mainVec);
}

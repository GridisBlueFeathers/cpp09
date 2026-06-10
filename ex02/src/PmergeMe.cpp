#include "PmergeMe.hpp"
#include <cmath>
#include <ctime>
#include <sstream>
#include <iostream>

PmergeMe::PmergeMe(): _size(0), _vecComp(0), _dequeComp(0) {
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

static std::ostream	&operator<<(std::ostream &out, std::vector<int> &vec) {
	for (std::vector<int>::iterator i = vec.begin(); i != vec.end(); i++) {
		out << " " << *i;
	}
	return (out);
}

#if DEBUG
static std::ostream	&operator<<(std::ostream &out, std::vector<size_t> &vec) {
	for (std::vector<size_t>::iterator i = vec.begin(); i != vec.end(); i++) {
		out << " " << *i;
	}
	return (out);
}
#endif

#if DEBUG
void	PmergeMe::printStepVec(const std::string &step) {
	std::cerr << step << std::endl
		<< "mainVec:" << _mainVec << std::endl
		<< "idxVec:" << _idxVec << std::endl
		<< "tmpVec:" << _tmpVec << std::endl
		<< "auxVec:" << _auxVec << std::endl
		<< "subcontSizes: " << _subcontSizes << std::endl
		<< "subcontExtra: " << _subcontExtra << std::endl;
	std::cerr << "\n" << std::endl;
}
#endif

void	PmergeMe::splitSubcontainersVec() {
	size_t	newExtra = _tmpVec.size() - _mainVec.size();

	for (size_t i = 0; i < _subcontSizesVec.size(); i += 2) {
		size_t	withoutLeftower = (_subcontSizesVec[i] - _subcontExtraVec[i]) / 2;
		size_t	withLeftower = _subcontSizesVec[i] - withoutLeftower;

		_subcontSizesVec[i] = withLeftower;
		_subcontExtraVec[i] = withLeftower - withoutLeftower;
		_subcontSizesVec.insert(_subcontSizesVec.begin() + i, withoutLeftower);
		_subcontExtraVec.insert(_subcontExtraVec.begin() + i, 0);
	}

	_subcontSizesVec.insert(_subcontSizesVec.begin(), _tmpVec.size());
	_subcontExtraVec.insert(_subcontExtraVec.begin(), newExtra);
}

void	PmergeMe::mergeSubcontainersVec() {
	for (size_t i = 0; i < _subcontSizesVec.size(); i++) {
		_subcontSizesVec[i] = _subcontSizesVec[i] + _subcontSizesVec[i + 1];
		_subcontSizesVec.erase(_subcontSizesVec.begin() + i + 1);
		_subcontExtraVec.erase(_subcontExtraVec.begin() + i);
	}
}

void	PmergeMe::splitVec(void) {
	size_t	curSize = _mainVec.size();
	int		idxOffset = 0;
	int		additionalVal = -1;

	if (_mainVec.size() != 1 && _mainVec.size() % 2)
		additionalVal = *(_mainVec.end() - 1);

	for (size_t i = 0; i < curSize / 2; i++) {
		_vecComp++;
		if (*(_mainVec.begin() + i) < *(_mainVec.begin() + i + 1)) {
			_tmpVec.push_back(*(_mainVec.begin() + i));
			_mainVec.erase(_mainVec.begin() + i);
			_idxVec.push_back(i + idxOffset);
			_idxVec.insert(_idxVec.begin() + i, i + 1 + idxOffset);
		} else {
			_tmpVec.push_back(*(_mainVec.begin() + i + 1));
			_mainVec.erase(_mainVec.begin() + i + 1);
			_idxVec.push_back(i + 1 + idxOffset);
			_idxVec.insert(_idxVec.begin() + i, i + idxOffset);
		}
		idxOffset++;
	}

	if (additionalVal >= 0) {
		_tmpVec.push_back(additionalVal);
		_mainVec.pop_back();
	}
}

void	PmergeMe::allignVec(int depth) {
	size_t				subContNum = pow(2, depth) - 1;
	std::vector<int>	auxCopy;
	size_t				offset = 0;

	for (size_t i = 0; i < subContNum; i++) {
		auxCopy.insert(
			auxCopy.begin(),
			_auxVec.begin() + offset,
			_auxVec.begin() + offset + _subcontSizesVec[i]);

		for (size_t j = 0; j < _idxVec.size(); j++)
			_auxVec[j + offset] = auxCopy[_idxVec[j]];

		auxCopy.clear();
		offset += _subcontSizesVec[i];
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

	while (low <= high) {
	 	mid = low + (high - low) / 2;
		if (*(_mainVec.begin() + mid) < val) {
			_vecComp++;
			low = mid + 1;
		} else
			high = mid - 1;
	}
	return (low);
}

template <class T>
static T	getInsertionOrder(size_t size) {
	T	res;

	res.push_back(0);

	size_t	k = 1;
	size_t	prevJT = 0;
	size_t	curJT = (pow(2, k + 1) + pow(-1, k)) / 3;
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

	return (res);
}

void	PmergeMe::mergeInsertVec() {
	std::vector<size_t>	order = getInsertionOrder<std::vector<size_t> >(_tmpVec.size());
	size_t				inserted = 0;
	size_t				mainInitSize = _mainVec.size();
	
	for (std::vector<size_t>::iterator i = order.begin(); i != order.end(); i++) {
		int		toInsert = *(_tmpVec.begin() + *i);
		size_t	boundary = inserted + *i;
		size_t	idToInsert = 0;

		if (_mainVec.size() - inserted <= *i)
			boundary = _mainVec.size() - 1;

		if (*i)
			idToInsert = binarySearchVec(boundary, toInsert);

		_mainVec.insert(_mainVec.begin() + idToInsert, toInsert);
		_idxVec.insert(_idxVec.begin() + idToInsert, mainInitSize + *i);
		inserted++;
	}
	_tmpVec.clear();
}	

void	PmergeMe::mergeVec(int depth) {
	if (depth) {
		while (_tmpVec.size() != _subcontSizesVec[0]) {
			_tmpVec.push_back(*(_auxVec.begin()));
			_auxVec.erase(_auxVec.begin());
		}
		_subcontSizesVec.erase(_subcontSizesVec.begin());
		_subcontExtraVec.erase(_subcontExtraVec.begin());
	} else
		_tmpVec.swap(_auxVec);

	for (size_t i = 0; i < _mainVec.size(); i++)
		_idxVec.push_back(i);

	#if DEBUG
	printStepVec("2.a");
	#endif

	mergeInsertVec();

	#if DEBUG
	printStepVec("2.b");
	#endif

	if (depth) {
		mergeSubcontainersVec();
		allignVec(depth);
	}

	#if DEBUG
	printStepVec("2.c");
	#endif

	_idxVec.clear();
}

void	PmergeMe::runDepthVec(int depth) {
	splitVec();

	#if DEBUG
	printStepVec("1.a");
	#endif

	allignVec(depth);

	#if DEBUG
	printStepVec("1.b");
	#endif

	splitSubcontainersVec();
	pushVec();

	#if DEBUG
	printStepVec("1.c");
	#endif

	if (_mainVec.size() != 1)
		runDepthVec(depth + 1);

	mergeVec(depth);
}

void	PmergeMe::sortVec() {
	if (_size != 1)
		runDepthVec(0);
}

size_t	PmergeMe::binarySearchDeque(int high, int val) {
	int	low = 0;
	int	mid;

	while (low <= high) {
	 	mid = low + (high - low) / 2;
		if (*(_mainDeque.begin() + mid) < val) {
			_dequeComp++;
			low = mid + 1;
		} else
			high = mid - 1;
	}
	return (low);
}

void	PmergeMe::mergeInsertDeque() {
	std::deque<size_t>	order =
		getInsertionOrder<std::deque<size_t> >(_tmpDeque.size());
	size_t				inserted = 0;
	size_t				mainInitSize = _mainDeque.size();
	
	for (std::deque<size_t>::iterator i = order.begin(); i != order.end(); i++) {
		int		toInsert = *(_tmpDeque.begin() + *i);
		size_t	boundary = inserted + *i;
		size_t	idToInsert = 0;

		if (_mainDeque.size() - inserted <= *i)
			boundary = _mainDeque.size() - 1;

		if (*i)
			idToInsert = binarySearchDeque(boundary, toInsert);

		_mainDeque.insert(_mainDeque.begin() + idToInsert, toInsert);
		_idxDeque.insert(_idxDeque.begin() + idToInsert, mainInitSize + *i);
		inserted++;
	}
	_tmpDeque.clear();
}	

void	PmergeMe::sortDeque() {
	if (_size != 1)
		runDepthDeque(0);
}

void	PmergeMe::runDepthDeque(int depth) {
	splitDeque();

	allignDeque(depth);

	splitSubcontainersDeque();
	pushDeque();

	if (_mainDeque.size() != 1)
		runDepthDeque(depth + 1);

	mergeDeque(depth);
}

void	PmergeMe::splitSubcontainersDeque() {
	size_t	newExtra = _tmpDeque.size() - _mainDeque.size();

	for (size_t i = 0; i < _subcontSizesDeque.size(); i += 2) {
		size_t	withoutLeftower = (_subcontSizesDeque[i] - _subcontExtraDeque[i]) / 2;
		size_t	withLeftower = _subcontSizesDeque[i] - withoutLeftower;

		_subcontSizesDeque[i] = withLeftower;
		_subcontExtraDeque[i] = withLeftower - withoutLeftower;
		_subcontSizesDeque.insert(_subcontSizesDeque.begin() + i, withoutLeftower);
		_subcontExtraDeque.insert(_subcontExtraDeque.begin() + i, 0);
	}

	_subcontSizesDeque.insert(_subcontSizesDeque.begin(), _tmpDeque.size());
	_subcontExtraDeque.insert(_subcontExtraDeque.begin(), newExtra);
}

void	PmergeMe::splitDeque() {
	size_t	curSize = _mainDeque.size();
	int		idxOffset = 0;
	int		additionalVal = -1;

	if (_mainDeque.size() != 1 && _mainDeque.size() % 2)
		additionalVal = *(_mainDeque.end() - 1);

	for (size_t i = 0; i < curSize / 2; i++) {
		_dequeComp++;
		if (*(_mainDeque.begin() + i) < *(_mainDeque.begin() + i + 1)) {
			_tmpDeque.push_back(*(_mainDeque.begin() + i));
			_mainDeque.erase(_mainDeque.begin() + i);
			_idxDeque.push_back(i + idxOffset);
			_idxDeque.insert(_idxDeque.begin() + i, i + 1 + idxOffset);
		} else {
			_tmpDeque.push_back(*(_mainDeque.begin() + i + 1));
			_mainDeque.erase(_mainDeque.begin() + i + 1);
			_idxDeque.push_back(i + 1 + idxOffset);
			_idxDeque.insert(_idxDeque.begin() + i, i + idxOffset);
		}
		idxOffset++;
	}

	if (additionalVal >= 0) {
		_tmpDeque.push_back(additionalVal);
		_mainDeque.pop_back();
	}
}

void	PmergeMe::allignDeque(int depth) {
	size_t				subContNum = pow(2, depth) - 1;
	std::deque<int>		auxCopy;
	size_t				offset = 0;

	for (size_t i = 0; i < subContNum; i++) {
		auxCopy.insert(
			auxCopy.begin(),
			_auxDeque.begin() + offset,
			_auxDeque.begin() + offset + _subcontSizesDeque[i]);

		for (size_t j = 0; j < _idxDeque.size(); j++)
			_auxDeque[j + offset] = auxCopy[_idxDeque[j]];

		auxCopy.clear();
		offset += _subcontSizesDeque[i];
	}
}

void	PmergeMe::pushDeque(void) {
	_auxDeque.insert(_auxDeque.begin(), _tmpDeque.begin(), _tmpDeque.end());
	_tmpDeque.clear();
	_idxDeque.clear();
}

void	PmergeMe::mergeDeque(int depth) {
	if (depth) {
		while (_tmpDeque.size() != _subcontSizesDeque[0]) {
			_tmpDeque.push_back(*(_auxDeque.begin()));
			_auxDeque.erase(_auxDeque.begin());
		}
		_subcontSizesDeque.erase(_subcontSizesDeque.begin());
		_subcontExtraDeque.erase(_subcontExtraDeque.begin());
	} else
		_tmpDeque.swap(_auxDeque);

	for (size_t i = 0; i < _mainDeque.size(); i++)
		_idxDeque.push_back(i);

	mergeInsertDeque();

	if (depth) {
		mergeSubcontainersDeque();
		allignDeque(depth);
	}

	_idxDeque.clear();
}

void	PmergeMe::mergeSubcontainersDeque() {
	for (size_t i = 0; i < _subcontSizesDeque.size(); i++) {
		_subcontSizesDeque[i] = _subcontSizesDeque[i] + _subcontSizesDeque[i + 1];
		_subcontSizesDeque.erase(_subcontSizesDeque.begin() + i + 1);
		_subcontExtraDeque.erase(_subcontExtraDeque.begin() + i);
	}
}

void	PmergeMe::run(void) {
	std::cout << "Before:" << _mainVec << std::endl;

	clock_t	startVec = clock();
	sortVec();
	clock_t endVec = clock();
	

	clock_t	startDeq = clock();
	sortDeque();
	clock_t endDeq = clock();
	
	std::cout << "After: " << _mainVec << std::endl;

	std::cout << "Time to process " << _size
		<< " of elements with std::vector : " << (double)(endVec - startVec)
		<< " us" << std::endl;
	std::cout << "Time to process " << _size
		<< " of elements with std::deque : " << (double)(endDeq - startDeq)
		<< " us" << std::endl;

	#if TEST
	std::cout << "comparisons " << _vecComp << std::endl;
	#endif
}

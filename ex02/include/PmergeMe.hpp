#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <cstddef>
# include <deque>
# include <vector>
# include <string>

class PmergeMe {
	private:
		size_t				_size;

		std::vector<int>	_mainVec;
		std::vector<int>	_idxVec;
		std::vector<int>	_tmpVec;
		std::vector<int>	_auxVec;
		std::vector<size_t>	_subcontSizesVec;
		std::vector<size_t>	_subcontExtraVec;
		size_t				_vecComp;


		std::deque<int>		_mainDeque;
		std::deque<int>		_idxDeque;
		std::deque<int>		_tmpDeque;
		std::deque<int>		_auxDeque;
		std::deque<size_t>	_subcontSizesDeque;
		std::deque<size_t>	_subcontExtraDeque;
		size_t				_dequeComp;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe	&other);
		~PmergeMe();

		PmergeMe	&operator=(const PmergeMe &other);

		void		init(std::string &intput);
		void		run(void);
	
		void		sortVec(void);
		void		runDepthVec(int depth);
		void		splitVec(void);
		void		allignVec(int depth);
		void		pushVec(void);
		size_t		binarySearchVec(int high, int val);
		void		mergeInsertVec(void);
		void		mergeVec(int depth);

		void		splitSubcontainersVec(void);
		void		mergeSubcontainersVec(void);
	
		#if DEBUG
		void		printStepVec(const std::string &step);
		#endif

		void		sortDeque(void);
		void		runDepthDeque(int depth);
		void		splitDeque(void);
		void		allignDeque(int depth);
		void		pushDeque(void);
		size_t		binarySearchDeque(int high, int val);

		void		mergeInsertDeque(void);
		void		mergeDeque(int depth);

		void		splitSubcontainersDeque(void);
		void		mergeSubcontainersDeque(void);
};

#endif

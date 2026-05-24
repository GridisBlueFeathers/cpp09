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
		size_t				_vecComp;

		std::vector<size_t>	_subcontSizes;

		std::deque<int>		_mainDeque;
		std::deque<int>		_auxDeque;
		size_t				_dequeComp;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe	&other);
		~PmergeMe();

		PmergeMe	&operator=(const PmergeMe &other);

		void		init(std::string &intput);
		void		run(void);
	
		void		sortVec();
		void		runDepthVec(int depth);
		void		splitVec(int depth);
		void		allignVec(int depth);
		void		pushVec(void);
		void		mergeVec(int depth);
		void		mergeInsertVec(void);
		size_t		binarySearchVec(int high, int val);

		void		splitSubcontainers(void);
		void		mergeSubcontainers(void);
	
		void		printStepVec(const std::string &step);
		

};

#endif

#ifndef _PMERGEME_HPP_
#define _PMERGEME_HPP_

#include <string>

class PmergeMe
{
public:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	static void run(int argc, char **argv);
	static bool parsePositiveInt(const std::string &token, int &value);
	static void printErrorAndExit();

private:
};

#endif


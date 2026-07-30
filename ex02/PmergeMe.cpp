/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvizcaya <fvizcaya@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 23:09:35 by fvizcaya          #+#    #+#             */
/*   Updated: 2026/07/12 23:23:29 by fvizcaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

#include <algorithm>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <deque>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

namespace
{
	typedef std::pair<int, int> IntPair;

	void mergePairs(std::vector<IntPair> &pairs, const std::vector<IntPair> &left, const std::vector<IntPair> &right)
	{
		pairs.clear();
		std::vector<IntPair>::const_iterator leftIt = left.begin();
		std::vector<IntPair>::const_iterator rightIt = right.begin();
		while (leftIt != left.end() || rightIt != right.end()) {
			if (rightIt == right.end() || (leftIt != left.end() && leftIt->first <= rightIt->first)) {
				pairs.push_back(*leftIt);
				++leftIt;
			}
			else {
				pairs.push_back(*rightIt);
				++rightIt;
			}
		}
	}

	void mergeSortPairs(std::vector<IntPair> &pairs)
	{
		if (pairs.size() <= 1) {
			return;
		}

		std::vector<IntPair>::size_type middle = pairs.size() / 2;
		std::vector<IntPair> left(pairs.begin(), pairs.begin() + middle);
		std::vector<IntPair> right(pairs.begin() + middle, pairs.end());
		mergeSortPairs(left);
		mergeSortPairs(right);
		mergePairs(pairs, left, right);
	}

	std::vector<std::size_t> buildInsertionOrder(std::size_t count)
	{
		std::vector<std::size_t> order;
		if (count == 0) {
			return order;
		}

		order.push_back(0);
		std::size_t previous = 1;
		std::size_t current = 3;
		while (previous < count) {
			std::size_t end = current < count ? current : count;
			for (std::size_t index = end; index > previous; --index) {
				order.push_back(index - 1);
			}
			std::size_t next = current + 2 * previous;
			previous = current;
			current = next;
		}
		return order;
	}

	template <typename Container>
	void insertSorted(Container &sequence, int value)
	{
		typename Container::iterator position = std::lower_bound(sequence.begin(), sequence.end(), value);
		sequence.insert(position, value);
	}

	template <typename Container>
	Container sortContainer(const Container &input)
	{
		if (input.size() <= 1) {
			return input;
		}

		typedef typename Container::value_type value_type;
		std::vector<IntPair> pairs;
		pairs.reserve(input.size() / 2 + 1);
		bool hasOdd = false;
		value_type oddValue = 0;

		typename Container::const_iterator it = input.begin();
		while (it != input.end()) {
			value_type first = *it;
			++it;
			if (it == input.end()) {
				hasOdd = true;
				oddValue = first;
				break;
			}

			value_type second = *it;
			++it;
			if (first < second) {
				std::swap(first, second);
			}
			pairs.push_back(IntPair(first, second));
		}

		mergeSortPairs(pairs);

		Container sequence;
		for (std::size_t index = 0; index < pairs.size(); ++index) {
			sequence.push_back(pairs[index].first);
		}

		std::vector<value_type> pending;
		pending.reserve(pairs.size() + (hasOdd ? 1 : 0));
		for (std::size_t index = 0; index < pairs.size(); ++index) {
			pending.push_back(pairs[index].second);
		}
		if (hasOdd) {
			pending.push_back(oddValue);
		}

		std::vector<std::size_t> order = buildInsertionOrder(pending.size());
		for (std::size_t index = 0; index < order.size(); ++index) {
			insertSorted(sequence, pending[order[index]]);
		}

		return sequence;
	}

	template <typename Container>
	void printSequence(const std::string &label, const Container &sequence)
	{
		std::cout << label;
		for (typename Container::const_iterator it = sequence.begin(); it != sequence.end(); ++it) {
			std::cout << ' ' << *it;
		}
		std::cout << std::endl;
	}

	template <typename Container>
	double measureSort(const Container &input, Container &output)
	{
		clock_t start = clock();
		output = sortContainer(input);
		clock_t end = clock();
		return static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
	}

	std::vector<int> parseInput(int argc, char **argv)
	{
		std::vector<int> numbers;
		for (int i = 1; i < argc; ++i) {
			std::istringstream stream(argv[i]);
			std::string token;
			while (stream >> token) {
				int value = 0;
				if (!PmergeMe::parsePositiveInt(token, value)) {
					PmergeMe::printErrorAndExit();
				}
				numbers.push_back(value);
			}
		}
		if (numbers.empty()) {
			PmergeMe::printErrorAndExit();
		}
		return numbers;
	}
}

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	(void)other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	(void)other;
	return *this;
}

PmergeMe::~PmergeMe() {}

bool PmergeMe::parsePositiveInt(const std::string &token, int &value)
{
	if (token.empty()) {
		return false;
	}
	for (std::string::size_type index = 0; index < token.size(); ++index) {
		if (!std::isdigit(static_cast<unsigned char>(token[index]))) {
			return false;
		}
	}

	char *end = 0;
	long parsed = std::strtol(token.c_str(), &end, 10);
	if (end == token.c_str() || *end != '\0' || parsed <= 0 || parsed > INT_MAX) {
		return false;
	}
	value = static_cast<int>(parsed);
	return true;
}

void PmergeMe::printErrorAndExit()
{
	std::cerr << "Error" << std::endl;
	std::exit(1);
}

void PmergeMe::run(int argc, char **argv)
{
	std::vector<int> input = parseInput(argc, argv);
	std::deque<int> dequeInput(input.begin(), input.end());

	printSequence("Before:", input);

	std::vector<int> sortedVector;
	double vectorTime = measureSort(input, sortedVector);
	std::deque<int> sortedDeque;
	double dequeTime = measureSort(dequeInput, sortedDeque);

	printSequence("After:", sortedVector);
	std::cout << "Time to process a range of " << input.size()
		<< " elements with std::vector : " << std::fixed << std::setprecision(5)
		<< vectorTime << " us" << std::endl;
	std::cout << "Time to process a range of " << input.size()
		<< " elements with std::deque : " << std::fixed << std::setprecision(5)
		<< dequeTime << " us" << std::endl;
}
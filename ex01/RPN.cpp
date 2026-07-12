/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvizcaya <fvizcaya@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:48:36 by fvizcaya          #+#    #+#             */
/*   Updated: 2026/07/12 23:06:12 by fvizcaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

#include <cctype>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stack>

RPN::RPN() 
{
}

RPN::RPN(const RPN &other)
{
	(void)other;
}

RPN &RPN::operator=(const RPN &other)
{
	(void)other;
	return *this;
}

RPN::~RPN() 
{
}

bool RPN::isOperator(char token)
{
	return (token == '+' || token == '-' || token == '*' || token == '/');
}

bool RPN::isSingleDigitNumber(const std::string &token)
{
	return (token.size() == 1 && std::isdigit(static_cast<unsigned char>(token[0])));
}

int RPN::applyOperation(int leftOperand, int rightOperand, char operation, bool &valid)
{
	long result = 0;

	if (operation == '+') {
		result = static_cast<long>(leftOperand) + static_cast<long>(rightOperand);
	}
	else if (operation == '-') {
		result = static_cast<long>(leftOperand) - static_cast<long>(rightOperand);
	}
	else if (operation == '*') {
		result = static_cast<long>(leftOperand) * static_cast<long>(rightOperand);
	}
	else if (operation == '/') {
		if (rightOperand == 0) {
			valid = false;
			return 0;
		}
		result = static_cast<long>(leftOperand) / static_cast<long>(rightOperand);
	}
	else {
		valid = false;
		return 0;
	}

	if (result < INT_MIN || result > INT_MAX) {
		valid = false;
		return 0;
	}
	return static_cast<int>(result);
}

int RPN::evaluate(const std::string &expression)
{
	std::stack<int> values;
	std::istringstream stream(expression);
	std::string token;

	while (stream >> token)
	{
		if (isSingleDigitNumber(token))
		{
			values.push(token[0] - '0');
			continue;
		}

		if (token.size() == 1 && isOperator(token[0]))
		{
			if (values.size() < 2)
				throw std::runtime_error("Error");

			int rightOperand = values.top();
			values.pop();
			int leftOperand = values.top();
			values.pop();

			bool valid = true;
			int result = applyOperation(leftOperand, rightOperand, token[0], valid);
			if (!valid) {
				throw std::runtime_error("Error");
			}

			values.push(result);
			continue;
		}

		throw std::runtime_error("Error");
	}

	if (values.size() != 1) {
		throw std::runtime_error("Error");
	}

	return values.top();
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvizcaya <fvizcaya@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:49:04 by fvizcaya          #+#    #+#             */
/*   Updated: 2026/07/12 23:09:57 by fvizcaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef _RPN_HPP_
#define _RPN_HPP_

#include <string>

class RPN
{
	private:
		static bool isOperator(char token);
		static bool isSingleDigitNumber(const std::string &token);
		static int applyOperation(int leftOperand, int rightOperand, char operation, bool &valid);
		
	public:
		RPN();
		RPN(const RPN &other);
		RPN &operator=(const RPN &other);
		~RPN();

		static int evaluate(const std::string &expression);

};

#endif
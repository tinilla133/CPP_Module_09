/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvizcaya <fvizcaya@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:47:55 by fvizcaya          #+#    #+#             */
/*   Updated: 2026/07/12 22:50:14 by fvizcaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

#include <exception>
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}

	try
	{
		std::cout << RPN::evaluate(argv[1]) << std::endl;
	}
	catch (const std::exception &)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}

	return 0;
}
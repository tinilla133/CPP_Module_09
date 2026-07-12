/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvizcaya <fvizcaya@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:36:09 by fvizcaya          #+#    #+#             */
/*   Updated: 2026/07/12 22:36:12 by fvizcaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

#include <exception>
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	try
	{
		BitcoinExchange exchange;
		exchange.run(argv[1]);
	}
	catch (const std::exception &exception)
	{
		std::cerr << exception.what() << std::endl;
		return 1;
	}
	
	return 0;
}
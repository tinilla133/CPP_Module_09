/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvizcaya <fvizcaya@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:26:06 by fvizcaya          #+#    #+#             */
/*   Updated: 2026/07/12 22:48:56 by fvizcaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _BITCOINEXCHANGE_HPP_
#define _BITCOINEXCHANGE_HPP_

#include <map>
#include <string>

class BitcoinExchange
{
	private:
		std::map<std::string, double> exchangeRates;

		void loadDatabase(const std::string &databasePath);
		void processInputFile(const std::string &inputFilePath) const;
		double findRateForDate(const std::string &date) const;
		static std::string trim(const std::string &value);
		static bool isValidDate(const std::string &date);
		static bool isLeapYear(int year);
		static bool parsePositiveValue(const std::string &valueText, double &value);
		static bool splitLine(const std::string &line, char delimiter, std::string &left, std::string &right);
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void run(const std::string &inputFilePath);
};

#endif
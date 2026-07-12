/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvizcaya <fvizcaya@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 22:35:59 by fvizcaya          #+#    #+#             */
/*   Updated: 2026/07/12 22:44:14 by fvizcaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
	: exchangeRates(other.exchangeRates)
{}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		exchangeRates = other.exchangeRates;
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::run(const std::string &inputFilePath)
{
	loadDatabase("data.csv");
	processInputFile(inputFilePath);
}

void BitcoinExchange::loadDatabase(const std::string &databasePath)
{
	std::ifstream file(databasePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file.");

	std::string line;
	if (!std::getline(file, line))
		throw std::runtime_error("Error: empty database.");

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		std::string date;
		std::string rateText;
		if (!splitLine(line, ',', date, rateText))
			continue;

		date = trim(date);
		rateText = trim(rateText);
		if (!isValidDate(date))
			continue;

		double rate = 0.0;
		if (!parsePositiveValue(rateText, rate))
			continue;

		exchangeRates[date] = rate;
	}

	if (exchangeRates.empty())
		throw std::runtime_error("Error: empty database.");
}

void BitcoinExchange::processInputFile(const std::string &inputFilePath) const
{
	std::ifstream file(inputFilePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open file.");

	std::string line;
	if (!std::getline(file, line))
		return;

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		std::string date;
		std::string valueText;
		if (!splitLine(line, '|', date, valueText))
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		date = trim(date);
		valueText = trim(valueText);

		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}

		double value = 0.0;
		if (!parsePositiveValue(valueText, value))
		{
			if (!valueText.empty() && valueText[0] == '-')
				std::cerr << "Error: not a positive number." << std::endl;
			else
				std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		if (value <= 0.0)
		{
			std::cerr << "Error: not a positive number." << std::endl;
			continue;
		}

		if (value > 1000.0)
		{
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}

		double rate = findRateForDate(date);
		std::cout << date << " => " << valueText << " = " << std::setprecision(10) << (value * rate) << std::endl;
	}
}

double BitcoinExchange::findRateForDate(const std::string &date) const
{
	std::map<std::string, double>::const_iterator it = exchangeRates.lower_bound(date);
	if (it == exchangeRates.end())
		--it;
	else if (it->first != date && it != exchangeRates.begin())
		--it;
	return it->second;
}

std::string BitcoinExchange::trim(const std::string &value)
{
	std::string::size_type start = 0;
	while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])))
		++start;
	std::string::size_type end = value.size();
	while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])))
		--end;
	return value.substr(start, end - start);
}

bool BitcoinExchange::isLeapYear(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

bool BitcoinExchange::isValidDate(const std::string &date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return false;

	for (std::string::size_type i = 0; i < date.size(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return false;
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());
	if (month < 1 || month > 12 || day < 1)
		return false;

	static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int maxDay = daysInMonth[month - 1];
	if (month == 2 && isLeapYear(year))
		maxDay = 29;
	return day <= maxDay;
}

bool BitcoinExchange::parsePositiveValue(const std::string &valueText, double &value)
{
	if (valueText.empty())
		return false;

	char *endPtr = 0;
	const double parsed = std::strtod(valueText.c_str(), &endPtr);
	if (endPtr == valueText.c_str() || *endPtr != '\0')
		return false;
	if (parsed < 0.0)
	{
		value = parsed;
		return false;
	}
	value = parsed;
	return true;
}

bool BitcoinExchange::splitLine(const std::string &line, char delimiter, std::string &left, std::string &right)
{
	std::string::size_type pos = line.find(delimiter);
	if (pos == std::string::npos)
		return false;
	left = line.substr(0, pos);
	right = line.substr(pos + 1);
	return true;
}


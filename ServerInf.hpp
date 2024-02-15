/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInf.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcharia < hcharia@student.1337.ma>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 19:44:04 by asidqi            #+#    #+#             */
/*   Updated: 2024/02/14 11:40:05 by hcharia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"
/**
 * @param ports: ports that the server will be listening to
 * @param server_name: this element is not mandatory and specifies domain name
 * @brief contains all configuration file's directives
 */
class ServerInf
{
private:
	std::vector<int> ports;
	std::string server_name;
	std::vector<std::string> error_pages;
	int body_size;
	std::string root;
	std::string host;
	bool		ai;

public:
	static std::string s;
	std::vector<Location> locs;
	std::string filcbs(std::stringstream &ss);
	std::string ferrp(std::stringstream &ss);
	std::string filroot(std::stringstream &ss);
	std::string fillports(std::stringstream &ss);

	static bool isbrac(std::string line, char c);
	bool chekFilld(std::stringstream &ss, std::string line);
	void getMethods(std::stringstream &ss , Location &tmp);
	std::string filloc(std::ifstream &inFile , Location &tmp);
	std::string filltmp(std::ifstream &inFile);
	std::vector<int> getPorts();
	std::string getServName();
	std::string getRoot();
	int			getBodySize();
	void reset();
	void	print() const;
	bool getAI()
	{
		return ai;
	}
};

std::map<std::string, std::string> populateMimeMap();
std::vector<ServerInf> confInf(char *av[]);

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInf.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 19:43:49 by asidqi            #+#    #+#             */
/*   Updated: 2024/01/24 09:47:23 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerInf.hpp"

std::string ServerInf::s = "{";

std::vector<int> ServerInf::getPorts()
{
	return (ports);
}

std::string ServerInf::getServName()
{
	return (server_name);
}

std::string ServerInf::filcbs(std::stringstream &ss)
{
	std::string el;
	ss >> el;
	while (el != ";" && !el.empty())
	{
		body_size = atoi(el.c_str());
		ss >> el;
	}
	return (el);
}
std::string ServerInf::ferrp(std::stringstream &ss)
{
	std::string el;
	ss >> el;
	while (el != ";" && !el.empty())
	{
		error_pages.push_back(el);
		ss >> el;
	}
	return (el);
}
std::string ServerInf::filroot(std::stringstream &ss)
{
	std::string el;
	ss >> el;
	while (el != ";" && !el.empty())
	{
		root = el;
		ss >> el;
	}
	return (el);
}

static void isNumWell(std::string el)
{
	char *s;
	double n = strtod(el.c_str(), &s);
	if (n < 0 || n > 65535 || *s != '\0')
		throw "Ports Are no buenos";
}

std::string ServerInf::fillports(std::stringstream &ss)
{
	std::string el;
	ss >> el;
	while (el != ";" && !el.empty())
	{
		// check number if its goochie
		isNumWell(el);
		ports.push_back(strtod(el.c_str(), NULL));
		ss >> el;
	}
	return (el);
}

bool ServerInf::isbrac(std::string line, char c)
{
	for (int i = 0; line[i]; i++)
		if (!std::isspace(line[i]) && line[i] != c)
			throw "Check brackets";
	return (true);
}
bool ServerInf::chekFilld(std::stringstream &ss, std::string line)
{
	std::string el;
	ss >> el;
	while (!el.empty())
	{
		if (el.compare(line))
			throw "Unknown token";
		ss >> el;
	}
	return (true);
}

void ServerInf::getMethods(std::stringstream &ss, Location &tmp)
{
	std::string el;
	ss >> el;

	while (!el.empty())
	{
		if (el == "GET" || el == "POST" || el == "DELETE")
		{
			tmp.methods.push_back(el);
		}
		else if (el == ";")
		{
			return; // Correctly found the end of the method list
		}
		else
		{
			throw "Unsupported method"; // Unsupported method encountered
		}
		if (!(ss >> el))
		{
			throw "Missing ; on methods"; // End of stream reached without finding ';'
		}
	}
	if (tmp.methods.empty())
		throw "Missing methods"; // Throw if no methods were specified
}

std::string ServerInf::filloc(std::ifstream &inFile, Location &tmp)
{
	std::string el;
	std::string ell;
	std::string line;

	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);

		ss >> el;
		if ("default_file" == el)
		{
			ss >> el; // Read the file name
			if (el.empty())
				throw "Missing file name in default_file directive";
			std::string nextToken;
			ss >> nextToken; // Attempt to read the next token, which should be the semicolon
			if (nextToken != ";")
				throw "Missing or incorrect delimiter in default_file directive";
			tmp.default_file = el;
		}
		else if ("methods" == el)
			getMethods(ss, tmp);
		else if ("cgi_bin" == el)
		{
			ss >> el >> ell;
			if (el.empty() || !chekFilld(ss, s))
				throw "Missing element or extra elements in cgi_bin directive";
			if (ell != ";")
				throw "Missing or incorrect delimiter";
			// locs.back().cgi_bin.push_back(el);
			tmp.cgi_bin.push_back(el);
		}
		else if ("cgi_extension" == el)
		{
			ss >> el >> ell;
			if (el.empty() || !chekFilld(ss, s))
				throw "Missing element or extra elements in cgi_extension directive";
			if (ell != ";")
				throw "Missing or incorrect delimiter";
			tmp.cgi_extension.push_back(el);
		}
		else if ("}" == el)
			break;
	}
	return (line);
}

void ServerInf::reset()
{
	ports.clear();
	server_name.clear();
	error_pages.clear();
	body_size = 0;
	root.clear();
	locs.clear();
}

std::string ServerInf::filltmp(std::ifstream &inFile)
{
	std::string el;
	std::string ell;
	std::string line;

	while (std::getline(inFile, line))
	{
		Location ltmp;
		std::stringstream ss(line);
		ss >> el;
		if (el == "listen")
		{
			el.clear();
			el = fillports(ss);
		}
		else if (el == "server_name")
		{
			el.clear();
			ss >> el;
			server_name = el;
			ss >> el;
		}
		else if (el == "error_pages")
		{
			el.clear();
			el = ferrp(ss);
		}
		else if (el == "client_body_size")
		{
			el.clear();
			el = filcbs(ss);
		}

		else if (el == "root")
		{
			el.clear();
			el = filroot(ss);
		}
		else if (el == "location")
		{
			el.clear();
			ss >> el >> ell;
			if (el.empty() || !ell.empty())
			{
				throw "Location context related error.";
			}
			ltmp.path.push_back(el);
			std::getline(inFile, line);
			if (chekFilld(ss, s))
			{
				ServerInf::isbrac(line, '{');
				line = filloc(inFile, ltmp);
			}
			if (ServerInf::isbrac(line, '}'))
			{
				locs.push_back(ltmp);
				continue;
			}
		}
		else if (el == "}")
			break;
		if (el != ";" && el != locs.back().path.back())
		{
			throw "Missing ;";
		}
	}
	return (line);
}

void	ServerInf::print() const
{
	std::cout << "Server Name: " << server_name << std::endl;
	std::cout << "Ports: ";
	for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	std::cout << "Error Pages: ";
	for (std::vector<std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	std::cout << "Body Size: " << body_size << std::endl;
	std::cout << "Root: " << root << std::endl;
	std::cout << "Location: ";
	for (std::vector<Location>::const_iterator it = locs.begin(); it != locs.end(); ++it)
	{
		std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<"\n";
		std::cout << "Path: " << *(*it).path.begin() << "\n";
		std::cout << "methods:	" << *(*it).methods.begin() << "\n";
		std::cout << "Default_file:	" << (*it).default_file << "\n";
		std::cout << "cgi_bin:	" << *(*it).cgi_bin.begin() << "\n";
		std::cout << "cgi_extension:	" << *(*it).cgi_extension.begin() << "\n";
		std::cout << "BACK Path: " << (*it).path.back() << "\n";
		std::cout << "BACK methods:	" << (*it).methods.back() << "\n";
		std::cout << "BACK Default_file:	" << (*it).default_file << "\n";
		std::cout << "BACK cgi_bin:	" << (*it).cgi_bin.back() << "\n";
		std::cout << "BACK cgi_extension:	" << (*it).cgi_extension.back() << "\n";
		std::cout << "			Increment location" <<"\n";
		std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<"\n";

	}
	std::cout << std::endl;
}
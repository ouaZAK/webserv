/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInf.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asidqi <asidqi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 19:43:49 by asidqi            #+#    #+#             */
/*   Updated: 2024/02/20 21:57:26 by asidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerInf.hpp"

std::vector<int> ServerInf::getPorts()
{
	return (ports);
}

std::string ServerInf::getServName()
{
	return (server_name);
}

std::string ServerInf::getRoot()
{
	return (root);
}

int ServerInf::getBodySize()
{
	return (body_size);
}

std::string ServerInf::s = "{";

std::string ServerInf::filcbs(std::stringstream &ss)
{
    std::string el;
    if (!(ss >> el))
        throw "Failed to read body size directive";
    while (el != ";" && !el.empty())
    {
        body_size = atoi(el.c_str());
        if (!(ss >> el))
            break;
    }
    if (el != ";")
        throw "Body size directive's syntax error";
    return el;
}
std::string ServerInf::ferrp(std::stringstream &ss)
{
    std::string el;
    if (!(ss >> el))
        throw "Failed to read error page directive";

    while (el != ";" && !el.empty())
    {
        error_pages.push_back(el);
        if (!(ss >> el))
            break;
    }
    if (el != ";")
        throw "Error page directive's syntax error";
    return el;
}

std::string ServerInf::filroot(std::stringstream &ss)
{
	std::string el;
	if (!(ss >> el))
		throw "Failed to read root directive";
	if (el[0] == '/' || el.back() == '/')
		throw "Fix ya d*mn root.";
	while (el != ";" && !el.empty())
	{
		root = el;
		if (!(ss >> el)) // Check if the stream is in a good state
			break;
	}
	if (el != ";")
		throw "Root directive's syntax error";
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
    if (!(ss >> el))
        throw "Failed to read ports directive";
    while (el != ";" && !el.empty())
    {
        // check number if its goochie
        isNumWell(el);
        ports.push_back(strtod(el.c_str(), NULL));
        if (!(ss >> el))
            break;
    }
    if (el != ";")
        throw "Ports directive's syntax error";
    return el;
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
			else if (!tmp.default_file.empty())
				throw "default_file directive repeated!";
			std::string nextToken;
			ss >> nextToken; // Attempt to read the next token, which should be the semicolon
			if (nextToken != ";")
				throw "Missing or incorrect delimiter in default_file directive";
			tmp.default_file = el;
		}
		else if ("root" == el)
		{
			ss >> el; // Read the file name
			if (el.empty())
				throw "Missing root in root directive";
			else if (!tmp.root.empty())
				throw "Root directive repeated!";
			std::string nextToken;
			ss >> nextToken; // Attempt to read the next token, which should be the semicolon
			if (nextToken != ";")
				throw "Missing or incorrect delimiter in root directive";
			tmp.root = el;
		}
		else if ("alias" == el)
        {
            ss >> el; // Read the file name
            if (el.empty())
                throw "Missing alias in alias directive";
            else if (!tmp.alias.empty())
                throw "alias directive repeated!";
            std::string nextToken;
            ss >> nextToken; // Attempt to read the next token, which should be the semicolon
            if (nextToken != ";")
                throw "Missing or incorrect delimiter in alias directive";
            tmp.alias = el;
        }
		else if ("methods" == el)
			getMethods(ss, tmp);
		else if ("cgi_bin" == el)
		{
			ss >> el >> ell;
 			if (!tmp.cgi_bin.empty())
				throw "CGI_bin directive repeated!";
			if (el.empty() || !chekFilld(ss, s))
				throw "Missing element or extra elements in cgi_bin directive";
			if (ell != ";")
				throw "Missing or incorrect delimiter";
			// locs.back().cgi_bin.push_back(el);
			tmp.cgi_bin.push_back(el);
		}
		else if ("return" == el)
		{
			std::string	elll;
			ss >> el >> ell >> elll;
			if (!tmp.redirect_to_dir.empty())
				throw "redirect_to_dir directive repeated!";
			if (el.empty() || ell.empty() || elll.empty() || !chekFilld(ss, s))
				throw "Missing element or extra elements in redirection directive";
			if (elll != ";")
				throw "Missing or incorrect delimiter";
			tmp.redirect_status = atoi(el.c_str());
			tmp.redirect_to_dir = ell;
		}
		else if ("cgi_extension" == el)
		{
			ss >> el >> ell;
 			if (!tmp.cgi_extension.empty())
				throw "CGI_extension directive repeated!";
			if (el.empty() || !chekFilld(ss, s))
				throw "Missing element or extra elements in cgi_extension directive";
			if (ell != ";")
				throw "Missing or incorrect delimiter";
			tmp.cgi_extension.push_back(el);
		}
		else if ("autoindex" == el)
		{
			el.clear();
			ss >> el;
			if (tmp.lai)
				throw "Auto_index directive inside location is repeated!";
			tmp.lai = (el == "on") ? true : ((el != "off") ? (throw "Unknown element", false) : false);
			// std::cout << tmp.lai << "CHECK\n";
			ss >> el;
			if (el != ";")
        		throw "Autoindex directive's syntax error";
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
	host.clear();
	ai = false;
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
			if (!ports.empty())
				throw "Ports directive repeated!";
			el.clear();
			el = fillports(ss);
		}
		else if (el == "server_name")
		{
			if (!server_name.empty())
				throw "Server_name directive repeated!";
			el.clear();
			ss >> el;
			server_name = el;
			ss >> el;
			if (el != ";")
        		throw "Server_name directive's syntax error";
		}
		else if (el == "default_file")
		{
			if (!globDefFile.empty())
				throw "globDefFile directive repeated!";
			el.clear();
			ss >> el;
			globDefFile = el;
			ss >> el;
			if (el != ";")
        		throw "globDefFile directive's syntax error";
		}
		else if (el == "host")
		{
			if (!host.empty())
				throw "Host directive repeated!";
			el.clear();
			ss >> el;
			host = el;
			ss >> el;
			if (el != ";")
        		throw "Host directive's syntax error";
		}
		else if (el == "autoindex")
		{
			if (ai)
				throw "Auto_index directive repeated!";
			el.clear();
			ss >> el;
			ai = (el == "on") ? true : ((el != "off") ? (throw "Unknown element", false) : false);

			ss >> el;
			if (el != ";")
        		throw "Autoindex directive's syntax error";
		}
		else if (el == "error_pages")
		{
			if (!error_pages.empty())
				throw "Error_pages directive repeated!";
			el.clear();
			el = ferrp(ss);
		}
		else if (el == "client_body_size")
		{
			// if (body_size.)
			// 	throw "clien_body_size directive repeated!";
			el.clear();
			el = filcbs(ss);
		}

		else if (el == "root")
		{
			if (!root.empty())
				throw "Root directive repeated!";
			el.clear();
			el = filroot(ss);
		}
		else if (el == "location")
		{
			ltmp.redirect_status = 0;
			el.clear();
			ss >> el >> ell;
			if (el.empty() || !ell.empty())
				throw "Location context related error.";
			if (el[0] != '/' || el.back() != '/')
				throw "Fix ya d*mn location path.";
			ltmp.locDirName.push_back(el);
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
		else if (!el.empty())//needs checking
			throw "Unknown element";
		if (el != ";" && el != locs.back().locDirName.back())
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
		std::cout << "Path: " << *(*it).locDirName.begin() << "\n";
		std::cout << "methods:	" << *(*it).methods.begin() << "\n";
		std::cout << "Default_file:	" << (*it).default_file << "\n";
		std::cout << "cgi_bin:	" << *(*it).cgi_bin.begin() << "\n";
		std::cout << "cgi_extension:	" << *(*it).cgi_extension.begin() << "\n";
		std::cout << "BACK Path: " << (*it).locDirName.back() << "\n";
		std::cout << "BACK methods:	" << (*it).methods.back() << "\n";
		std::cout << "BACK Default_file:	" << (*it).default_file << "\n";
		std::cout << "BACK cgi_bin:	" << (*it).cgi_bin.back() << "\n";
		std::cout << "BACK cgi_extension:	" << (*it).cgi_extension.back() << "\n";
		std::cout << "BACK redirect_to_dir:	" << (*it).redirect_to_dir << "\n";
		std::cout << "BACK redirect_status:	" << (*it).redirect_status << "\n";
		std::cout << "			Increment location" <<"\n";
		std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<"\n";

	}
	std::cout << std::endl;
}
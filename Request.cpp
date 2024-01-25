/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcharia < hcharia@student.1337.ma>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:28:05 by hcharia           #+#    #+#             */
/*   Updated: 2024/01/25 11:55:13 by hcharia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
}

Request::Request(std::string input) : postBody("")
{
	std::stringstream ss;
	std::string line;

	ss << input;
	std::getline(ss, line);
	prl(line);
	while (std::getline(ss, line) && line != "\r")
	{
		size_t pos = line.find(':', 0);
		if (pos == std::string::npos)
		{
			std::cout << "error" << std::endl;
			exit(1);
		}
		// this is for eleminating spaces
		std::stringstream ss2;
		std::string second;
		ss2 << line.substr(pos + 1);
		ss2 >> second;
		//this is for eleminating spaces
		headers.insert(std::pair<std::string, std::string>(line.substr(0, pos), second));
	}
	if (method == "POST")
	{
		while (std::getline(ss, line))
			postBody += line + '\n';
	}
}

Request::~Request()
{
}

std::string 						Request::get_method()
{
	return method;
}

std::string							Request::get_path()
{
	return path;
}

std::string							Request::get_version()
{
	return version;
}

std::map<std::string, std::string>	Request::get_headers()
{
	return headers;
}
int									Request::get_status()
{
	return status;
}

void								Request::set_body(std::string& s)
{
	postBody = s;
}

void								Request::prl(std::string line) // parse request line
{
	std::stringstream	ss;
	std::string			result;
	int					i;

	i = 0;
	ss << line;
	while (ss >> result)
	{
		if (i == 0)
			method = result;
		else if (i == 1)
			path = result;
		else if (i == 2)
			version = result;
		i++;
	}
	if (method != "POST" && method != "DELETE" && method != "GET")
	{
		std::cout << "wrong method" << std::endl;
		exit (1);
	}	
}

std::string							Request::get_file_name()
{
	 std::string str;
	size_t pos = postBody.find("filename");
	if (pos == std::string::npos)
	{
		std::cout << "no filename found" << std::endl;
		return (NULL);
	}
	str = postBody.substr(pos + 10);
	pos = str.find("\"");
	if (pos == std::string::npos)
	{
		std::cout << "there is a  problem in filename format :p";
		return (NULL);
	}
	str = str.substr(0, pos);
	return (str);
}

std::string							Request::get_body()
{
	return (postBody);
}

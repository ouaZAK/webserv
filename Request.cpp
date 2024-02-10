/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:28:05 by hcharia           #+#    #+#             */
/*   Updated: 2024/02/10 12:05:40 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request()
{
}

void	Request::set_status(int s)
{
	status = s;
}


Request::Request(std::string input) : status(200), postBody("")
{
	std::stringstream ss;
	std::string line;

	ss << input;
	std::getline(ss, line);
	prl(line);
	// if (status == BAD_REQUEST)
	// {
	// 		std::cout << "************************** findddddddd **************************\n";
	// 	return ;
	// }
	while (std::getline(ss, line) && line != "\r")
	{
		size_t pos = line.find(':', 0);
		if (pos == std::string::npos)
		{
			status = BAD_REQUEST;
			std::cout << "************************** poooooooosss **************************\n";
			return ;
		}
		if (line.find("\r") == std::string::npos || line[line.find("\r") + 1] != 0)
		{
			std::cout << "anaaaaaaaa hnaaaa 1\n";
			status = BAD_REQUEST;
			return ;
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

int	Request::get_status()
{
	return status;
}

void	Request::set_body(std::string& s)
{
	postBody = s;
}
std::string	Request::get_path()
{
	return path;
}

std::string	Request::get_version()
{
	return version;
}



std::map<std::string, std::string>	Request::get_headers()
{
	return headers;
}

void	Request::prl(std::string line) // parse request line
{
	std::stringstream	ss;
	std::string			result;
	int					i;

	i = 0;
	ss << line;
	if (line.find("\r") == std::string::npos || line[line.find("\r") + 1] != 0)
	{
			std::cout << "anaaaaaaaa hnaaaa 2\n";

		status = BAD_REQUEST;
		return ;
	}
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
	
	if (ss >> result || (method != "POST" && method != "DELETE" && method != "GET") 
		|| version != "HTTP/1.1")
	{
		std::cout << "anaaaaaaaa hnaaaa 3\n";
		status = BAD_REQUEST;
		return ;
	}
	if (path.size() >= 2048)
	{
		status = REQ_URL_L;
		return ;
	}
	version = version.substr(0, version.find('\r'));
	for (int i = 0; path[i]; i++)
	{
		if (!isprint(path[i]) || path[i] == '\"' 
		 || path[i] == ';' || path[i] == '>' || path[i] == '<' 
		 || path[i] == '\\' ||  (path[i] >= '[' && path[i] <= '^') 
		 || path[i] == '`' ||  (path[i] >= '{' && path[i] <= '}'))
		{
			std::cout << "anaaaaaaaa hnaaaa 4\n";
			status = BAD_REQUEST;
			return ;
		}
	}
}

std::string							Request::get_file_name()
{
	 std::string str;
	size_t pos = postBody.find("filename");
	if (pos == std::string::npos)
	{
		std::cout << "no filename found" << std::endl;
		return ("");
	}
	str = postBody.substr(pos + 10);
	pos = str.find("\"");
	if (pos == std::string::npos)
	{
		std::cout << "there is a  problem in filename format :p";
		return ("");
	}
	str = str.substr(0, pos);
	return (str);
}

std::string							Request::get_body()
{
	return (postBody);
}

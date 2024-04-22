/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:28:05 by hcharia           #+#    #+#             */
/*   Updated: 2024/04/22 12:40:54 by zouaraqa         ###   ########.fr       */
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


Request::Request(std::string input) : status(200), postBody(""), query("")
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
			status = BAD_REQUEST;
			return ;
		}
		if (line.find("\r") == std::string::npos || line[line.find("\r") + 1] != 0)
		{
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
	set_query();
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

std::string Request::getCleanBody()
{
	return postBody;
}

std::map<std::string, std::string>	Request::get_headers()
{
	return headers;
}

void	Request::prl(std::string line)
{
	std::stringstream	ss;
	std::string			result;
	int					i;

	i = 0;
	ss << line;
	if (line.find("\r") == std::string::npos || line[line.find("\r") + 1] != 0)
	{
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
			status = BAD_REQUEST;
			return ;
		}
	}
}

std::string		Request::get_file_name()
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

std::string		Request::get_body()
{
	return (postBody);
}

void		Request::set_query()
{
	std::string new_path;
	std::string			result;
	
	if (method != "GET")
		return ;
	if (path.find("?") != std::string::npos)
	{
		query = path.substr(path.find("?") + 1);
		new_path = path.substr(0, path.find("?"));
		orig_query = query;
		std::replace(query.begin(), query.end(), '&', ' ');
		path = new_path;
	}
}

std::string		Request::get_query()
{
	return (query);
}

std::string		Request::get_oquery()
{
	return (orig_query);
}


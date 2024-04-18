/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:28:05 by hcharia           #+#    #+#             */
/*   Updated: 2024/04/18 15:06:16 by zouaraqa         ###   ########.fr       */
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
	// std::cout << "we are heeeeeeeeeerereerererererer\n";
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

void	Request::prl(std::string line) // parse request line
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
		// std::cout << "lalalalala*********************\n***************\n********************\n";
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
	// std::stringstream	ss;
	std::string			result;
	
	//std::cout << "we are heeheheheehehehehehe 2\n";
	if (method != "GET")
		return ;
	//std::cout << "we are heeheheheehehehehehe 3\n";
	if (path.find("?") != std::string::npos)
	{
		//std::cout << "we are heeheheheehehehehehe 4\n";
		query = path.substr(path.find("?") + 1);
		//std::cout << "this is queryyyyyy : " + query << std::endl;
		new_path = path.substr(0, path.find("?"));
		//std::cout << "this is dummyyyyyy : " + dummy << std::endl;
		orig_query = query;
		std::replace(query.begin(), query.end(), '&', ' ');
		// ss << query;
		// while (ss >> result)
		// {
		// 	query_vec.push_back(result);
		// }
		path = new_path;
		// vecToChars();
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

// char	**Request::get_qvv() const
// {
// 	return (qvv);
// }


// void	Request::vecToChars()
// {
//     try
//     {
//         qvv = new char *[query_vec.size() + 1];
//         int i = 0;
//         std::string entry;
//         for (std::vector<std::string>::iterator it = query_vec.begin(); it != query_vec.end(); ++it)
//         {
//             entry.clear();
//             entry = *(it);
//             qvv[i] = new char[entry.length() + 1];
//             std::strcpy(qvv[i], entry.c_str());
//             i++;
//         }
//         qvv[i] = NULL;
//     }
//     catch (const std::exception &e)
//     {
//         std::cout << e.what() << '\n';
//     }
// }
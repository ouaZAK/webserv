/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 19:50:28 by asidqi            #+#    #+#             */
/*   Updated: 2024/02/20 17:15:23 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autoindex.hpp"

autoindex::autoindex(std::string root, std::string url, std::string host, int p) : server_root(root), urlPath(url), host_name(host), port(p)
{
}

autoindex::~autoindex()
	{
	}


autoindex &autoindex::operator=(const autoindex &other)
	{
		if (this == &other)
			return (*this);
		return (*this);
	}
std::string autoindex::pageGen()
{
	std::string pg = "<!DOCTYPE html>\n"
					 "<html lang=\"en\">\n"
					 "<head>\n"
					 "\t<meta charset=\"UTF-8\">\n"
					 "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
					 "\t<title>Index file </title>\n"
					 "</head>\n"
					 "<body>\n"
					 "\t<header>\n"
					 "\t\t<h1>" +
					 urlPath + "</h1>\n"
								   "\t</header>\n"
								   "\t<section>\n"
								   "\t\t<ul>";
	pg += linkGen(server_root, host_name, port, urlPath) + "</ul>\n"
												  "\t  </section>\n"
												  "</body>\n"
												  "</html>";
	return (pg);
}

std::string autoindex::linkGen(std::string server_root, std::string host_name, int port, std::string urlPath)
{
	std::string pg;
	DIR *dir;
	struct dirent *entry;
	std::cout << urlPath << '\n';
	urlPath == "/" ? urlPath	: urlPath += "/";
	dir = opendir((server_root + urlPath).c_str());
	if (dir == NULL)
	{
		perror("opendir");
		return (pg); // throw exception perhaps?
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
			continue;
		std::cout << std::string(entry->d_name) << "\n";
		std::string hyperlink = "\t\t<p><a href=\"http://" + host_name + ":" + std::to_string(port) + urlPath + std::string(entry->d_name) + "\">" + std::string(entry->d_name) + "</a></p>\n";
		pg += "<li>" + hyperlink + "</li>\n";
	}

	closedir(dir);
	return (pg);
}
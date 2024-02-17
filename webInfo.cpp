/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webInfo.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 08:58:26 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/02/16 10:26:58 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webInfo.hpp"

webInfo::webInfo()
{
}

unsigned int r(unsigned int d1, unsigned int d2,unsigned int d3,unsigned int d4)
{
	return (d1 << 24 | d2 << 16 | d3 << 8 | d4);
}
unsigned int	convert(std::string str)
{
	if (str.empty())
		return (INADDR_ANY);// any address
	if (str == "localhost")
		str = "127.0.0.1";
	size_t pos1, pos2, pos3;
	pos1 = str.find(".",0);
	std::string s1, s2, s3, s4;
	s1 = str.substr(0, pos1);
	std::cout << "s1: " << s1 << '\n';
	pos2 = str.find(".", pos1 + 1);
	s2 = str.substr(pos1 + 1, pos2 - (pos1 + 1));
	std::cout << "s2: " << s2 << '\n';
	pos3 = str.find(".", pos2 + 1);
	s3 = str.substr(pos2 + 1, pos3 - (pos2 + 1));
	std::cout << "s3: " << s3 << '\n';
	s4 = str.substr(pos3 + 1, str.length() - (pos3 + 1));
	std::cout << "s4: " << s4 << '\n';

	double d1, d2, d3, d4;
	d1 = strtod(s1.c_str(), NULL);
	d2 = strtod(s2.c_str(), NULL);
	d3 = strtod(s3.c_str(), NULL);
	d4 = strtod(s4.c_str(), NULL);
	unsigned int uc = r(d1, d2, d3, d4);
	std::cout << "uc : " << uc << '\n';
	return (uc);
}

webInfo::webInfo(int port, ServerInf &inf) : port(port)
{
	this->ai = inf.getAI();
	locationVec = inf.locs;
	serverName = inf.getServName();
	root = inf.getRoot();
	bodySize = inf.getBodySize();
	if (bodySize == -1)
		bodySize = 1000000;
	errorPages = inf.getErrorPages();
	//print
	std::cout << "\nserv name is : " << serverName << '\n' << "port nbr : " << port << '\n';
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		std::cout << "failed to create server socket" << std::endl;
		exit(1);
	}
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	unsigned int host = convert(inf.getHost());
	serverAddress.sin_addr.s_addr = htonl(host);
}

int webInfo::getPort() const
{
	return (port);
}

size_t webInfo::getBodySize() const
{
	return (bodySize);
}

int webInfo::getSock() const
{
	return (sock);
}

struct sockaddr_in webInfo::getServerAddress() const
{
	return (serverAddress);
}

std::string webInfo::getRoot() const
{
	return (root);
}

std::vector<Location> webInfo::getLoc() const
{
	return (locationVec);
}

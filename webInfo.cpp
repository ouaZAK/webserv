/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webInfo.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 08:58:26 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/04/18 09:40:42 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webInfo.hpp"

webInfo::webInfo()
{
}

unsigned int shift(unsigned int d1, unsigned int d2, unsigned int d3, unsigned int d4)
{
	return (d1 << 24 | d2 << 16 | d3 << 8 | d4);
}

unsigned int	convert(std::string str)
{
	if (str.empty())
		return (INADDR_ANY);
	if (str == "localhost")
		str = "127.0.0.1";
	std::string s1, s2, s3, s4;
	size_t 		pos1, pos2, pos3;
	
	pos1 = str.find(".",0);
	s1 = str.substr(0, pos1);
	pos2 = str.find(".", pos1 + 1);
	s2 = str.substr(pos1 + 1, pos2 - (pos1 + 1));
	pos3 = str.find(".", pos2 + 1);
	s3 = str.substr(pos2 + 1, pos3 - (pos2 + 1));
	s4 = str.substr(pos3 + 1, str.length() - (pos3 + 1));

	double d1, d2, d3, d4;
	d1 = strtod(s1.c_str(), NULL);
	d2 = strtod(s2.c_str(), NULL);
	d3 = strtod(s3.c_str(), NULL);
	d4 = strtod(s4.c_str(), NULL);
	unsigned int uc = shift(d1, d2, d3, d4);
	return (uc);
}

webInfo::webInfo(int port, ServerInf &inf, int oneS, int oneSrv) : port(port), oneSock(oneS), oneServer(oneSrv)
{
	serverName = inf.getServName();
	root = inf.getRoot();
	host = inf.getHost();
	ai = inf.getAI();
	locationVec = inf.locs;
	errorPages = inf.getErrorPages();
	bodySize = inf.getBodySize();
	globDefFile = inf.getGlobDefFile();
	// if (bodySize == -1)
	// 	bodySize = 1000000;
	// std::cout << "\nserv name is : " << serverName << '\n' << "port nbr : " << port << '\n';
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		std::cout << "failed to create server socket" << std::endl;
		return ;
	}
	memset(&serverAddress, 0, sizeof serverAddress);
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
std::vector<std::string> webInfo::getErrorPages() const
{
	return (errorPages);
}
bool webInfo::getAI() const
{
	return ai;
}
std::string	webInfo::getHost() const
{
	return (host);
}

int	webInfo::getOneSock() const
{
	return (oneSock);
}
int	webInfo::getOneServer() const
{
	return (oneServer);
}
std::string	webInfo::getGlobDefFile() const
{
	return (globDefFile);
}

std::string		webInfo::getServnam() const
{
	return (serverName);
}

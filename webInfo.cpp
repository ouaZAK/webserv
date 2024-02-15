/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webInfo.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 08:58:26 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/02/15 18:14:37 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webInfo.hpp"

webInfo::webInfo()
{
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
	// serverAddress.sin_addr.s_addr = htonl();
	serverAddress.sin_addr.s_addr = INADDR_ANY;
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

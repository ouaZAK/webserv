/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 11:30:21 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/03/02 17:03:33 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <signal.h>

void fun(int i)
{
	(void)i;
}

int main(int ac, char **av)
{
	signal(SIGPIPE, fun);
	(void)ac;
	std::map<std::string, std::string>	mimeMap;
	std::vector<ServerInf>				serverInf;
	std::vector<webInfo>				listWebInfo;
	std::vector<int>					ports;
	int									oneSock, oneServer;
	try
	{
		serverInf = confInf(av);
		oneServer = serverInf.size();
		// std::cout << "size sz : " << oneServer << std::endl;
		mimeMap = populateMimeMap();
		for (std::vector<ServerInf>::iterator infIt = serverInf.begin(); infIt != serverInf.end(); ++infIt)
		{
			ports = infIt->getPorts();
			oneSock = ports.size();
		// std::cout << "p sz : " << ports.size() << std::endl;
			for (std::vector<int>::iterator itV = ports.begin(); itV != ports.end(); itV++)
			{
				webInfo info(*itV, *infIt, oneSock, oneServer);
				if (info.getSock() != -1)
					listWebInfo.push_back(info);
			}
		}
		if (!listWebInfo.size())
			return (1);
	}
	catch(const std::exception& e)
	{
		std::cout << "$$$$$$$------- Error -----------$$$$$$$$\n" << e.what() << '\n';
		return (1);
	}
	catch(const char* e)
	{
		std::cout << e << '\n';
		return (1);
	}
	
	try
	{
		webserv start(listWebInfo, mimeMap);
	}
	catch(const std::exception& e)
	{
		std::cout << "$$$$$$$------- Error -----------$$$$$$$$\n" << e.what() << '\n';
		return (1);
	}
	catch(const char* e)
	{
		std::cout << e << '\n';
		return (1);
	}
	return (0);
}

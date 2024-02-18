/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webInfo.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 08:58:37 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/02/18 10:56:07 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "ServerInf.hpp"

class webInfo
{
	private:
		struct sockaddr_in			serverAddress;
		std::string 				serverName;
		std::string 				root;
		std::string 				host;
		int 						bodySize;
		int 						port;
		int 						sock;
		bool 						ai;
		std::vector<Location>		locationVec;
		std::vector<std::string>	errorPages;
		int							oneSock;
		int							oneServer;
	public:
		webInfo();
		webInfo(int port, ServerInf &inf, int oneS, int oneServer);
		
		int							getPort() const;
		int							getSock() const;
		int							getOneSock() const;
		int							getOneServer() const;
		size_t 						getBodySize() const;
		std::vector<Location>		getLoc() const;
		struct sockaddr_in			getServerAddress() const;
		std::string					getRoot()const;
		std::vector<std::string>	getErrorPages() const;
		bool						getAI() const;
		std::string					getHost() const;
};
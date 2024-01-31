/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webInfo.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 08:58:37 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/01/29 17:26:12 by zouaraqa         ###   ########.fr       */
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
		struct sockaddr_in		serverAddress;
		std::string 			serverName;
		std::string 			root;
		int 					bodySize;
		int 					port;
		int 					sock;
		std::vector<Location>	locationVec;
		// std::vector<std::string> error_pages;
	public:
		webInfo();
		webInfo(int port, ServerInf &inf);
		
		int						getPort() const;
		int						getSock() const;
		std::vector<Location>	getLoc() const;
		struct sockaddr_in		getServerAddress() const;
		std::string				getRoot()const;
};
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:15:31 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/01/25 11:20:57 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sstream>
#include <fstream>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fcntl.h>
#include <map>
#include <vector>


#include "Request.hpp"
#include "ServerInf.hpp"

// #define PORT 8080
#define MAX_CLIENTS 10

class webInfo
{
	private:
		struct sockaddr_in	serverAddress;
		std::string 		serverName;
		std::string 		root;
		int 				bodySize;
		int 				port;
		int 				sock;
		// std::vector<std::string> error_pages;
	public:
		webInfo(){}
		webInfo(int port, ServerInf &inf) : port(port)
		{
			//print
			std::cout << "\nserv name is : " << serverName << '\n' << "port nbr : " << port << '\n';

			serverName = inf.getServName();
			root = inf.getRoot();
			bodySize = inf.getBodySize();
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock == -1)
			{
				std::cout << "failed to create server socket" << std::endl;
				exit(1);
			}
			serverAddress.sin_family = AF_INET;
			serverAddress.sin_port = htons(port);
			serverAddress.sin_addr.s_addr = INADDR_ANY;
		}
		
		int getPort() const
		{
			return (port);
		}
		int getSock() const
		{
			return (sock);
		}
		struct sockaddr_in getServerAddress() const
		{
			return (serverAddress);
		}
		std::string getRoot()
		{
			return (root);
		}
};

class clientInfo
{
	private:
		std::string reqFull;
		std::string reqChunk;
		std::string root;
	public:
		void	setReqFull(std::string cnt)
		{
			reqFull = cnt;
		}
		std::string getReqFull() const
		{
			return (reqFull);
		}

		void	setReqChunk(std::string cnt)
		{
			reqChunk = cnt;
		}
		std::string getReqChunk() const
		{
			return (reqChunk);
		}
		void	reqClear()
		{
			reqChunk.clear();
		}
		std::string getRoot()
		{
			return (root);
		}
		void	setRoot(std::string str)
		{
			root = str;
		}
};

class webserv
{
	private:
		clientInfo 							cliento;
		std::map<int, webInfo> 				serverMap;
		std::map<int, webInfo>::iterator 	mapIt;
		std::vector<struct sockaddr_in>		serverAddress;
		std::vector<struct sockaddr_in>		clientAddress;
		socklen_t							clientAddressLen;
		int									newClientSocket;
		std::map<int, clientInfo>			clientMap;
		int									maxSocket;
		std::vector<webInfo>				serverSocket;
		// int								serverSocket;
		char								buff[3000];
		std::string 						reqContent;
		fd_set								read_set;
		fd_set								write_set;
		fd_set 								copyRead;
		fd_set 								copyWrite;

		std::string cleanBody;
		std::string body;
		size_t bodyLength;
		size_t pos;
		// int stop;
	public:
		webserv(){}
		webserv(std::vector<webInfo> &servervector);
		~webserv();
		std::map<int, webInfo>  getmap() const
		{
			return (serverMap);
		}
		void	setNoBlocking();
		void	creatAddresses();
		void	bindSockets();
		void	vectorening();
		void	setFds();
		void	listening();
		void	acceptSockets(int i);
		void	reading(int i);
		void	writing(int i);
		int 	parse_the_request(int i);
		void	extractBody(int i);
		bool	getRequest(Request req, int i);

		// void	updateMaxSocket(int i);

		// virtual int connectToNetwork(int sock, struct sockaddr_in address) = 0;
		// void	testConnection(int);
};


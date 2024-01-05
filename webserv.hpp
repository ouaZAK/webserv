
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
#include <list>

// #define PORT 8080
#define MAX_CLIENTS 4

class webInfo
{
	private:
		struct sockaddr_in serverAddress;
		int port;
		int sock;
		webInfo(){}
	public:
		webInfo(int port) : port(port)
		{
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
};

class webserv
{
	private:
		std::map<int, webInfo> serverMap;
		std::map<int, webInfo>::iterator mapIt;
		std::list<struct sockaddr_in>	serverAddress;
		std::list<struct sockaddr_in>	clientAddress;
		socklen_t			clientAddressLen;
		std::list<int>		newClientSocket;
		int					maxSocket;
		std::list<webInfo>	serverSocket;
		// int					serverSocket;
		char				buff[3000];
		fd_set				read_set;
		fd_set				write_set;
		fd_set 				copyRead;
		fd_set 				copyWrite;
	public:
		webserv(){}
		webserv(std::list<webInfo> &serverList);
		~webserv();
		void	setNoBlocking();
		void	creatAddresses();
		void	bindSockets();
		void	listening();
		void	setFds();
		void	acceptSockets(int i);
		// void	updateMaxSocket(int i);

		// virtual int connectToNetwork(int sock, struct sockaddr_in address) = 0;
		// void	testConnection(int);
};


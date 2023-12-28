
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

#define PORT 8080
#define MAX_CLIENTS 4
class webserv
{
	private:
		struct sockaddr_in	serverAddress;
		struct sockaddr_in	clientAddress;
		socklen_t			clientAddressLen;
		// int					newSocket;
		int					newClientSocket;
		int					maxSocket;
		int					serverSocket;
		// int					clientSockets[MAX_CLIENTS];
		char				buff[3000];
		fd_set				read_set;
		// fd_set				write_set;
	public:
		webserv();
		~webserv();

		// virtual int connectToNetwork(int sock, struct sockaddr_in address) = 0;
		// void	testConnection(int);
};


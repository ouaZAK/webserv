
#pragma once

#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>

class webserv
{
	private:
		struct sockaddr_in	address;	
		int					sock;
		int					connection;
	public:
		webserv(int domain, int service, int protocol, int port, size_t interface);	
		~webserv();	
		
		virtual int connectToNetwork(int sock, struct sockaddr_in address) = 0;
		void	testConnection(int);
};


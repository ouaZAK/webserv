/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:15:31 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/02/17 11:56:37 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sstream>
#include <fstream>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>
#include <map>
#include <vector>
#include <sys/stat.h>


#include "Response.hpp"
#include "Request.hpp"
#include "ServerInf.hpp"
#include "webInfo.hpp"
#include "clientInfo.hpp"
#include "autoindex.hpp"

// #define PORT 8080
#define MAX_CLIENTS 10

class response
{
	private:
		std::string reqFull;
	public:
		void	setReqFull(std::string cnt)
		{
			reqFull = cnt;
		}

		std::string getReqFull() const
		{
			return (reqFull);
		}
};

class webserv
{
	private:
		std::map<int, response>				respMap;
		std::map<int, response>::iterator	respMapIt;
		
		std::map<std::string, std::string>	mimeMap;
		clientInfo 							clientInf;
		std::map<int, webInfo> 				serverMap;
		std::map<int, webInfo>::iterator 	mapIt;
		std::vector<struct sockaddr_in>		serverAddress;
		std::vector<struct sockaddr_in>		clientAddress;
		socklen_t							clientAddressLen;
		int									newClientSocket;
		std::map<int, clientInfo>			clientMap;
		std::map<int, clientInfo>::iterator	cliMapIt;
		int									maxSocket;
		int 								check;
		std::vector<webInfo>				serverSocket;
		char								*buff;
		std::string 						reqContent;
		fd_set								read_set;
		fd_set								write_set;
		fd_set 								copyRead;
		fd_set 								copyWrite;
		
		bool								is_dir;
		std::string							urlPath;
		std::string 						cleanBody;
		std::string 						body;
		std::string 						empty;
		size_t 								bodyLength;
		size_t 								pos;
		bool 								resError;
		std::string htmlFile;
		
	public:
		webserv(){}
		webserv(std::vector<webInfo> &servervector, std::map<std::string, std::string>	mime);
		~webserv();

		
		std::map<int, webInfo>  getmap() const;
		void					setNoBlocking();
		void					creatAddresses();
		void					bindSockets();
		void					setFds();
		void					listening();
		void					acceptSockets(int i);
		void					reading(int i);
		void					writing(int i);
		void					extractBody(int i);
		bool					getRequest(int i);
		std::string				serveFile(int i);
		void					checkLocMeth(int i);
		bool 					check_dir(int i, std::string dir);
		void					setResStatus(int i, int status, std::string &htmlFile, std::string statusHtml);
		// void	updateMaxSocket(int i);

		// virtual int connectToNetwork(int sock, struct sockaddr_in address) = 0;
		// void	testConnection(int);
};


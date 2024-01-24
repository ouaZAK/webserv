/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:04:51 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/01/24 09:34:24 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string readFile(const std::string &str)
{
	std::ifstream file(str, std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "failed to pen file" << std::endl;
		return "";
	}

	std::stringstream inStr;
	inStr << file.rdbuf();
	return (inStr.str());
}

int webserv::parse_the_request(int i)
{
	std::cout << i << std::endl;
	// if (reqContent.find("GET", 0) != std::string::npos)
	// {
	// 	std::cout << "\n############### the request is : \n" << reqContent << std::endl;
	// 	return (1);
	// }
	// else
	// 	std::cout << "no get" << std::endl;
	return (1);
	// buff.substr(4, buff.find('\n', 0))
	// std::cout << buff.find('\n', 0) - buff.find(' ', 5) << std::endl;
}

void	webserv::setNoBlocking()
{
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	{
		int flags = fcntl(mapIt->first, F_GETFL, 0);
		fcntl(mapIt->first, F_SETFL, flags | O_NONBLOCK);
	}
}

void	webserv::creatAddresses()
{
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
		serverAddress.push_back(mapIt->second.getServerAddress());
}

void	webserv::bindSockets()
{
	std::vector<struct sockaddr_in>::iterator adrIt = serverAddress.begin();
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	{
		if (bind(mapIt->first, (struct sockaddr *)&(*adrIt), sizeof(*adrIt)) < 0)
		{
			std::cout << "failed to bind server socket" << std::endl;
			return;
		}
		++adrIt;
	}
}

void	webserv::listening()
{
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	{
		if (listen(mapIt->first, 5) < 0)
		{
			std::cout << "failed to listen" << std::endl;
			return;
		}
	}
}

void	webserv::setFds()
{
	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	FD_ZERO(&copyRead);
	FD_ZERO(&copyWrite);
	maxSocket = serverMap.begin()->first;
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	{
		FD_SET(mapIt->first, &read_set);
		if (maxSocket < mapIt->first)
			maxSocket = mapIt->first;
	}
}

void	webserv::acceptSockets(int i)
{
	// for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	// {
		//if sock server is in read which is always in read whadaheck
		if (FD_ISSET(i, &copyRead))
		{
		// std::cout << "server " << i  << " accepted"<< std::endl;

			// accept new connection
			clientAddressLen = sizeof(*clientAddress.begin());
			newClientSocket = accept(i, (struct sockaddr *)&clientAddress, &clientAddressLen);
			if (newClientSocket < 0)
			{
				// std::cout << "failed to accept" << std::endl;
				// return true;
			}
			// std::cout <<"new client accepted [" << newClientSocket << "] " <<  std::endl;
			//nonblocking fds
			int flags = fcntl(newClientSocket, F_GETFL, 0);
			fcntl(newClientSocket, F_SETFL, flags | O_NONBLOCK);

			//set newclient to copyRead
			FD_SET(newClientSocket, &read_set);
			// std::cout <<"new client set to READ " <<  std::endl;

			if (newClientSocket > maxSocket)
				maxSocket = newClientSocket;

			clientMap.insert(std::make_pair(newClientSocket, cliento));
		// std::cout << "socket is " << i  << " client sock is " << newClientSocket << '\n';

			// return false;
		}
	// }
	// return false;
}

void	webserv::reading(int i)
{
	// std::cout << "READ block " << std::endl;
	// 	std::cout << "i : " << i << std::endl;
	// if (!recStop)
	// {
		
		int bytesReaded = recv(i, buff, 1000, 0);
		if (bytesReaded < 0)
			// std::cout << "failed to recv" << std::endl;
		if (bytesReaded == 0)
			// std::cout << "the connectuion is done " << std::endl;
		// else
		// 	std::cout << "\nlen recv is < " << bytesReaded << " >\n" << std::endl;
		buff[bytesReaded] = '\0';
		std::string bufTmp;
		std::string tmp = clientMap[i].getReqCnt();
		bufTmp = std::string(buff, bytesReaded);
		tmp.append(bufTmp);
		clientMap.at(i).setReqCnt(tmp);
	// }

	// std::cout << "[[[ \n\n" << clientMap.at(i).getReqCnt() << " \n]]]" << std::endl;
	Request req(tmp);
	size_t bodyLength = std::strtod(req.get_headers()["Content-Length"].c_str(), NULL);


	// std::cout  << " " << clientMap.at(i).getReqCnt().find("\r\n\r\n", 0) << std::endl;
	// sleep(1);
	std::string tmpBody;
	std::string body;
	size_t posNwl;
	std::string cleanBody;
	if (req.get_method() == "POST")
	{
		try
		{
			pos = clientMap[i].getReqCnt().find("\r\n\r\n", 0);
			tmpBody = clientMap[i].getReqCnt();
			body = tmpBody.substr(pos + 4, tmpBody.length() - (pos + 4));
			posNwl = body.find("\r\n", 0);
			// std::cout << "\nfirst nwl " << pos << '\n' << std::endl;
			pos = body.find("\r\n\r\n", 0);
			cleanBody = body.substr(pos + 4, body.length() - (pos + 4) - (posNwl + 4));
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << "$$$$$$$$$$$$$$$$$________________$$$$$$$$$$$$$$$$$" << '\n';
		}
	}


	// std::cout << "\n\n[[[--------\n\n" << cleanBody << "\n\n------]]]\nbodylength: " 
	// 			 << body.length() << " " << bodyLength << std::endl;
		
	if (clientMap[i].getReqCnt().find("\r\n\r\n", 0) != std::string::npos)
	{
		if (req.get_method() == "POST")
		{
			if (body.length() < bodyLength)
			{
				// std::cout << "body less\n";
				return;
			}
			int fd = open("img.png", O_CREAT | O_RDWR, 0644);
			write(fd, cleanBody.c_str(), cleanBody.length());
		}
		clientMap[i].setContent(clientMap.at(i).getReqCnt());
		// std::cout << "\n@@@@@@@@@@ end of req\ni : " << i << " \n" << clientMap.at(i).getContent() << '\n'
		// 		<< body.length() << " " << bodyLength << std::endl;
		clientMap.at(i).reqClear();
	}
	else
	{
		// std::cout << "\npos = npos so returned\n";
		return ;
	}
// sleep(2);
			// std::cout << "\nreading to writing" << std::endl;

	// std::cout << "\n//--- begin req ---//\n" << std::endl;
	// std::cout << clientMap.find(i)->second.getContent() << std::endl;
	// std::cout << "//--- end req ---//\n\n" << "the request size : -> " << clientMap.find(i)->second.getContent().length() << '\n';

	//set to write on the client socket
	FD_SET(i, &write_set);
	//clear it from read cuz now it need to be writing on it only
	FD_CLR(i, &read_set);
}

void	webserv::writing(int i)
{
	// std::cout << "WRITE block " << std::endl;
	// std::cout << "here\n[ " << serverMap.find(6)->second.getContent() << "]" << std::endl;
	// std::string tmp = static_cast<std::string>(buff);
	// if (parse_the_request(i))
	// {
		// std::string	test = "GET /path/to/resource HTTP/1.1\r\nHost: example.com\r\nUser-Agent: MyTestApp\r\n\r\n";
		// Request reqHakimeeee(clientMap.at(i).getContent());
		
		// std::map <std::string, std::string> test2 = reqHakimeeee.get_headers();
		// for (std::map <std::string, std::string>::iterator a = test2.begin(); a != test2.end(); a++)
		// 	std::cout <<  a->first << " | " << a->second << std::endl;
		
		// std::cout << "hakimeeeeeeee ---->>>>>  \n" << reqHakimeeee.get_body() << '\n' << std::endl;
		// int fd = open("tst.png", O_CREAT | O_RDWR | O_TRUNC, 0644);
		// if (fd == -1)
		// 	std::cout << "im losing it\n";
		// write(fd, reqHakimeeee.get_body().c_str(), reqHakimeeee.get_body().length());

		
		std::string htmlFile;
		std::string fileContent;
		// std::cout << "i : " << i << std::endl;
		// std::cout << "\n############### the request is : \n" << clientMap.find(i)->second.getContent() << std::endl;
		if (clientMap.at(i).getContent().substr(0, 30).find(".html") != std::string::npos)
		{
			htmlFile = readFile("./stuff/example.html");
			fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: text/html\r\n\r\n" + htmlFile;
		}
		else if (clientMap.at(i).getContent().substr(0, 30).find(".js") != std::string::npos)
		{
			htmlFile = readFile("./stuff/script.js");
			fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: application/javascript\r\n\r\n" + htmlFile;
		}
		else if (clientMap.at(i).getContent().substr(0, 30).find(".css") != std::string::npos)
		{
			htmlFile = readFile("./stuff/styles.css");
			fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: text/css\r\n\r\n" + htmlFile;
		}
		else if (clientMap.at(i).getContent().substr(0, 30).find(".ico") != std::string::npos)
		{
			htmlFile = readFile("./stuff/favicon.ico");
			fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: image/x-icon\r\n\r\n" + htmlFile;
		}
		long long len = send(i, fileContent.c_str(), fileContent.length(), 0);
		if (len < 0)
			std::cout << "error " << std::endl;
		// std::cout << "send \n";
		shutdown(i, SHUT_WR);
		FD_CLR(i, &write_set);

		//if you close the next client willtake fd 6 maybe dont close now
		close(i);

		// for (std::list<int>::const_iterator it = newClientSocket; it != newClientSocket; ++it)
		// {
		// 	std::cout << "newclient " << *it << " i " << i << std::endl;
		// 	if (*it == i)
		// 		newClientSockett);
		// for (std::list<int>::const_iterator it2 = newClientSocket; it2 != newClientSocket; ++it2)
		// 		std::cout << "newc " << *it2 << std::endl;
		// }
		// ToDo: update max fd socket
		// if (i == maxSocket)
		// 	--maxSocket;
	// }
}

webserv::webserv(std::vector<webInfo> &serverList)
{
	timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	// create server socket in a map
	for (std::vector<webInfo>::iterator it = serverList.begin(); it != serverList.end(); ++it)
		serverMap.insert(std::make_pair(it->getSock(), *it));
	
	
	//set to non blocking
	setNoBlocking();
	// to close the socket after program ends
	int nbr = 1;
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
		setsockopt(mapIt->first, SOL_SOCKET, SO_REUSEADDR, &nbr, sizeof(nbr));

	//print
	std::cout << "serversokt are : ";
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
		 std::cout << "[" << mapIt->first << "] ";
	std::cout << std::endl;

	// create server address
	creatAddresses();
	// bind socket to an address port
	bindSockets();
	// listen
	listening();
	// set servermap to read
	setFds();


	while(1)
	{
		// std::cout << "\nstart \n----------------\n";

		copyRead = read_set;
		copyWrite = write_set;

		// select
		int check = select(maxSocket + 1, &copyRead, &copyWrite, NULL, NULL);
		if (check < 0)
			return;
		// std::cout << "after select " << std::endl;
		
		//accept all sockets

		// accept connection
		for (int i = 3; i <= maxSocket; ++i)
		{
		// sleep(1);
			if (serverMap.count(i)) // if its server socket we have to accept it not read it
				acceptSockets(i);
			else if (FD_ISSET(i, &copyRead))
				reading(i);
			else if (FD_ISSET(i, &copyWrite)) // receiv request from clinet
				writing(i);
		}
	}
	close(serverMap.begin()->first);
}

webserv::~webserv()
{
}
	// stop 
	// {
	// 	receve
	// }
	// join
	// checki
	// rnrn 
	// {
	// 	stop = true
	// 	check body size m3a cont size
	// 	receve
	// }


	//create vector of characters from buffer 
	// creat a file and put the buffer in it append the rest
	// or put the buffer in  a stream then put it back in a std string
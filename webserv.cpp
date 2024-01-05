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

int parse_the_request(const std::string &buff)
{
	
	if (buff.find("GET", 0) != std::string::npos)
	{
		std::cout << "\nthe request is : \n" << buff << std::endl;
		return (1);
	}
	else
		std::cout << "no get" << std::endl;
	return (0);
	// buff.substr(4, buff.find('\n', 0))
	// std::cout << buff.find('\n', 0) - buff.find(' ', 5) << std::endl;

}

void	webserv::creatAddresses()
{
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
		serverAddress.push_back(mapIt->second.getServerAddress());
}

void	webserv::bindSockets()
{
	std::list<struct sockaddr_in>::iterator adrIt = serverAddress.begin();
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	{
		std::cout << "first :" << mapIt->first << std::endl;
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
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
		FD_SET(mapIt->first, &read_set);
	maxSocket = serverMap.begin()->first;
}

void	webserv::acceptSockets()
{
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	{
		if (FD_ISSET(mapIt->first, &copyRead))
		{
			std::cout <<"serversoket set to read"<< std::endl;

			// accept new connection
			clientAddressLen = sizeof(*clientAddress.begin());
			newClientSocket.push_back(accept(mapIt->first, (struct sockaddr *)&clientAddress, &clientAddressLen));
			if (newClientSocket.back() < 0)
			{
				std::cout << "failed to accept" << std::endl;
				return;
			}

			//nonblocking fds
			int flags = fcntl(newClientSocket.back(), F_GETFL, 0);
			fcntl(newClientSocket.back(), F_SETFL, flags | O_NONBLOCK);

			//set newclient to copyRead
			FD_SET(newClientSocket.back(), &copyRead);
			if (newClientSocket.back() > maxSocket)
				maxSocket = newClientSocket.back();
		}
	}
}

webserv::webserv(std::list<webInfo> &serverList)
{

	// create server socket
	for (std::list<webInfo>::iterator it = serverList.begin(); it != serverList.end(); ++it)
		serverMap.insert(std::make_pair(it->getSock(), *it));
	

	if (serverMap.begin()->first == -1)
	{
		std::cout << "failed to create server socket" << std::endl;
		return;
	}
	
	int flags = fcntl(serverMap.begin()->first, F_GETFL, 0);
	fcntl(serverMap.begin()->first, F_SETFL, flags | O_NONBLOCK);

	//close the socket after program ends
	int nbr = 1;
	setsockopt(serverMap.begin()->first, SOL_SOCKET, SO_REUSEADDR, &nbr, sizeof(nbr));

	std::cout << "sokt is : " << serverMap.begin()->first << std::endl;

	// create server address
	creatAddresses();

	// bind socket to an address port
	bindSockets();
	
	// listen
	listening();

	setFds();
	while(1)
	{
		std::cout << "start \n";

		copyRead = read_set;
		copyWrite = write_set;

		// select
		int check = select(maxSocket + 1, &copyRead, &copyWrite, NULL, NULL);
		if (check < 0)
			return;
		std::cout << check << " after select "<< std::endl;
		
		//accept all sockets
		acceptSockets();

		// accept connection
		for (int i = 3; i <= maxSocket; ++i)
		{
			std::cout << "i = " << i << std::endl;
			// sleep(1);
			if (FD_ISSET(i, &copyRead))
			{
				//recv(i, buff, sizeof(buff), 0);
				int bytesReaded = read(i, buff, sizeof(buff));
				if (bytesReaded < 0)
				{
					std::cout << "failed to recv" << std::endl;
					FD_CLR(i, &read_set);
					FD_CLR(i, &write_set);
					continue;
				}
				buff[bytesReaded] = '\0';
				std::cout << "readed -> " << buff << std::endl;
				FD_SET(i, &write_set);
			}
			else if (FD_ISSET(i, &copyWrite))// receiv request from clinet
			{
				// FD_SET(i, write_set);
				std::cout << "the request size : -> \n" << " " << sizeof(buff) << '\n';
				std::string tmp = static_cast<std::string>(buff);
				if (parse_the_request(tmp))
				{
					std::string htmlFile = readFile("favicon.ico");
					std::string fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: image/x-icon\r\n\r\n" + htmlFile;
					send(i, fileContent.c_str(), fileContent.length(), 0);

					std::cout << "send \n";
					FD_CLR(i, &write_set);
					// ToDo: update max fd socket
				}
			}
			// fill the clientsockets with the new accepted clientsocket
		}
		
	}
	close(serverMap.begin()->first);
}

webserv::~webserv()
{
}

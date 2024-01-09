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
	if (reqContent.find("GET", 0) != std::string::npos)
	{
		std::cout << "\n############### the request is : \n" << reqContent << std::endl;
		return (1);
	}
	else
		std::cout << "no get" << std::endl;
	return (0);
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
	std::list<struct sockaddr_in>::iterator adrIt = serverAddress.begin();
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
		std::cout << "accept " << i << std::endl;
		if (FD_ISSET(i, &copyRead))
		{

			// accept new connection
			clientAddressLen = sizeof(*clientAddress.begin());
			newClientSocket = accept(i, (struct sockaddr *)&clientAddress, &clientAddressLen);
			if (newClientSocket < 0)
			{
				std::cout << "failed to accept" << std::endl;
				// return true;
			}
			std::cout <<"new client accepted [" << newClientSocket << "] " <<  std::endl;
			//nonblocking fds
			int flags = fcntl(newClientSocket, F_GETFL, 0);
			fcntl(newClientSocket, F_SETFL, flags | O_NONBLOCK);

			//set newclient to copyRead
			FD_SET(newClientSocket, &read_set);
			std::cout <<"new client set to READ " <<  std::endl;

			if (newClientSocket > maxSocket)
				maxSocket = newClientSocket;

			clientMap.insert(std::make_pair(newClientSocket, cliento));

			std::cout << "acceptSockets " << maxSocket << std::endl;
			// return false;
		}
	// }
	// return false;
}

void	webserv::reading(int i)
{
	std::cout << "COPY_READ block " << std::endl;
	int bytesReaded = recv(i, buff, 100, 0);
	if (bytesReaded < 0)
	{
		std::cout << "failed to recv" << std::endl;
		FD_CLR(i, &read_set);
		FD_CLR(i, &write_set);
		// continue;
	}
	if (bytesReaded == 0)
		std::cout << "the connectuion is done " << std::endl;
	else
		std::cout << "\nlen recv is < " << bytesReaded << " >\n" << std::endl;
	buff[bytesReaded] = '\0';
	std::string bufTmp;
	bufTmp = static_cast<std::string>(buff);
	reqContent.append(bufTmp);
	
	std::cout << "[[[ \n\n" << reqContent << " \n]]]" << std::endl;

	if (reqContent.find("\r\n\r\n", 0) != std::string::npos)
	{
		std::cout << "\n@@@@@@@@@@ end of req\n" << i << ' ' << reqContent.length() << '\n' << std::endl;
		try
		{
			clientMap.at(i).setContent(reqContent);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << '\n';
		}
		reqContent.clear();
	}
	else
	{
		std::cout << "\n&&&&&&&&&&& keep recv not done yet\n" << std::endl;
		// sleep(2);
		return ;
	}
// sleep(2);
			std::cout << "reading " << maxSocket << std::endl;

	std::cout << "\n//--- begin req ---//\n" << std::endl;
	std::cout << clientMap.find(i)->second.getContent() << std::endl;
	std::cout << "//--- end req ---//\n\n" << "the request size : -> " << clientMap.find(i)->second.getContent().length() << '\n';

	//set to write on the client socket
	FD_SET(i, &write_set);
	//clear it from read cuz now it need to be writing on it only
	FD_CLR(i, &read_set);
}

void	webserv::writing(int i)
{
	std::cout << "COPY_WRITE block " << std::endl;
	// std::cout << "here\n[ " << serverMap.find(6)->second.getContent() << "]" << std::endl;
	// std::string tmp = static_cast<std::string>(buff);
	// if (parse_the_request(i))
	// {
		std::cout << "\n############### the request is : \n" << clientMap.find(i)->second.getContent() << std::endl;
		// std::string htmlFile = readFile("favicon.ico");
		// std::string fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: image/x-icon\r\n\r\n" + htmlFile;
		std::string htmlFile = readFile("index.html");
		std::string fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: text/html\r\n\r\n" + htmlFile;
		long long len = send(i, fileContent.c_str(), fileContent.length(), 0);
		if (len < 0)
			std::cout << "error " << std::endl;
		std::cout << "send \n";
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

webserv::webserv(std::list<webInfo> &serverList)
{
	timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	// create server socket in a map
	for (std::list<webInfo>::iterator it = serverList.begin(); it != serverList.end(); ++it)
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
		std::cout << "\nstart \n----------------\n";

		copyRead = read_set;
		copyWrite = write_set;

		// select
		int check = select(maxSocket + 1, &copyRead, &copyWrite, NULL, NULL);
		if (check < 0)
			return;
		std::cout << "after select " << std::endl;
		
		//accept all sockets

		// accept connection
		for (int i = 3; i <= maxSocket; ++i)
		{
		// sleep(1);
			std::cout << "socket = " << i << std::endl;
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

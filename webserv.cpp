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

webserv::webserv()
{

	// create server socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cout << "failed to create server socket" << std::endl;
		return;
	}
	
	int flags = fcntl(serverSocket, F_GETFL, 0);
	fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

	//close the socket after program ends
	int nbr = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &nbr, sizeof(nbr));

	std::cout << "sokt is : " << serverSocket << std::endl;
	// create server address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// bind socket to an address port
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cout << "failed to bind server socket" << std::endl;
		return;
	}

	// listen
	if (listen(serverSocket, 5) < 0)
	{
		std::cout << "failed to listen" << std::endl;
		return;
	}

	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	FD_SET(serverSocket, &read_set);
	maxSocket = serverSocket;

	fd_set copyRead;
	fd_set copyWrite;
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
		if (FD_ISSET(serverSocket, &copyRead))
		{
			std::cout <<"serversoket set to read"<< std::endl;

			// accept new connection
			clientAddressLen = sizeof(clientAddress);
			newClientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
			if (newClientSocket < 0)
			{
				std::cout << "failed to accept" << std::endl;
				return;
			}

			//nonblocking fds
			int flags = fcntl(newClientSocket, F_GETFL, 0);
			fcntl(newClientSocket, F_SETFL, flags | O_NONBLOCK);

			//set newclient to copyRead
			FD_SET(newClientSocket, &copyRead);
			if (newClientSocket > maxSocket)
				maxSocket = newClientSocket;
		}

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
	close(serverSocket);
}

webserv::~webserv()
{
}

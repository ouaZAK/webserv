#include "webserv.hpp"

std::string	readFile(const std::string &str)
{
	std::ifstream file(str);
	if (!file.is_open())
	{
		std::cout << "failed to pen file" << std::endl;
		return "";
	}

	std::stringstream inStr;
	inStr << file.rdbuf();
	return (inStr.str());
}

webserv::webserv() : serverSocket(-1)
{
	
	//create server socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cout << "failed to create server socket" << std::endl;
		return ;
	}
	int r = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &r, sizeof(r));
	
	//create server address
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	//bind socket to an address port
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		std::cout << "failed to bind server socket" << std::endl;
		return ;
	}

	//listen
	if (listen(serverSocket, 5) < 0)
	{
		std::cout << "failed to listen" << std::endl;
		return ;
	}

		FD_ZERO(&read_set);
		FD_SET(serverSocket, &read_set);
		maxSocket = serverSocket;

		fd_set copyRead;
	while (1)
	{
		std::cout << "start \n";

		copyRead = read_set;

		//select
		int check = select(maxSocket + 1, &copyRead, NULL, NULL, NULL);
		if (check < 0)
			return ;
std::cout << check <<  " after select \n";

		//accept connection
		for (int i = 0; i <= maxSocket; ++i)
		{
				std::cout << i << "\n";
				// sleep(2);
			if (FD_ISSET(i, &read_set))
			{
				if (i == serverSocket)
				{
					clientAddressLen = sizeof(clientAddress);
					 // accept new connection
					newClientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
					if (newClientSocket < 0)
					{
		std::cout << "failed to accept" << std::endl;
						return ;
					}
			int flags = fcntl(newClientSocket, F_GETFL, 0);
			fcntl(newClientSocket, F_SETFL, flags | O_NONBLOCK);
					FD_SET(newClientSocket, &read_set);
					if (newClientSocket > maxSocket)
						maxSocket = newClientSocket;
				}
				else //receiv request from clinet
				{
					size_t bytesReaded = recv(i, buff, sizeof(buff), 0);
					if (bytesReaded <= 0)
					{
						std::cout << "failed to recv" << std::endl;
						close(i);
					}

	std::cout << "fd socket is set to reaad\n";
					std::string htmlFile = readFile("index.html");
					std::string fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: text/html\r\n\r\n" + htmlFile;
					send(i, fileContent.c_str(), fileContent.length(), 0);
	std::cout << "send \n";
					close(i);
					FD_CLR(i, &read_set);
					// ToDo: update max fd socket
				}

				//fill the clientsockets with the new accepted clientsocket
			}
		}
std::cout << "after accept \n";

		
	}
	close(serverSocket);
}

webserv::~webserv()
{

}


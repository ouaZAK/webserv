#include "webserv.hpp"

int	creatSock()
{
	int serversock = socket(AF_INET, SOCK_STREAM, 0);
	if (serversock == -1)
	{
		std::cout << "failed to create server socket" << std::endl;
		exit(1);
	}
	return (serversock);
}

int main()
{
	std::list<webInfo> listWebInfo;
	int port = 8080;
	for (int i = 0; i < 3; i++)
	{
		webInfo info(port++);
		listWebInfo.push_back(info);
	}
	std::cout << "size is " << listWebInfo.size() << std::endl;

	// std::map<int, webInfo> server;
	// for (std::list<webInfo>::iterator it = listWebInfo.begin(); it != listWebInfo.end(); ++it)
	// {
	// 	server.insert(std::make_pair(creatSock(), *it));
	// }
	// std::cout << server.begin()->first << std::endl;

	webserv start(listWebInfo);
	// webserv start;
	return (0);
}

#include "webserv.hpp"
#include "ServerInf.hpp"

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

int main(int ac, char **av)
{
	if (ac != 2)
		return (std::cout << "Error" << std::endl, 1);
	std::vector<ServerInf> serverInfAymaaaaaaan = confInf(av);
	std::map<std::string, std::string> mimeMap = populateMimeMap();
	std::vector<webInfo> listWebInfo;
	std::vector<int> ports;
	try
	{
		for (std::vector<ServerInf>::iterator it = serverInfAymaaaaaaan.begin(); it != serverInfAymaaaaaaan.end(); ++it)
		{
			ports = it->getPorts();
			for (std::vector<int>::iterator itV = ports.begin(); itV != ports.end(); itV++)
			{
				webInfo info(*itV, *it);
				listWebInfo.push_back(info);
			}
		}
		// webserv start;
		webserv start(listWebInfo);
	}
	catch(const std::exception& e)
	{
		std::cerr << "$$$$$$$------------------$$$$$$$$$\n" << e.what() << '\n';
	}
	return (0);
}

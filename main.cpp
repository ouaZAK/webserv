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
		for (std::vector<ServerInf>::iterator infIt = serverInfAymaaaaaaan.begin(); infIt != serverInfAymaaaaaaan.end(); ++infIt)
		{
			ports = infIt->getPorts();
			for (std::vector<int>::iterator itV = ports.begin(); itV != ports.end(); itV++)
			{
				webInfo info(*itV, *infIt);
				listWebInfo.push_back(info);
			}
		}
		// webserv start;
		// std::cout << listWebInfo[0].getRoot() << '\n';
		// std::cout << listWebInfo[1].getRoot() << '\n';
		// std::cout << listWebInfo[2].getRoot() << '\n';
		webserv start(listWebInfo);
	}
	catch(const std::exception& e)
	{
		std::cerr << "$$$$$$$------------------$$$$$$$$$\n" << e.what() << '\n';
	}
	return (0);
}

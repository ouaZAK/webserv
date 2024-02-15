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
	std::map<std::string, std::string> mimeMap;
	std::vector<webInfo> listWebInfo;
	try
	{
		std::vector<ServerInf> serverInfAymaaaaaaan = confInf(av);
		mimeMap = populateMimeMap();
		std::vector<int> ports;
		for (std::vector<ServerInf>::iterator infIt = serverInfAymaaaaaaan.begin(); infIt != serverInfAymaaaaaaan.end(); ++infIt)
		{
			ports = infIt->getPorts();
			for (std::vector<int>::iterator itV = ports.begin(); itV != ports.end(); itV++)
			{
				webInfo info(*itV, *infIt);
				listWebInfo.push_back(info);
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "$$$$$$$------------------$$$$$$$$$\n" << e.what() << '\n';
		return (1);
	}
	catch(const char* e)
	{
		std::cerr << e << '\n';
		return (1);
	}
	webserv start(listWebInfo, mimeMap);
	return (0);
}

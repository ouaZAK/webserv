#include "webserv.hpp"
#include "ServerInf.hpp"

int main(int ac, char **av)
{
	(void)ac;
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
		std::cerr << "$$$$$$$------- Error -----------$$$$$$$$\n" << e.what() << '\n';
		return (1);
	}
	catch(const char* e)
	{
		std::cerr << e << '\n';
		return (1);
	}
	try
	{
		webserv start(listWebInfo, mimeMap);
	}
	catch(const std::exception& e)
	{
		std::cerr << "$$$$$$$------- Error -----------$$$$$$$$\n" << e.what() << '\n';
		return (1);
	}
	catch(const char* e)
	{
		std::cerr << e << '\n';
		return (1);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getServerInf.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 19:46:31 by asidqi            #+#    #+#             */
/*   Updated: 2024/01/25 15:47:36 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerInf.hpp"

std::vector<ServerInf> confInf(char *av[])
{
	std::vector<ServerInf> sInf;
	ServerInf tmp;
	std::ifstream inFile(av[1]);
	std::string el;

	if (inFile.fail())
		throw "File failed to open";
	std::string line;
	try
	{

		while (std::getline(inFile, line))
		{
			std::stringstream ss(line);
			ss >> el;
			if (el == "server")
			{
				el.clear();
				ss >> el;
				std::getline(inFile, line);
				if (line == "{")
					line = tmp.filltmp(inFile);
				if (ServerInf::isbrac(line, '}'))
				{
					sInf.push_back(tmp);
					tmp.reset();
					continue;
				}
				else if (!el.empty())
				{
					tmp.reset();
					throw "Unknown element";
					break;
				}
			}
		}
		inFile.close();
	}
	catch (char const *e)
	{
		std::cout << e << '\n';
	}
	std::cout << "====================== " << std::endl;
	// for (std::vector<ServerInf>::iterator it = sInf.begin(); it != sInf.end(); ++it)
	// {
		// (*it).print();
		// std::cout << "intcrement: "<<(*it).locs.size()<<"\n";
		// std::cout << "size of serverinf: "<< sInf.size() <<"\n";
	// }
	// std::cout << "====================== " << std::endl;
	return (sInf);
}

std::map<std::string, std::string> populateMimeMap()
{
	std::map<std::string, std::string> mimeMap;
	std::ifstream sis("confPars/mime.types"); //./confPars/mime.types
	if (!sis)
	{
		std::cerr << "Failed to open file." << std::endl;
		return mimeMap; 
	}
	std::string line;
	while (std::getline(sis, line))
	{ 
		std::stringstream iss(line);
		std::string key, value;
		iss >> value >> key;
		if (!key.empty() && !value.empty())
			mimeMap.insert(std::pair<std::string, std::string>(key, value));
	}
	return mimeMap;
}
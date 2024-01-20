/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asidqi <asidqi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 19:46:31 by asidqi            #+#    #+#             */
/*   Updated: 2024/01/20 17:35:02 by asidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerInf.hpp"



int main(int ac, char *av[])
{
	if (ac != 2)
		return (1);
	std::vector<ServerInf> sInf;
	ServerInf tmp;
	std::ifstream inFile(av[1]);
	std::string el;

	if (inFile.fail())
		return (1);
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
	// std::cout << "====================== " << std::endl;
	// for (std::vector<ServerInf>::iterator it = sInf.begin(); it != sInf.end(); ++it)
	// {
	// 	(*it).print();
	// 	std::cout << "intcrement: "<<(*it).locs.size()<<"\n";
	// 	std::cout << "size of serverinf: "<< sInf.size() <<"\n";
	// }
	// std::cout << "====================== " << std::endl;
	return (0);
}
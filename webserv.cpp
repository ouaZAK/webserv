/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:04:51 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/01/31 18:35:08 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string readFile(const std::string &str)
{
	// std::cout << "file to open : " << str << '\n';
	std::ifstream file(str, std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "failed to pen file" << std::endl;
		return ("");
	}
	std::stringstream inStr;
	inStr << file.rdbuf();
	return (inStr.str());
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
	std::vector<struct sockaddr_in>::iterator adrIt = serverAddress.begin();
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
	//if sock server is in read which is always in read whadaheck
	if (FD_ISSET(i, &copyRead))
	{
	// std::cout << "server " << i  << " accepted"<< std::endl;

		// accept new connection
		clientAddressLen = sizeof(*clientAddress.begin());
		newClientSocket = accept(i, (struct sockaddr *)&clientAddress, &clientAddressLen);
		if (newClientSocket < 0)
			std::cout << "failed to accept" << std::endl;
	
		//nonblocking fds
		int flags = fcntl(newClientSocket, F_GETFL, 0);
		fcntl(newClientSocket, F_SETFL, flags | O_NONBLOCK);

		//set newclient to copyRead
		FD_SET(newClientSocket, &read_set);

		if (newClientSocket > maxSocket)
			maxSocket = newClientSocket;
		
		
		clientMap.insert(std::make_pair(newClientSocket, clientInf));
		clientMap[newClientSocket].setRoot(serverMap[i].getRoot());
		clientMap[newClientSocket].setLoc(serverMap[i].getLoc());
		
	// std::cout << "socket is " << i  << " client sock is " << newClientSocket << '\n';
	}
}

void	webserv::extractBody(int i)
{
	try
	{
		std::string tmpBody;
		size_t posNwl;

		pos = clientMap[i].getReqChunk().find("\r\n\r\n", 0);
		tmpBody = clientMap[i].getReqChunk();
		body = tmpBody.substr(pos + 4, tmpBody.length() - (pos + 4));
		posNwl = body.find("\r\n", 0);// to know length of first line
		pos = body.find("\r\n\r\n", 0);
		cleanBody = body.substr(pos + 4, body.length() - (pos + 4) - (posNwl + 4));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << "$$$$$$$$$$$$$$$$$________________$$$$$$$$$$$$$$$$$" << '\n';
	}
}

bool	webserv::getRequest(int i)
{
	Request req = clientMap[i].getReq();
	if (req.get_method() == "POST")
	{
		extractBody(i);
		if (body.length() < bodyLength)
			return (true);
		req.set_body(body);
		std::string filename = req.get_file_name();
		
		/***** get the proper root if its default one or inside location ******/
		std::ofstream file(clientMap[i].getRoot() + "/" + filename);		/* get the root path */
		file << cleanBody;
	}
	clientMap[i].setReqFull(clientMap.at(i).getReqChunk());
	clientMap.at(i).reqChunckClear();

	//print
	// std::cout << "\n@@@@@@@@@@ end of req\ni : " << i << " \n" << clientMap.at(i).getReqFull() << '\n'
	// 		<< body.length() << " " << bodyLength << std::endl;
	return (false);
}

void	webserv::reading(int i)
{
	// std::cout << "READ block " << std::endl;
	// 	std::cout << "i : " << i << std::endl;

	int bytesReaded = recv(i, buff, 3000, 0);
	if (bytesReaded < 0)
		// std::cout << "failed to recv" << std::endl;
	if (bytesReaded == 0)
		// std::cout << "the connectuion is done " << std::endl;
	// else
	// 	std::cout << "\nlen recv is < " << bytesReaded << " >\n" << std::endl;
	buff[bytesReaded] = '\0';
	std::string bufTmp;
	std::string tmp = clientMap[i].getReqChunk();
	bufTmp = std::string(buff, bytesReaded);
	tmp.append(bufTmp);
	clientMap.at(i).setReqChunk(tmp);

	//print
	// std::cout << "[[[ \n\n" << clientMap.at(i).getReqChunk() << " \n]]]" << std::endl;
	// std::cout  << " " << clientMap.at(i).getReqChunk().find("\r\n\r\n", 0) << std::endl;
	// std::cout << "\n\n[[[--------\n\n" << cleanBody << "\n\n------]]]\nbodylength: " 
	// 			 << body.length() << " " << bodyLength << std::endl;

	if (clientMap[i].getReqChunk().find("\r\n\r\n", 0) != std::string::npos)
	{
		Request req(clientMap[i].getReqChunk());
		clientMap[i].setReq(req);
		bodyLength = std::strtod(req.get_headers()["Content-Length"].c_str(), NULL);
		if (getRequest(i))
			return ;
	}
	else
		return ;

	//print
	// std::cout << "\nreadin to writin" << std::endl;
	// std::cout << "\n//--- begin req ---//\n" << std::endl;
	// std::cout << clientMap.find(i)->second.getReqFull() << std::endl;
	// std::cout << "//--- end req ---//\n\n" << "the request size : -> " << clientMap.find(i)->second.getReqFull().length() << '\n';

	//set to write on the client socket
	FD_SET(i, &write_set);
	//clear it from read cuz now it need to be writin on it only
	FD_CLR(i, &read_set);
}

std::string	webserv::serveFile(int i)
{
	std::string htmlFile;
	std::string fileContent;
	
	//if its dir show default file or show index
	if (is_dir)
	{
		//if (clientMap[i].serverInf.getIndex()) check if index is on on the configue file
		htmlFile = readFile("stuff/index.html");
	}
	else
		htmlFile = readFile("stuff/default.html");
	fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: text/html\r\n\r\n" + htmlFile;
	
	int x = access(urlPath.c_str(), F_OK);
	if (x == -1)
		return (fileContent);//make it 404 after

	for (std::map<std::string, std::string>::iterator typeIt = mimeMap.begin(); typeIt != mimeMap.end(); ++typeIt)
	{
		if (clientMap.at(i).getReqFull().substr(0, 30).find("." + typeIt->first) != std::string::npos)
		{
			htmlFile = readFile(urlPath);
			fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: " + typeIt->second + "\r\n\r\n" + htmlFile;
			return (fileContent);
		}
	}
	//return default page
	return (fileContent);
}

bool	countSlash(std::string dir)
{
	int count = 0;
		// std::cout << " \n dir is [ " << dir << " ]" << '\n';
	for (std::string::iterator it = dir.begin(); it != dir.end(); ++it)
	{
		if (*it == '/')
			++count;
		if (count >= 2)
			return (true);
	}
	return (false);
}

void	webserv::checkLocMeth(int i)
{
	/* ######### test 8080/dir/lala.html in ngnix ########### */
	std::string dir = clientMap[i].getReq().get_path();
	if (!countSlash(dir))
			dir = dir.append("/");// taygadha ha ki me
	
	if (countSlash(dir))
	{
		std::cout << "\n ---------- \n there is 2 slash dir is [ " << dir << " ]" << '\n';
		size_t pos1 = urlPath.find("/", 0);
		size_t pos2 = urlPath.find("/", pos1 + 1);
		dir = urlPath.substr(pos1, pos2 - pos1 + 1);
		std::cout << "last dir {" <<  dir << "}" << '\n';
		std::vector<Location> loc = clientMap[i].getLoc();
		for (std::vector<Location>::iterator locIt = loc.begin(); locIt != loc.end(); ++locIt)
		{
			std::cout << "\n inside \n";
			for (std::vector<std::string>::iterator itV = locIt->path.begin(); itV != locIt->path.end(); itV++)
			{
				std::cout << "location: <<< " << *itV << " >>>" << dir;
				if (dir == *itV)
				{
					for (std::vector<std::string>::iterator itV2 = locIt->methods.begin(); itV2 != locIt->methods.end(); itV2++)
					{
						std::cout << clientMap[i].getReq().get_method() << " <- req itV -> " <<  *itV2 << " " << itV2->size() << " | \n";
						if (clientMap[i].getReq().get_method() == *itV2)
						{
							std::cout << "good return \n";
							return ;
						}
					}
					std::cout << "not allowed return \n\n";
					return;
				}
			}
		}
	}
	// if ()
	std::cout << "\nnot a dir\n";
}

void	webserv::writing(int i)
{
	// std::cout << "WRITE block " << std::endl;
	// std::cout << "here\n[ " << serverMap.find(6)->second.getReqFull() << "]" << std::endl;

	// join url with path
	Request reqHakimeeee(clientMap.at(i).getReqFull());
	urlPath = clientMap[i].getRoot() + reqHakimeeee.get_path();
	std::cout << urlPath << '\n';
	//check is dir
	is_dir = false;
	struct stat fileStat;
	if (stat(urlPath.c_str(), &fileStat) == 0)
	{
		is_dir = true;
	}

	//check method if its allowed
	checkLocMeth(i);

	// std::cout << "i : " << i << std::endl;
	// std::cout << "\n############### the request is : \n" << clientMap.find(i)->second.getReqFull() << std::endl;
	
	std::string fileContent = serveFile(i);	
	long long len = send(i, fileContent.c_str(), fileContent.length(), 0);
	if (len < 0)
		std::cout << "error " << std::endl;
	// std::cout << "send \n";
	shutdown(i, SHUT_WR);
	FD_CLR(i, &write_set);

	//if you close the next client willtake fd 6 maybe dont close now
	close(i);
}

webserv::webserv(std::vector<webInfo> &serverList, std::map<std::string, std::string> mime)
{
	timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	mimeMap = mime;
	
	// create server socket in a map
	for (std::vector<webInfo>::iterator it = serverList.begin(); it != serverList.end(); ++it)
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
		// std::cout << "\nstart \n----------------\n";

		copyRead = read_set;
		copyWrite = write_set;

		// select
		int check = select(maxSocket + 1, &copyRead, &copyWrite, NULL, NULL);
		if (check < 0)
			return;

		// accept connection
		for (int i = 3; i <= maxSocket; ++i)
		{
			if (serverMap.count(i)) // if its server socket we have to accept it not read it
				acceptSockets(i);
			else if (FD_ISSET(i, &copyRead))
			{
				// ############## should do now #############
				//request has to be in client map i guess

				reading(i);
			}
			else if (FD_ISSET(i, &copyWrite)) // receiv request from clinet
				writing(i);
		}
	}
	close(serverMap.begin()->first);
}

webserv::~webserv()
{
}
std::map<int, webInfo>  webserv::getmap() const
{
	return (serverMap);
}
	//create vector of characters from buffer 
	// creat a file and put the buffer in it append the rest
	// or put the buffer in  a stream then put it back in a std string
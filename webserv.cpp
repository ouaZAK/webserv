/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:04:51 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/02/17 20:40:22 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// TO DO : 
// " wasir gad auto index bdak html " go to there variable it3taw
// remove client when its close
// /dir/ is a fking url not directory
// loop 3la client o chof variable dial time 
// check cgi path if it doesnt have cgi dont fork for it
// if system calls fails with only one socket exit
// dont exit with error if 2 servers have the same port and dirferrent hosts
// 

#define BL1 "\x1b[34m"
#define BL2 "\x1b[0m"
#define GR1 "\x1b[32m"
#define GR2 "\x1b[0m"
#define OR1 "\x1b[38;5;208m"
#define OR2 "\x1b[0m"
#define YL1 "\x1b[33m"
#define YL2 "\x1b[0m"

template<typename T>
void	print(T str, std::string s)
{
	std::cout << BL1 << s << BL2 << '\n';
	std::cout << GR1 << "[" << str << "]" << GR2 << '\n';
}

std::string readFile(const std::string &str)
{
	// std::cout << "file to open : " << str << '\n';
	std::ifstream file(str, std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "failed to open file" << std::endl;
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
		std::cout << OR1 << mapIt->first << OR2 << '\n';
		if (bind(mapIt->first, (struct sockaddr *)&(*adrIt), sizeof(*adrIt)) < 0)
		{
			if (serverMap.size() == 1)
				throw "Error:\nFatal error";
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
	std::cout << YL1 << "server " << i  << " accepted" << YL2 << std::endl;

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
		clientMap[newClientSocket].setBodySize(serverMap[i].getBodySize());
		clientMap[newClientSocket].setAutoIndx(serverMap[i].getAI());
		clientMap[newClientSocket].setErrorPages(serverMap[i].getErrorPages());
		clientMap[newClientSocket].setPort(serverMap[i].getPort());
		clientMap[newClientSocket].setHost(serverMap[i].getHost());

	// std::cout << "socket is " << i  << " client sock is " << newClientSocket << '\n';
	}
}

void	webserv::extractBody(int i)
{
	try
	{
		std::string	tmpBody;
		size_t 		posNwl;

		tmpBody = clientMap[i].getReqChunk();
		pos = tmpBody.find("\r\n\r\n", 0);
		body = tmpBody.substr(pos + 4, tmpBody.length() - (pos + 4));
		posNwl = body.find("\r\n", 0); // to know length of first line
		pos = body.find("\r\n\r\n", 0);
		cleanBody = body.substr(pos + 4, body.length() - (pos + 4) - (posNwl + 7));/* 4 for "-" and 3 for '\n' */
		// print(cleanBody, "cleanBody: " );
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << "$$$$$$$$$$$$$$$$$________________$$$$$$$$$$$$$$$$$" << '\n';
	}
}

void	webserv::setResStatus(int i, int status, std::string &htmlFile, std::string statusHtml)
{
	int x, y;
	if (!htmlFile.empty())
	{
		for (std::vector<std::string>::iterator it = clientMap[i].getErrorPages().begin(); it != clientMap[i].getErrorPages().end(); ++it)
		{
			if (statusHtml == *it)
			{
				x = access(("dirOfErrors/" + statusHtml).c_str(), F_OK);
				y = access(("dirOfErrors/" + statusHtml).c_str(), R_OK);
				std::cout << "x : " << x << "y: " << y << '\n';
				if (x != -1 && y != -1)
				{
					htmlFile = readFile("dirOfErrors/" + statusHtml);
					resError = false;
					break ;
				}
			}
		}
	}
	clientMap[i].getReq().set_status(status);
	clientMap[i].setRes(clientMap[i].getReq());
}

bool webserv::check_dir(int i, std::string dir)
{
	/* location vector is loc */
	std::vector<Location> loc = clientMap[i].getLoc();
	for (std::vector<Location>::iterator locIt = loc.begin(); locIt != loc.end(); ++locIt)
	{
		for (std::vector<std::string>::iterator itV = locIt->path.begin(); itV != locIt->path.end(); itV++)
		{
			std::cout << "location: [" << *itV << "]    dir: [" << dir << "]\n";
			if (dir == *itV)
			{
				std::cout << "ROOOOOOOOOOOOOOOOOOOOOOT : " << locIt->root << '\n';
				if (locIt->redirect_status == 301)
				{
					std::cout << "########### : " << locIt->redirect_to_dir << " " << locIt->redirect_status << '\n';
					clientMap[i].getReq().set_status(301);
					Response res(clientMap[i].getReq(), locIt->redirect_to_dir); // then send req to res
					clientMap[i].setRes(res);
					return (true);
				}
				for (std::vector<std::string>::iterator itV2 = locIt->methods.begin(); itV2 != locIt->methods.end(); itV2++)
				{
					std::cout << clientMap[i].getReq().get_method() << " <- req  |  itV -> " <<  *itV2 << "\n";
					if (clientMap[i].getReq().get_method() == *itV2)
					{
						clientMap[i].setDefFile(locIt->default_file); /* get the default file in every directory */
						std::cout << GR1 << "good return from directory" << GR2 << "\n";
						return (false);
					}
				}
				htmlFile = "dirOfErrors/405.html";
				setResStatus(i, 405, htmlFile, "405.html");
				std::cout << "not allowed return from directory\n\n";
				return (true);
			}
		}
	}
	return (true);
}

bool	webserv::getRequest(int i)
{
	Request req = clientMap[i].getReq();
	std::string dir = clientMap[i].getReq().get_path();
	if (check_dir(i, dir))
	{
		clientMap[i].setReqFull(clientMap.at(i).getReqChunk());
		clientMap.at(i).clearReqChunk();
		return false;
	}
	if (req.get_method() == "POST")
	{
		// std::string dir = clientMap[i].getReq().get_path();
		// if (check_dir(i, dir))
		// {
		// 	clientMap[i].setReqFull(clientMap.at(i).getReqChunk());
		// 	clientMap.at(i).clearReqChunk();
		// 	return false;
		// }
		
		extractBody(i);
		if (body.length() < bodyLength)
			return (true);
		if (cleanBody.size() > clientMap[i].getBodySize() && clientMap[i].getReq().get_status() == 200/* and 200 range */)
		{
			clientMap[i].getReq().set_status(413); // first fill status in req 
			Response res(clientMap[i].getReq()); // then send req to res
			clientMap[i].setRes(res);
			clientMap.at(i).clearReqChunk();
			return false;
		}
		// std::cout << "clean body size" << cleanBody.size() << " " << "[" << cleanBody << "]" << clientMap[i].getBodySize()<< "\n";
		req.set_body(body);
		std::string filename = req.get_file_name();
		/***** get the proper root if its default one or inside location ******/
		std::ofstream file(clientMap[i].getRoot() + "/upload/" + filename);		/* get the root path */
		file << cleanBody;
		std::cout << OR1 <<  " body lenght --------------- \n"   << OR2 << '\n';
	}
	clientMap[i].setReqFull(clientMap.at(i).getReqChunk());
	clientMap.at(i).clearReqChunk();

	//print
	// std::cout << "\n@@@@@@@@@@ end of req\ni : " << i << " \n" << clientMap.at(i).getReqFull() << '\n'
	// 		<< body.length() << " " << bodyLength << std::endl;
	return (false);
}

void	webserv::reading(int i)
{
	resError = true;
	buff = new char[BUFFERSIZE];
	std::cout << YL1 << "READ block " << YL2 << std::endl;
	// 	std::cout << "i : " << i << std::endl;
	int bytesReaded = recv(i, buff, BUFFERSIZE, 0);
	// std::cout <<  bytesReaded  << std::endl;
	if (bytesReaded == -1)
		std::cout <<  "error in reacv"  << std::endl;
	// {
	// FD_CLR(i, &read_set);
	// 	return ;
	// }
	if (bytesReaded < 0)
		// std::cout << "failed to recv" << std::endl;
	if (bytesReaded == 0)
		// std::cout << "the connectuion is done " << std::endl;
	// else
	// 	std::cout << "\nlen recv is < " << bytesReaded << " >\n" << std::endl;
	buff[bytesReaded] = '\0';
	std::string bufTmp;
	std::string tmp = clientMap[i].getReqChunk();
// std::cout << "-------------------------------------\n";
	bufTmp = std::string(buff, bytesReaded);
// std::cout << "############@@@@@@@@@@@@*************\n";
	tmp.append(bufTmp);
	clientMap.at(i).setReqChunk(tmp);

	//print
	// print(clientMap.at(i).getReqChunk(), "Request\n------------------------------");
	// std::cout << "[[[ \n\n" << clientMap.at(i).getReqChunk() << " \n]]]" << std::endl;
	// std::cout  << " " << clientMap.at(i).getReqChunk().find("\r\n\r\n", 0) << std::endl;

	if (clientMap[i].getReqChunk().find("\r\n\r\n", 0) != std::string::npos)
	{
		Request req(clientMap[i].getReqChunk());
		bodyLength = std::strtod(req.get_headers()["Content-Length"].c_str(), NULL);
		clientMap[i].setReq(req);
		if (getRequest(i))
		{
			delete (buff);
			return ;
		}
	}
	else if (!clientMap.at(i).getReqChunk().empty())
	{
		delete (buff) ;
		return ;
	}

	//print
	// std::cout << "\nreadin to writin" << std::endl;
	// std::cout << "\n//--- begin req ---//\n" << std::endl;
	// std::cout << clientMap.find(i)->second.getReqFull() << std::endl;
	// std::cout << "//--- end req ---//\n\n" << "the request size : -> " << clientMap.find(i)->second.getReqFull().length() << '\n';

	//set to write on the client socket
	FD_SET(i, &write_set);
	//clear it from read cuz now it need to be writin on it only
	FD_CLR(i, &read_set);
	delete (buff);
}

std::string	webserv::serveFile(int i)
{
	// std::string htmlFile;
	std::string fileContent;
	
	//if its dir show default file or show index
	if (is_dir && clientMap[i].getReq().get_path() == "/upload/" && clientMap[i].getReq().get_method() == "POST") /* if we upload we should show DONE or the same example.html page */
	{
		std::cout << "upload html response ***********************************\n";
		htmlFile = readFile("stuff/example.html");
	}
	else if (is_dir && clientMap[i].getReq().get_status() != 301) // redirection check
	{
		//if (clientMap[i].serverInf.getIndex()) check if index is on on the configue file
		std::cout << "def********************************** " << clientMap[i].getDefFile() << '\n';
		if (resError)
			htmlFile = readFile(clientMap[i].getRoot() + "/" + clientMap[i].getDefFile());
		std::cout << "htmlFile [" << htmlFile << "]" << clientMap[i].getAutoIndx() << '\n';
		if (htmlFile.empty() && clientMap[i].getAutoIndx() && resError)
		{
			std::cout << OR1<< "---------------------------- wasir gad auto index bdak html -------------------" << OR2 << '\n';
			// autoindex aiGen("stuff", "127.0.0.1", 8080);
			autoindex aiGen(clientMap[i].getRoot(),  clientMap[i].getHost(), clientMap[i].getPort());
			
			if (resError)
				htmlFile = aiGen.pageGen();//readFile(clientMap[i].getRoot() + "/index.html");
			// print(clientMap[i].getRoot() + "/index.html", "here");
		}
		else if (htmlFile.empty() && !clientMap[i].getAutoIndx() && resError)
		{
			std::cout << OR1<< "---------------------------- ERROR no defltfile no auto indx -------------------" << OR2 << '\n';
			// respons 500 or 400
			setResStatus(i, 404, empty, "");
		}
	}
	else if (clientMap[i].getReq().get_status() != 301)
		htmlFile = readFile(clientMap[i].getRoot() + "/default.html");
	/* response here  */
	int x = access(urlPath.c_str(), F_OK);
	if (x == -1)
		setResStatus(i, 404, htmlFile, "404.html");
	x = access(urlPath.c_str(), R_OK);
	if (x == -1)
		setResStatus(i, 403, htmlFile, "403.html");
	if (clientMap[i].getReq().get_status() != 200)
	{
		if (resError)
			htmlFile = clientMap[i].getRes().getHtmlError();
		// print(htmlFile, "BOOOOOM------lalala");
		fileContent = clientMap[i].getRes().getHead() + "Content-Length: " + std::to_string(htmlFile.length()) + "\r\n\r\n" + htmlFile;
		htmlFile.clear();
		return (fileContent);
	}
	fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\r\n\r\n" + htmlFile;
	

	for (std::map<std::string, std::string>::iterator typeIt = mimeMap.begin(); typeIt != mimeMap.end(); ++typeIt)
	{
		if (clientMap.at(i).getReqFull().substr(0, 30).find("." + typeIt->first) != std::string::npos)
		{
			htmlFile = readFile(urlPath);
			fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: " + typeIt->second + "\r\n\r\n" + htmlFile;
		// print(fileContent, "file ctn");
		htmlFile.clear();
			return (fileContent);
		}
	}
	htmlFile.clear();
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
	std::cout << "is dir : -> " << is_dir << '\n';
	std::string dir = clientMap[i].getReq().get_path();
	if (is_dir && !countSlash(dir))
			dir = dir.append("/");// taygadha ha ki me
	std::cout << "\n ---------- \n there is 2 slash dir is [ " << dir << " ]" << '\n';
	size_t pos1 = urlPath.find("/", 0);
	size_t pos2 = urlPath.find("/", pos1 + 1);
	dir = urlPath.substr(pos1, pos2 - pos1 + 1);
	std::cout << "last dir {" <<  dir << "}" << '\n';
	check_dir(i, dir);
}

void	webserv::writing(int i)
{
	std::cout << YL1 <<  "WRITE block " << YL2 << std::endl;
	// std::cout << "here\n[ " << serverMap.find(6)->second.getReqFull() << "]" << std::endl;

	// join url with path
	urlPath = clientMap[i].getRoot() + clientMap[i].getReq().get_path();
	print(urlPath, "url : ");
	// if (urlPath != (clientMap[i].getRoot() + "/upload/"))
	
	print(clientMap[i].getReqFull(), "POST---------");
	//check is dir
	is_dir = false;
	struct stat fileStat;
	if (stat(urlPath.c_str(), &fileStat) == 0)
		if (S_ISDIR(fileStat.st_mode))
			is_dir = true;

	//check method if its allowed
	checkLocMeth(i);

	// std::cout << "i : " << i << std::endl;
	// std::cout << "\n############### the request is : \n" << clientMap.find(i)->second.getReqFull() << std::endl;

	std::string fileContent = serveFile(i);
	long long len = send(i, fileContent.c_str(), fileContent.length(), 0);
	if (len < 0)
		std::cout << "error " << std::endl;
	std::cout << "send \n";
	shutdown(i, SHUT_WR);
	FD_CLR(i, &write_set);

	//if you close the next client willtake fd 6 maybe dont close now
	close(i);
	// clientMap.erase(clientMap.find(i));
}

webserv::webserv(std::vector<webInfo> &serverList, std::map<std::string, std::string> mime)
{
	timeval timeout;
	timeout.tv_sec = 5;
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
		std::cout << OR1 << "\nstart \n----------------" << OR2 << "\n";

		copyRead = read_set;
		copyWrite = write_set;

		// select
		check = select(maxSocket + 1, &copyRead, &copyWrite, NULL, &timeout);
		if (check == 0)
		{
			// loop on clientMap and check if the connection is keep alive and erase that client map
			continue ;
		}
		if (check < 0)
			return;
		std::cout << "after select " << check << '\n';
		// accept connection
		for (int i = 3; i <= maxSocket; ++i)
		{
			std::cout << i << '\n';
			if (serverMap.count(i)) // if its server socket we have to accept it not read it
				acceptSockets(i);
			else if (FD_ISSET(i, &copyRead))
				reading(i);
			else if (FD_ISSET(i, &copyWrite)) // receiv request from clinet
				writing(i);
		}
	}
	serverMap.clear();
	clientMap.clear();
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

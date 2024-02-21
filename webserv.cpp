/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:04:51 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/02/21 15:44:21 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// TO DO :
// look for "// HERE" if no def file exist in the dir show AI
// /dir/ show indx of loc not root
// send chunck
// /dir/ is url not directory
// loop 3la client o chof variable dial time 
// check cgi path if it doesnt have cgi dont fork for it

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
	std::cout << YL1 << "server " << i  << " accepted" << YL2 << std::endl;

	// accept new connection
	clientAddressLen = sizeof(*clientAddress.begin());
	newClientSocket = accept(i, (struct sockaddr *)&clientAddress, &clientAddressLen);
	if (newClientSocket < 0)
	{
		std::cout << "failed to accept" << std::endl;
		return ;
	}

	//nonblocking fds
	int flags = fcntl(newClientSocket, F_GETFL, 0);
	if (fcntl(newClientSocket, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		std::cout << "failed to set non blocking\n";
		close(newClientSocket);
		return ;
	}

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
	clientMap[newClientSocket].setGlobDefFile(serverMap[i].getGlobDefFile());

	// std::cout << "socket is " << i  << " client sock is " << newClientSocket << '\n';
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
			std::cout << statusHtml << " " << *it << '\n';
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
		for (std::vector<std::string>::iterator itV = locIt->locDirName.begin(); itV != locIt->locDirName.end(); itV++)
		{
			std::cout << "location: [" << *itV << "]  |  dir: [" << dir << "]\n";
			if (dir == *itV)
			{
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

void	webserv::parseChunk(int i)
{
	std::string str  = clientMap[i].getReqChunk();
	size_t pos1 = str.find("\r\n\r\n", 0);
	size_t pos2 = str.find("0\r\n\r\n");
	pos1 += 4;
	pos2 += 4;
	std::string sub = str.substr(pos1, pos2 - pos1);
	std::cout << sub.size() << '\n';
	print(sub, "POST---------");
	
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
	std::cout << "["<< transfer << "]" << '\n';
	if (req.get_method() == "POST")
	{
		if (transfer == "chunked")
			parseChunk(i);
		// else
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
		// std::cout << "body" << body << '\n';
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
	{
		std::cout <<  "error in reacv"  << std::endl;
		FD_CLR(i, &read_set);
		close(i);
		clientMap.erase(clientMap.find(i));
		updateMaxSocket();
		return ;
	}
	if (bytesReaded < 0)
		// std::cout << "failed to recv" << std::endl;
	if (bytesReaded == 0)
		// std::cout << "the connectuion is done " << std::endl;
	buff[bytesReaded] = '\0';
	std::string bufTmp;
	std::string tmp = clientMap[i].getReqChunk();
	bufTmp = std::string(buff, bytesReaded);
	tmp.append(bufTmp);
	clientMap.at(i).setReqChunk(tmp);

	//print
	// print(clientMap.at(i).getReqChunk(), "Request\n------------------------------");
	// std::cout << "[[[ \n\n" << clientMap.at(i).getReqChunk() << " \n]]]" << std::endl;
	// std::cout  << " " << clientMap.at(i).getReqChunk().find("\r\n\r\n", 0) << std::endl;

	// std::string 
	Request req(clientMap[i].getReqChunk());
	if (req.get_status() != 200)
	{
		clientMap[i].getReq().set_status(400);
		Response res(clientMap[i].getReq());
		clientMap[i].setRes(res);
		clientMap.at(i).clearReqChunk();
		FD_SET(i, &write_set);
		FD_CLR(i, &read_set);
		delete (buff);
		return ;
	}
	transfer = req.get_headers()["Transfer-Encoding"].c_str();
	if (clientMap[i].getReqChunk().find("\r\n\r\n", 0) != std::string::npos)
	{
		clientMap[i].setReq(req);
		bodyLength = std::strtod(req.get_headers()["Content-Length"].c_str(), NULL);
		if (transfer == "chunked")
		{
			std::cout << "im chunked\n";
			std::string rqst = clientMap[i].getReqChunk();
			if (rqst.find("0\r\n\r\n", 0) != std::string::npos)
			{
				clientMap[i].setReq(req);
				std::cout << "found 0 \n";
				if (!rqst.compare(rqst.size() - 5, 5, "0\r\n\r\n"))
				{
					std::cout << "compared 0 true \n";
					if (getRequest(i))
					{
						delete (buff);
						return ;
					}
				}
			}
			else if (!clientMap.at(i).getReqChunk().empty()) // if not empty return to read after
			{
				delete (buff) ;
				return ;
			}
		}
		else if (getRequest(i))
		{
			delete (buff);
			return ;
		}
	}
	else if (!clientMap.at(i).getReqChunk().empty()) // if not empty return to read after
	{
		delete (buff) ;
		return ;
	}

	//print
	std::cout << "\nreadin to writin" << std::endl;
	// std::cout << "\n//--- begin req ---//\n" << std::endl;
	// std::cout << clientMap.find(i)->second.getReqFull() << std::endl;
	// std::cout << "//--- end req ---//\n\n" << "the request size : -> " << clientMap.find(i)->second.getReqFull().length() << '\n';
	
	std::ofstream fil("zlala.txt");
	fil << clientMap[i].getReqChunk();
	//set to write on the client socket
	FD_SET(i, &write_set);
	//clear it from read cuz now it need to be writin on it only
	FD_CLR(i, &read_set);
	delete (buff);
}

void	webserv::redirOrAutoIndx(int i)
{
	//if (clientMap[i].serverInf.getIndex()) check if index is on on the configue file
	if (resError)
	{
		// HERE
		htmlFile = readFile(clientMap[i].getRoot() + "/" + clientMap[i].getDefFile()); // read default file
		if (htmlFile.empty())
			htmlFile = readFile(clientMap[i].getRoot() + "/" + clientMap[i].getGlobDefFile()); // read global default file
	}
	if (htmlFile.empty() && clientMap[i].getAutoIndx() && resError) // if no default file and auIndx on list AIndx
	{
		std::cout << OR1<< "---------------------------- wasir gad auto index bdak html -------------------" << OR2 << '\n';
		autoindex aiGen(clientMap[i].getRoot(), clientMap[i].getReq().get_path(), clientMap[i].getHost(), clientMap[i].getPort());
		std::cout << "root: " << clientMap[i].getRoot() << "\nhost: " << clientMap[i].getHost() << "\nport: " << clientMap[i].getPort() << '\n';
		if (resError)
			htmlFile = aiGen.pageGen();
		// print(clientMap[i].getRoot() + "/index.html", "here");
	}
	else if (htmlFile.empty() && !clientMap[i].getAutoIndx() && resError) // no def no AIndx error 404
	{
		std::cout << OR1<< "---------------------------- ERROR no defltfile no auto indx -------------------" << OR2 << '\n';
		// respons 500 or 400
		htmlFile = "dirOfErrors/404.html";
		setResStatus(i, 404, htmlFile, "404.html");
	}
}

std::string	webserv::serveFile(int i)
{
	std::string fileContent;
	int 		x;

	//if its dir show default file or show index
	if (is_dir && clientMap[i].getReq().get_path() == "/upload/" && clientMap[i].getReq().get_method() == "POST") /* if we upload we should show DONE or the same example.html page */
	{
		std::cout << "upload html response ***********************************\n";
		htmlFile = readFile("stuff/example.html");
	}
	else if (is_dir && clientMap[i].getReq().get_status() != 301)
		redirOrAutoIndx(i);
	else if (clientMap[i].getReq().get_status() != 301) // if not redirection get default file
		htmlFile = readFile(clientMap[i].getRoot() + "/default.html");
	x = access(urlPath.c_str(), F_OK); // if file path exist
	if (x == -1)
	{
		htmlFile = "dirOfErrors/404.html";
		setResStatus(i, 404, htmlFile, "404.html");
		std::cout << BL1 << " NO file to read\n" << OR2 << '\n';
	}
	if (x != -1)
	{
		x = access(urlPath.c_str(), R_OK); // if file has permission to read
		if (x == -1)
		{
			std::cout << BL1 << " NO PERMISSION TO READ\n" << OR2 << '\n';
			htmlFile = "dirOfErrors/403.html";
			setResStatus(i, 403, htmlFile, "403.html");
		}
	}
	if (clientMap[i].getReq().get_status() != 200) // if status != 200 means there is an error or redirection
	{
		std::cout << BL1 << resError << " STATUS ERROR : " << clientMap[i].getReq().get_status() << OR2 << '\n';
		if (resError)
			htmlFile = clientMap[i].getRes().getHtmlError();
		print(htmlFile, "BOOOOOM------lalala");
		fileContent = clientMap[i].getRes().getHead() + "Content-Length: " + std::to_string(htmlFile.length()) + "\r\n\r\n" + htmlFile;
		htmlFile.clear();
		return (fileContent);
	}
	// if its not error then ok 200
	fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\r\n\r\n" + htmlFile;

	//search for the mime type of the url ( example."mimeType" )
	if (!is_dir)
	{
		htmlFile = readFile(urlPath);
		for (std::map<std::string, std::string>::iterator typeIt = mimeMap.begin(); typeIt != mimeMap.end(); ++typeIt)
		{
			if (urlPath.find("." + typeIt->first) != std::string::npos)
			{
				fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: " + typeIt->second + "\r\n\r\n" + htmlFile;
				htmlFile.clear();
				return (fileContent);
			}
		}
		fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: " + "application/octet-stream" + "\r\n\r\n" + htmlFile;
	}
	// setResStatus(i, 403, htmlFile, "403.html");
	htmlFile.clear();
	return (fileContent);
}

bool	countSlash(std::string dir)
{
	int count = 0;
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
	std::cout << "is dir : -> " << is_dir << dir << '\n';
	if (is_dir && !countSlash(dir))
		urlPath.append("/");// taygadha ha ki me
	std::cout << "\n ---------- \n there is 2 slash dir is [ " << dir << " ]" << '\n';
	size_t pos1 = urlPath.find("/", 0);
	size_t pos2 = urlPath.find("/", pos1 + 1);
	dir = urlPath.substr(pos1, (pos2 + 1) - pos1);
	std::cout << "last dir {" <<  dir << "} " << (pos2 ) << " " << (pos1) << '\n';
	check_dir(i, dir);
}

void	webserv::updateMaxSocket()
{
	for (int i = maxSocket; i > 3;)
	{
		if (!FD_ISSET(i, &write_set) && !FD_ISSET(i, &read_set))
			--i;
		else
		{
			maxSocket = i;
			break;
		}
	}
}

void	webserv::writing(int i)
{
	std::cout << YL1 <<  "WRITE block " << YL2 << std::endl;

	// join url with path
	urlPath = clientMap[i].getRoot() + clientMap[i].getReq().get_path();
	std::cout << OR1 <<  "url : " << urlPath << OR2 << '\n';

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
		std::cout << "error in send" << std::endl;

	std::cout << "SENDING : " << len << '\n';
	
	shutdown(i, SHUT_WR);
	FD_CLR(i, &write_set);

	//update maxClient
	close(i);
	clientMap.erase(clientMap.find(i)); // rmove client
	updateMaxSocket();
	std::cout << "MAXSOCKET : >>>>>>>>>>> " << maxSocket << '\n';
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
			{
				if (FD_ISSET(i, &copyRead))
					acceptSockets(i);
			}
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

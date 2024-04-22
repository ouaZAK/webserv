/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:04:51 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/04/22 19:33:17 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <cstdio>

// to do : buff check 4096 in stack not new
std::string readFile(const std::string &str)
{
	// InputFile open a file to read from
	std::ifstream file(str, std::ios::binary); // the file will be read in binary mode, treating the data as raw bytes rather than text.
	if (!file.is_open())
		return ("");
	std::stringstream inStr;
	inStr << file.rdbuf();
	return (inStr.str());
}

void webserv::setNoBlocking()
{
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	{
		if (fcntl(mapIt->first, F_SETFL, O_NONBLOCK) == -1) // F_SETFL tells the fcntl to set the file status flag with O_NONBLOCK to the fd
			if (serverMap.size() == 1)
				throw "Error:fcntl failed\nFatal error";
	}
}

void webserv::creatAddresses()
{
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
		serverAddress.push_back(mapIt->second.getServerAddress());
}

void webserv::bindSockets()
{
	std::vector<struct sockaddr_in>::iterator adrIt = serverAddress.begin(); // serverMap and serverAddress are the same size
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	{
		if (bind(mapIt->first, reinterpret_cast<struct sockaddr *>(&(*adrIt)), sizeof(*adrIt)) < 0) // once a socket is bound to a port it's ready to start listening for incoming connections
		{
			std::cout << "failed to bind" << '\n';
			if (serverMap.size() == 1)
				throw "Error: bind failed\nFatal error";
			return;
		}
		++adrIt;
	}
}

void webserv::listening()
{
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
	{
		if (listen(mapIt->first, 30) < 0)
			throw "Error: listen failed\nFatal error";
	}
}

void webserv::setFds()
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

void webserv::acceptSockets(int i)
{
	// accept new connection
	clientAddressLen = sizeof(*clientAddress.begin());
	newClientSocket = accept(i, reinterpret_cast<struct sockaddr *>(&clientAddress), &clientAddressLen);
	if (newClientSocket < 0)
	{
		std::cout << "failed to accept connection" << std::endl;
		return;
	}

	// nonblocking fds
	if (fcntl(newClientSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << "failed to set non blocking\n";
		close(newClientSocket);
		return;
	}

	// set newclient to copyRead
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
	clientMap[newClientSocket].setServerNam(serverMap[i].getServnam());
}

int	webserv::checkSize(int i)
{
	// if extracted body is > than body length in reauest header
	if (body.size() > clientMap[i].getBodySize() && clientMap[i].getReq().get_status() == 200)
	{
		htmlFile = "dirOfErrors/413.html";
		setResStatus(i, 413, htmlFile, "413.html");
		clientMap[i].clearReqChunk();
		return 0;
	}
	return 1;
}

void	webserv::creatFile(int i, std::string bodyCopy, Request req)
{
	req.set_body(bodyCopy);
	std::string filename = req.get_file_name();
	std::string rmUpload = clientMap[i].getReq().get_path();
	//if its upload remove it from url so we creat path of the file
	size_t begin = rmUpload.find("/upload", 0);
	if (begin != std::string::npos)
		rmUpload = rmUpload.substr(0,  begin);
	std::ofstream file(clientMap[i].getRoot() + rmUpload + "/" + filename); // OutputFile open file for writing
	file << cleanBody;
}

bool webserv::extractBody(int i)
{
	try
	{
		std::string tmpBody;
		size_t lenBndry;
		
		tmpBody = clientMap[i].getReqChunk();
		posRNRN = tmpBody.find("\r\n\r\n", 0) + 4;
		body = tmpBody.substr(posRNRN, tmpBody.length() - posRNRN);
		
		std::string bodyCopy = body;
		
		if (transfer == "chunked")
		{
			while (strToJoinChunk.size() < body.size())
			{
				size_t RN = bodyCopy.find("\r\n", 0);
				std::string strHexNbr = bodyCopy.substr(0, RN);
				if (strHexNbr == "0")
					break;
				
				int hexNb;
				std::istringstream s(strHexNbr); // its like ifstream intead of file it read from std::string
				s >> std::hex >> hexNb; // its in iomanip so the stream can read the hex notation as it is
				strToJoinChunk += bodyCopy.substr(RN + 2, hexNb);
				
				bodyCopy = bodyCopy.substr(RN + 2 + 2 + hexNb, bodyCopy.size() - RN + 2 + hexNb);
			}
			bodyCopy = strToJoinChunk;
			strToJoinChunk.clear();
		}
		if (clientMap[i].getReq().get_headers()["Content-Type"].find("multipart/form-data") == std::string::npos)
		{
			if (!checkSize(i))
				return false;
			std::string filename = "text";
			std::ofstream file(clientMap[i].getRoot() + "/" + filename);
			file << body;
			return true;
		}
		std::string boundry;
		if (transfer != "chunked") // if not chunked take boundry in body
		{
			lenBndry = body.find("\r\n", 0); // to know length of first line
			boundry = body.substr(0, lenBndry + 2);
		}
		else
		{
			lenBndry = bodyCopy.find("\r\n", 0); // if its chunked take boundry in bodyCopy
			boundry = bodyCopy.substr(0, lenBndry + 2);
		}

		size_t findBndry;
		findBndry = bodyCopy.find(boundry.c_str(), lenBndry);
		if (findBndry == std::string::npos)
		{
			posRNRN = body.find("\r\n\r\n", 0) + 4;
			cleanBody = body.substr(posRNRN, body.length() - posRNRN - (lenBndry + 7)); /* 4 for "-" and 2 for '\n' */
			if (!checkSize(i))
				return false;
			
			Request req = clientMap[i].getReq();
			creatFile(i, bodyCopy, req);
		}
		while (bodyCopy.find(boundry.c_str(), lenBndry) != std::string::npos)
		{
			posRNRN = bodyCopy.find("\r\n\r\n", 0) + 4;
			cleanBody = bodyCopy.substr(posRNRN, bodyCopy.find(boundry.c_str(), lenBndry) - posRNRN - 3);

			if (!checkSize(i))
				return false;
		
			clientMap[i].getReq().set_body(bodyCopy);
			Request req = clientMap[i].getReq();
			creatFile(i, bodyCopy, req);

			bodyCopy = bodyCopy.substr(bodyCopy.find(boundry.c_str(), lenBndry), bodyCopy.size() - bodyCopy.find(boundry.c_str()));
		
			if (bodyCopy.find(boundry.c_str(), lenBndry) == std::string::npos)
			{
				std::string::iterator it = boundry.end();
				boundry.erase(--it);
				boundry.erase(--it);
				if (bodyCopy.find((boundry).c_str(), lenBndry + 2) != std::string::npos)
				{
					posRNRN = bodyCopy.find("\r\n\r\n", 0) + 4;
					cleanBody = bodyCopy.substr(posRNRN, bodyCopy.length() - posRNRN - (lenBndry + 7));

					if (!checkSize(i))
						return false;
					creatFile(i, bodyCopy, req);
					break;
				}
			}
		}
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << '\n';
	}
	return true;
}

void webserv::setResStatus(int i, int status, std::string &htmlFile, std::string statusHtml)
{
	int x, y;
	for (std::vector<std::string>::iterator it = clientMap[i].getErrorPages().begin(); it != clientMap[i].getErrorPages().end(); ++it)
	{
		if (statusHtml == *it)
		{
			x = access(("dirOfErrors/" + statusHtml).c_str(), F_OK);
			y = access(("dirOfErrors/" + statusHtml).c_str(), R_OK);
			if (x != -1 && y != -1)
			{
				htmlFile = readFile("dirOfErrors/" + statusHtml);
				resError = false;
				break;
			}
		}
	}
	clientMap[i].getReq().set_status(status);
	Response res(clientMap[i].getReq());
	clientMap[i].setRes(res);
}

bool countSlash(std::string &dir)
{
	int count = 0;
	for (std::string::iterator it = dir.begin(); it != dir.end(); ++it)
	{
		if (*it == '/')
			++count;
		if (count >= 2)
			return (true);
	}
	dir.append("/");
	return (false);
}

bool webserv::check_dir(int i, std::string dir)
{
	/* location vector is loc */
	std::vector<Location> loc = clientMap[i].getLoc();
	for (std::vector<Location>::iterator locIt = loc.begin(); locIt != loc.end(); ++locIt)
	{
		for (std::vector<std::string>::iterator itV = locIt->locDirName.begin(); itV != locIt->locDirName.end(); itV++)
		{
			if (dir == *itV)
			{
				if (locIt->redirect_status == 301) // if its redirection
				{
					clientMap[i].getReq().set_status(301);
					Response res(clientMap[i].getReq(), locIt->redirect_to_dir);
					clientMap[i].setRes(res);
					return (true);
				}
				/* get the default file in directory locDirName */
				for (std::vector<std::string>::iterator itV2 = locIt->methods.begin(); itV2 != locIt->methods.end(); itV2++)
				{
					if (clientMap[i].getReq().get_method() == *itV2)
					{
						clientMap[i].setDefFile(locIt->default_file);
						return (false);
					}
				}
				htmlFile = "dirOfErrors/405.html";
				setResStatus(i, 405, htmlFile, "405.html");
				return (true);
			}
		}
	}
	return (false);
}

void webserv::parseChunk(int i)
{
	std::string str = clientMap[i].getReqChunk();
	size_t pos1 = str.find("\r\n\r\n", 0);
	size_t pos2 = str.find("0\r\n\r\n");
	pos1 += 4;
	pos2 += 4;
	std::string sub = str.substr(pos1, pos2 - pos1);
}

bool webserv::getRequest(int i)
{
	Request req = clientMap[i].getReq();
	std::string dir = clientMap[i].getReq().get_path();
	if (check_dir(i, dir))
	{
		clientMap[i].setReqFull(clientMap[i].getReqChunk());
		clientMap[i].clearReqChunk();
		return false;
	}
	if (req.get_method() == "DELETE")
	{
		clientMap[i].setReqFull(clientMap[i].getReqChunk());
		clientMap[i].clearReqChunk();
		return (false);
	}
	else if (req.get_method() == "POST")
	{
		if (!extractBody(i))
			return false;
		if (body.length() < bodyLength)
			return (true);
	}
	clientMap[i].setReqFull(clientMap[i].getReqChunk());
	clientMap[i].clearReqChunk();
	return (false);
}

bool isCgi(std::string path, std::string prefix, std::string extension)
{
	if (path.compare(0, prefix.length(), prefix) == 0 &&
		path.length() >= extension.length() &&
		path.compare(path.length() - extension.length(), extension.length(), extension) == 0)
		return true;
	return false;
}

void webserv::reading(int i)
{
	resError = true;
	try
	{
		buff = new char[BUFFERSIZE];
	}
	catch (std::exception &e)
	{
		std::cout << "error bad alloc" << std::endl;
		FD_CLR(i, &read_set);
		close(i);
		clientMap.erase(clientMap.find(i));
		updateMaxSocket();
		return;
	}
	int bytesReaded = recv(i, buff, BUFFERSIZE, 0);// 0 for normal operation or set other flags to block system until reading all the data from sock for ex
	if (bytesReaded == -1)
	{
		std::cout << "error in reacv" << std::endl;
		FD_CLR(i, &read_set);
		close(i);
		clientMap.erase(clientMap.find(i));
		updateMaxSocket();
		return;
	}
	if (bytesReaded == 0)
	{
		std::cout << "the connectuion is done" << std::endl;
		FD_SET(i, &write_set);
		FD_CLR(i, &read_set);
		delete (buff);
		return;
	}
	buff[bytesReaded] = '\0';
	std::string bufTmp;
	std::string tmp = clientMap[i].getReqChunk();
	bufTmp = std::string(buff, bytesReaded);
	tmp.append(bufTmp);
	clientMap[i].setReqChunk(tmp);

	Request req(clientMap[i].getReqChunk());
	int resp = req.get_status();
	if (resp != 200)
	{
		if (resp == 400)
		{
			htmlFile = "dirOfErrors/400.html";
			setResStatus(i, 400, htmlFile, "400.html");
		}
		else if (resp == 414)
		{
			htmlFile = "dirOfErrors/414.html";
			setResStatus(i, 414, htmlFile, "414.html");
		}
		clientMap[i].clearReqChunk();
		FD_SET(i, &write_set);
		FD_CLR(i, &read_set);
		delete (buff);
		return;
	}
	
	transfer = req.get_headers()["Transfer-Encoding"].c_str();
	if (clientMap[i].getReqChunk().find("\r\n\r\n", 0) != std::string::npos)
	{
		clientMap[i].setReq(req);
		bodyLength = std::strtod(req.get_headers()["Content-Length"].c_str(), NULL);
		if (transfer == "chunked")
		{
			std::string rqst = clientMap[i].getReqChunk();
			if (rqst.find("0\r\n\r\n", 0) != std::string::npos)
			{
				clientMap[i].setReq(req);
				if (!rqst.compare(rqst.size() - 5, 5, "0\r\n\r\n"))
				{
					if (getRequest(i))
					{
						delete (buff);
						return;
					}
				}
			}
			else if (!clientMap[i].getReqChunk().empty()) // if not empty return to read after
			{
				delete (buff);
				return;
			}
		}
		else if (getRequest(i))
		{
			delete (buff);
			return;
		}
	}
	else if (!clientMap[i].getReqChunk().empty()) // if not empty return to read after
	{
		delete (buff);
		return;
	}

	aCgi = isCgi(req.get_path(), "/cgi/", ".py") ? aCgi = true : aCgi = false;

	FD_CLR(i, &read_set);
	FD_SET(i, &write_set);
	delete (buff);
}

void webserv::redirOrAutoIndx(int i)
{
	bool AutoIndx = clientMap[i].getAutoIndx();
	std::string root = clientMap[i].getRoot();
	
	htmlFile = readFile(urlPath + "/" + clientMap[i].getDefFile()); // read default file
	if (htmlFile.empty())
		htmlFile = readFile(urlPath + "/" + clientMap[i].getGlobDefFile()); // read global default file
	if (htmlFile.empty() && AutoIndx) // if no default file and auIndx on list AIndx
	{
		autoindex aiGen(root, clientMap[i].getReq().get_path(), clientMap[i].getHost(), clientMap[i].getPort());
		if (resError)
			htmlFile = aiGen.pageGen();
	}
	else if (htmlFile.empty() && !AutoIndx) // no def no AIndx error 403
	{
		htmlFile = "dirOfErrors/403.html";
		setResStatus(i, 403, htmlFile, "403.html");
	}
}

std::string webserv::serveFile(int i)
{
	std::string fileContent;
	int x;

	// if its dir show default file or show index
	if (resError)
	{
		if (is_dir && clientMap[i].getReq().get_method() == "DELETE")
		{
			htmlFile = "dirOfErrors/403.html";
			setResStatus(i, 403, htmlFile, "403.html");
		}
		else if (!aCgi && clientMap[i].getReq().get_method() == "POST") // thank you hakime
		{
			clientMap[i].getReq().set_status(201);
			Response res(clientMap[i].getReq());
			clientMap[i].setRes(res);
			resError = false;
		}
		else if (is_dir && clientMap[i].getReq().get_method() != "DELETE" && clientMap[i].getReq().get_status() != 301)
			redirOrAutoIndx(i);
		else if (clientMap[i].getReq().get_status() != 301) // if not redirection get default file
		{
			if (!aCgi)
				htmlFile = readFile(clientMap[i].getRoot() + "/index.html");
		}
	}
	x = access(urlPath.c_str(), F_OK); // if file path exist
	if (x == -1)
	{
		htmlFile = "dirOfErrors/404.html";
		setResStatus(i, 404, htmlFile, "404.html");
	}
	if (x != -1)
	{
		x = access(urlPath.c_str(), R_OK); // if file has permission to read
		if (x == -1 || (is_dir && access(urlPath.c_str(), X_OK) == -1))
		{
			htmlFile = "dirOfErrors/403.html";
			setResStatus(i, 403, htmlFile, "403.html");
		}
	}
	if (clientMap[i].getReq().get_status() != 200) // if status != 200 means there is an error or redirection HAKIME RESPONSE CREATED HERE
	{
		if (resError) // if resEroor is true ofc
			htmlFile = clientMap[i].getRes().getHtmlError();
		else if (clientMap[i].getReq().get_status() == 201)
		{
			htmlFile = readFile(clientMap[i].getRoot() + "/Done.html");
			if (htmlFile.empty())
				htmlFile = clientMap[i].getRes().getHtmlError();
		}
		fileContent = clientMap[i].getRes().getHead() + "Content-Length: " + std::to_string(htmlFile.length()) + "\r\n\r\n" + htmlFile;
		htmlFile.clear();
		return (fileContent);
	}
	// if its not error then ok 200
	fileContent = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(htmlFile.length())  + "\r\nContent-Type: text/html\r\n\r\n" + htmlFile;

	// search for the mime type of the url ( example."mimeType" )
	if (!is_dir)
	{
		if (!aCgi)
			htmlFile = readFile(urlPath);
		if (clientMap[i].getReq().get_method() == "DELETE")
		{
			clientMap[i].getReq().set_status(204);
			Response res(clientMap[i].getReq());
			clientMap[i].setRes(res);
			htmlFile = clientMap[i].getRes().getHtmlError();
			std::remove((clientMap[i].getRoot() + clientMap[i].getReq().get_path()).c_str());
			fileContent = clientMap[i].getRes().getHead() + "Content-Length: " + std::to_string(htmlFile.length()) + "\r\n\r\n" + htmlFile;
			htmlFile.clear();
			return (fileContent);
		}
		for (std::map<std::string, std::string>::iterator typeIt = mimeMap.begin(); typeIt != mimeMap.end(); ++typeIt)
		{
			if (urlPath.find("." + typeIt->first) != std::string::npos)
			{
				if (!aCgi)
					fileContent = "HTTP/1.1 200 OK\nContent-Length: " + std::to_string(htmlFile.length()) + "\nContent-Type: " + typeIt->second + "\r\n\r\n" + htmlFile;
				else
					fileContent = "HTTP/1.1 200 OK\n" + htmlFile;
				htmlFile.clear();
				return (fileContent);
			}
		}
		fileContent = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(htmlFile.length()) + "\r\nContent-Type: " + "application/octet-stream" + "\r\n\r\n" + htmlFile;
	}
	htmlFile.clear();
	return (fileContent);
}

bool webserv::is_alias(int i, std::string &dir)
{
	countSlash(dir);
	std::vector<Location> loc = clientMap[i].getLoc();
	for (std::vector<Location>::iterator locIt = loc.begin(); locIt != loc.end(); ++locIt)
	{
		for (std::vector<std::string>::iterator itV = locIt->locDirName.begin(); itV != locIt->locDirName.end(); itV++)
		{
			if (dir == *itV && !locIt->alias.empty())
			{
				dir = (locIt->alias);
				struct stat fileStat;
				if (stat((clientMap[i].getRoot() + dir).c_str(), &fileStat) == 0)
				{
					if (S_ISDIR(fileStat.st_mode))
						is_dir = true;
					return (true);
				}
				return (true);
			}
		}
	}
	return (false);
}

void webserv::checkLocMeth(int i)
{
	std::string dir = clientMap[i].getReq().get_path();
	if (is_alias(i, dir))
		urlPath = clientMap[i].getRoot() + dir;
	
	// check is dir
	is_dir = false;
	struct stat fileStat;
	if (stat(urlPath.c_str(), &fileStat) == 0)
		if (S_ISDIR(fileStat.st_mode))
			is_dir = true;
	
	if ((is_dir && !countSlash(dir)))
		urlPath = clientMap[i].getRoot() + dir;
		
	size_t pos1 = urlPath.find("/", 0);
	size_t pos2 = urlPath.find("/", pos1 + 1);
	dir = urlPath.substr(pos1, (pos2 + 1) - pos1);
	check_dir(i, dir);
}

void webserv::updateMaxSocket()
{
	for (int i = maxSocket; i >= 3;)
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

void webserv::writing(int i)
{
	urlPath = clientMap[i].getRoot() + clientMap[i].getReq().get_path();
	
	// check method if its allowed
	checkLocMeth(i);

	//serv file
	std::string fileContent = serveFile(i);
	long long written = send(i, fileContent.c_str() + clientMap[i].oldByteSent, fileContent.length() - clientMap[i].oldByteSent, 0);
	if (written < 0)
	{
		std::cout << "error in send" << std::endl;
		FD_CLR(i, &write_set);
		shutdown(i, SHUT_WR);
		updateMaxSocket();
		close(i);
		clientMap.erase(clientMap.find(i)); // rmove client
		return;
	}

	clientMap[i].oldByteSent += written;
	if ((size_t)clientMap[i].oldByteSent < fileContent.length())
		return;

	FD_CLR(i, &write_set);
	shutdown(i, SHUT_WR);
	
	// update maxClient
	updateMaxSocket();
	close(i);
	clientMap.erase(clientMap.find(i)); // rmove client
}

webserv::webserv(std::vector<webInfo> &serverList, std::map<std::string, std::string> mime)
{
	timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	mimeMap = mime;

	// create server socket in a map
	for (std::vector<webInfo>::iterator it = serverList.begin(); it != serverList.end(); ++it)
		serverMap.insert(std::make_pair(it->getSock(), *it));

	// set to non blocking
	setNoBlocking();

	// to close the socket after program ends
	int nbr = 1;
	for (mapIt = serverMap.begin(); mapIt != serverMap.end(); ++mapIt)
		setsockopt(mapIt->first, SOL_SOCKET, SO_REUSEADDR, &nbr, sizeof(nbr));

	// print
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
	while (1)
	{
		copyRead = read_set;
		copyWrite = write_set;

		// select
		check = select(maxSocket + 1, &copyRead, &copyWrite, NULL, &timeout);
		if (check < 0)
			throw("internal error 500 or something");
			
		for (int i = 3; i <= maxSocket; ++i)
		{
			if (serverMap.count(i)) // if its server socket we have to accept it not read it
			{
				if (FD_ISSET(i, &copyRead))
					acceptSockets(i);
			}
			else if (FD_ISSET(i, &copyRead))
				reading(i);
			else if (FD_ISSET(i, &copyWrite)) // receiv request from clinet
			{
				if (aCgi && resError)
				{
					slowCgi cgiScr(clientMap[i]);
					htmlFile = cgiScr.slowCgiExecute(clientMap[i]);
					if (htmlFile == "500")
					{
						htmlFile = "dirOfErrors/500.html";
						setResStatus(i, 500, htmlFile, "500.html");
					}
				}
				writing(i);
			}
		}
	}
	serverMap.clear();
	clientMap.clear();
}

webserv::~webserv()
{
}
std::map<int, webInfo> webserv::getmap() const
{
	return (serverMap);
}

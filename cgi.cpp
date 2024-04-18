/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:03:16 by asidqi            #+#    #+#             */
/*   Updated: 2024/04/17 16:54:14 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include "Request.hpp"
#include <sys/wait.h>

#include "webserv.hpp"

// cgiElem["REMOTE_ADDR"] = "";
// cgiElem["REMOTE_USER"] = "";
// cgiElem["HTTP_USER_AGENT"] = "";
// cgiElem["HTTP_REFERER"] = "";

char **slowCgi::mapToChars(std::map<std::string, std::string> inputMap)
{
	char **result = NULL;
	try
	{
		result = new char *[inputMap.size() + 1];
		int i = 0;
		std::string entry;

		for (std::map<std::string, std::string>::iterator it = inputMap.begin(); it != inputMap.end(); ++it)
		{
			entry.clear();
			entry = it->first + "=" + it->second;
			result[i] = new char[entry.length() + 1];
			std::strcpy(result[i], entry.c_str());
			i++;
		}
		result[i] = NULL;
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << '\n';
	}
	return result;
}

std::string slowCgi::slowCgiExecute(clientInfo &clientMap)
{
	char **envp = NULL;
	FILE *tempFile = std::tmpfile();
	int fileFD = fileno(tempFile);
	FILE *tempBody = std::tmpfile();
	int tmpBodyFD = fileno(tempBody);
	int stdOutput = dup(1);
	int stdInput = dup(0);
	std::string joined;

	if (write(fileFD, clientMap.getReq().get_body().c_str(), clientMap.getReq().get_body().size()) == -1)
		write(tmpBodyFD, "500", 3);
	if (lseek(fileFD, 0, SEEK_SET) == -1)
		write(tmpBodyFD, "500", 3);
	pid_t pid = fork();
	if (pid == -1)
		write(tmpBodyFD, "500", 3);
	else if (pid == 0)
	{
		// std::cout << "**************--------CGI-------************ : [" << clientMap.getReq().get_body() << "]\n";
		envp = mapToChars(cgiElem);
		if (clientMap.getReq().get_method() == "POST" && !clientMap.getReq().getCleanBody().empty())
		{
			if (write(fileFD, clientMap.getReq().getCleanBody().c_str(), clientMap.getReq().getCleanBody().size()) == -1)
				write(tmpBodyFD, "500", 3);
			if (lseek(fileFD, 0, SEEK_SET) == -1)
				write(tmpBodyFD, "500", 3);
		}
		// std::cout << "[" << "./" + clientMap.getRoot() + clientMap.getReq().get_path() << "]" << std::endl;
		dup2(fileFD, 0);
		dup2(tmpBodyFD, 1);
		if (execve(("./" + clientMap.getRoot() + clientMap.getReq().get_path()).c_str(), NULL, envp) == -1)
		{
			write(tmpBodyFD, "500", 3);
		}
		delete []envp;
	}
	else
	{
		// Wait for the child process to finish
		// int status;
		if (waitpid(-1, NULL, 0) == -1)
            write(tmpBodyFD, "500", 3);

		// int exstatus = WIFEXITED(status);
		if (lseek(tmpBodyFD, 0, SEEK_SET) == -1)
            write(tmpBodyFD, "500", 3);

		std::cout<< "---" << tmpBodyFD << "---" << '\n';
		// std::cerr << "send from parent " << '\n';
		// do a while here to read 1024 little by little if bytereaded == 1024 put '\0' if byte readed > 0 join the readed untill the bytereaded == 0 then return response which is buffer in that case
		char buffer[1025];

		int byteReaded;
		while (true)
		{
			memset(buffer, 0, 1024);
			byteReaded = read(tmpBodyFD, buffer, 1024);
			if (byteReaded == -1)
			{
				write(tmpBodyFD, "500", 3);
				dup2(stdOutput, 1);
				return ("");
			}
			else if (byteReaded == 0)
				break;
			if (byteReaded > 0)
				joined += buffer;
		}
	}
	dup2(stdOutput, 1);
	dup2(stdInput, 0);
	// std::cerr << "response from child: \n[" << joined << "] size: " << joined.size() << '\n';
	if (!pid)
		exit(0);
	return (joined);
}

slowCgi::slowCgi(clientInfo &ci)
{
	// cgiElem["HTTP_TRANSFER_ENCODING"] = ci.getReq().get_headers()["Transfer-Encoding"];
	cgiElem["QUERY_STRING"] = ci.getReq().get_oquery();
	cgiElem["REQUEST_URI"] = ci.getReq().get_path() + "?" + ci.getReq().get_oquery();

	cgiElem["CONTENT_LENGTH"] = ci.getReq().get_headers()["Content-Length"];
	cgiElem["CONTENT_TYPE"] = ci.getReq().get_headers()["Content-Type"];
	cgiElem["REQUEST_METHOD"] = ci.getReq().get_method();
	cgiElem["SERVER_NAME"] = ci.getServerNam();
	cgiElem["SERVER_SOFTWARE"] = "Algmad/3.0"; // not needed?
	cgiElem["REDIRECT_STATUS"] = "200";        // not needed?
	cgiElem["HTTP_CONNECTION"] = ci.getReq().get_headers()["Connection"];
	cgiElem["HTTP_HOST"] = ci.getServerNam();
	cgiElem["HTTP_VERSION"] = "HTTP/1.1";
	cgiElem["HTTP_USER_AGENT"] = ci.getReq().get_headers()["User-Agent"];
	cgiElem["HTTP_ACCEPT"] = ci.getReq().get_headers()["Accept"]; // print later to check if all the line is there
	cgiElem["SERVER_PROTOCOL"] = "HTTP/1.1";
	cgiElem["PATH_INFO"] = ci.getReq().get_path();
}

slowCgi::slowCgi(slowCgi const &other)
{
	if (this != &other)
		cgiElem = other.cgiElem;
	return;
}

slowCgi::~slowCgi()
{
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:03:16 by asidqi            #+#    #+#             */
/*   Updated: 2024/02/25 19:15:28 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include "Request.hpp"
#include <sys/wait.h>

#include"webserv.hpp"

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
        std::cerr << e.what() << '\n';
    }
    return result;
}

std::string slowCgi::slowCgiExecute(clientInfo &clientMap)
{
    char **envp = NULL;
    FILE* tempFile = std::tmpfile();
    int fileFD = fileno(tempFile);
    FILE* tempBody = std::tmpfile();
    int tmpBodyFD = fileno(tempBody);

    if (write(fileFD, clientMap.getReq().get_body().c_str(), clientMap.getReq().get_body().size()) == -1)
    {
        clientMap.getReq().set_status(500);
        Response res(clientMap.getReq());
        clientMap.setRes(res);
    }
    if (lseek(fileFD, 0, SEEK_SET) == -1)
    {
        clientMap.getReq().set_status(500);
        Response res(clientMap.getReq());
        clientMap.setRes(res);
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        clientMap.getReq().set_status(500);
        Response res(clientMap.getReq());
        clientMap.setRes(res);
    }
    else if (pid == 0)
    {
        envp = mapToChars(cgiElem);
        std::cout << "**************---------------************ :" << clientMap.getReq().get_body() << '\n';
        std::cout << "./" + clientMap.getRoot() + clientMap.getReq().get_path() << std::endl;
        dup2(fileFD, 0);
        dup2(tmpBodyFD, 1);
        // dup2(CtoP[1], 1);
        if (execve(("./" + clientMap.getRoot() + clientMap.getReq().get_path()).c_str(), NULL, envp) == -1)
        {
            clientMap.getReq().set_status(500);
            Response res(clientMap.getReq());
            clientMap.setRes(res);
        }
    }
    // Wait for the child process to finish
    int status;
    if (waitpid(pid, &status, 0) == -1)
    {
        clientMap.getReq().set_status(500);
        Response res(clientMap.getReq());
        clientMap.setRes(res);
    }
    // int exstatus = WIFEXITED(status);
    if (lseek(tmpBodyFD, 0, SEEK_SET) == -1)
    {
        clientMap.getReq().set_status(500);
        Response res(clientMap.getReq());
        clientMap.setRes(res);
    }
    
    std::cout << "send from parent " << '\n';
    // do a while here to read 1024 little by little if bytereaded == 1024 put '\0' if byte readed > 0 join the readed untill the bytereaded == 0 then return response which is buffer in that case 
    char buffer[1024];

    int byteReaded = 1;
    std::string joined;
    while (byteReaded)
    {
        memset(buffer, 0, 1024);
        byteReaded = read(tmpBodyFD, buffer, 1024);
        if (byteReaded == -1)
        {
            std::cout << "read failed\n";
            return ("");
        }
        if (byteReaded > 0)
            joined.append(buffer);
    }
    std::cout << "response from child: \n"
              << joined << " " << joined.size() << '\n';
    return (joined);
}

slowCgi::slowCgi(clientInfo &ci)
{
    // cgiElem["HTTP_TRANSFER_ENCODING"] = ci.getReq().get_headers()["Transfer-Encoding"];
    
    cgiElem["CONTENT_LENGTH"] = ci.getReq().get_headers()["Content-Length"];
    cgiElem["CONTENT_TYPE"] = ci.getReq().get_headers()["Content-Type"];
    cgiElem["REQUEST_METHOD"] = ci.getReq().get_method();
    cgiElem["REQUEST_URI"] = ci.getReq().get_path();
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



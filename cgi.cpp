/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:03:16 by asidqi            #+#    #+#             */
/*   Updated: 2024/02/25 10:00:23 by zouaraqa         ###   ########.fr       */
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
    char **envp;
    int CtoP[2];
    pipe(CtoP);
    
    if (pipe(CtoP) < 0)
    {
        clientMap.getReq().set_status(500);
        Response res(clientMap.getReq());
        clientMap.setRes(res);
    }
    envp = mapToChars(cgiElem);

    pid_t fd = fork();
    if (fd == -1)
    {
        clientMap.getReq().set_status(500);
        Response res(clientMap.getReq());
        clientMap.setRes(res);
    }
    else if (fd == 0)
    {
        char **args;
        args = new char *[3];
        
        args[0] = strdup("/usr/local/bin/python3");
        args[1] = strdup(const_cast<char *>((clientMap.getRoot() + clientMap.getReq().get_path()).c_str()));
        args[2] = NULL;
        close(CtoP[0]);
        std::cout << "from child : " << args[1] << '\n';
        dup2(CtoP[1], 1);
        if (execve(args[0], args, envp) == -1)
        {
            std::cout << "EXECVE ERROR\n";
            clientMap.getReq().set_status(500);
            Response res(clientMap.getReq());
            clientMap.setRes(res);
        }
        close(CtoP[1]);
    }
    close(CtoP[1]);
    // Wait for the child process to finish
    int status;
    waitpid(fd, &status, 0);
    // std::cout << "par file is : \n" << fullstr << '\n';
    std::cout << "send from parent " << '\n';
    close(CtoP[1]);
    // do a while here to read 1024 little by little if bytereaded == 1024 put '\0' if byte readed > 0 join the readed untill the bytereaded == 0 then return response which is buffer in that case 
    char buffer[1024];
    if (fcntl(CtoP[0], F_SETFL, O_NONBLOCK) == -1)
        std::cout << "er\n";
    int byteReaded = 1;
    std::string joined;
    while (byteReaded)
    {
        byteReaded = read(CtoP[0], buffer, 1024);
        if (byteReaded == -1)
        {
            std::cout << "read failed\n";
            return ("");
        }
        else if (byteReaded == 1024)
            buffer[byteReaded] = '\0';
        if (byteReaded > 0)
            joined.append(buffer);
    }
    close(CtoP[0]);
    std::cout << "response from child: \n"
              << buffer << " " << strlen(buffer) << '\n';
    return (joined);
}

slowCgi::slowCgi(clientInfo &ci)
{
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

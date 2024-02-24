/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asidqi <asidqi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:03:16 by asidqi            #+#    #+#             */
/*   Updated: 2024/02/23 19:14:25 by asidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cgi.hpp"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include "Request.hpp"
#include <sys/wait.h>
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
    char buffer[3000];
    int xd = read(CtoP[0], buffer, 3000);
    buffer[xd] = '\0';
    close(CtoP[0]);
    std::cout << "response from child: \n"
              << buffer << " " << strlen(buffer) << '\n';

    return (buffer);
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

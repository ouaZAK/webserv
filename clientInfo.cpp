/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asidqi <asidqi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/02/24 14:32:04 by asidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "clientInfo.hpp"

clientInfo::clientInfo()
{
	
}

clientInfo::~clientInfo()
{
}

void	clientInfo::setReqFull(std::string cnt)
{
	reqFull = cnt;
}

std::string clientInfo::getReqFull() const
{
	return (reqFull);
}

void	clientInfo::setReqChunk(std::string cnt)
{
	reqChunk = cnt;
}

std::string clientInfo::getReqChunk() const
{
	return (reqChunk);
}

std::string clientInfo::getRoot()
{
	return (root);
}

void	clientInfo::setRoot(std::string str)
{
	root = str;
}

void	clientInfo::clearReqChunk()
{
	reqChunk.clear();
}

std::vector<Location> clientInfo::getLoc() const
{
	return (locationVec);
}

void	clientInfo::setLoc(std::vector<Location> loc)
{
	locationVec = loc;
}

Request		&clientInfo::getReq()
{
	return (req);
}

void	clientInfo::setReq(Request reqst)
{
	req = reqst;
}

size_t	 clientInfo::getBodySize() const
{
	return (bodySize);
}

void	clientInfo::setBodySize(size_t bodySize) 
{
	this->bodySize = bodySize;
}

Response 	&clientInfo::getRes()
{
	return (res);
}

void	clientInfo::setRes(Response resp)
{
	res = resp;
}

void	clientInfo::setAutoIndx(bool ai)
{
	autoIndx = ai;	
}

bool	clientInfo::getAutoIndx() const
{
	return (autoIndx);
}

void		clientInfo::setDefFile(std::string defF)
{
	defFile = defF;	
}

std::string	clientInfo::getDefFile() const
{
	return (defFile);
}

void	clientInfo::setErrorPages(std::vector<std::string> erPages)
{
	errorPages = erPages;
}

std::vector<std::string> clientInfo::getErrorPages() const
{
	return (errorPages);
}

void	clientInfo::setPort(int p)
{
	port = p;
}

int	clientInfo::getPort()
{
	return (port);
}

void	clientInfo::setHost(std::string hst)
{
	host = hst;
}

std::string	clientInfo::getHost()
{
	return (host);
}

void	clientInfo::setGlobDefFile(std::string defF)
{
	globDefFile = defF;
}

std::string	clientInfo::getGlobDefFile() const
{
	return (globDefFile);
}

std::string	clientInfo::getServerNam() const
{
	return(servNam);
}

void		clientInfo::setServerNam(std::string other)
{
	servNam = other;
}
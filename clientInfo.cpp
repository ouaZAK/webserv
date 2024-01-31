/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 09:06:33 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/01/31 10:37:37 by zouaraqa         ###   ########.fr       */
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

void	clientInfo::reqChunckClear()
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

Request		clientInfo::getReq() const
{
	return (req);
}

void	clientInfo::setReq(Request reqst)
{
	req = reqst;
}
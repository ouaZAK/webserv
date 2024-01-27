/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientInfo.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 09:06:33 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/01/27 10:15:19 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clientInfo.hpp"

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
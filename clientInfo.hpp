/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 09:05:40 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/02/03 09:21:41 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "Location.hpp"
#include "Request.hpp"

class clientInfo
{
	private:
		std::string 			reqFull;
		std::string 			reqChunk;
		std::string 			root;
		std::vector<Location>	locationVec;
		Request					req;
		size_t					bodySize;
	public:
		clientInfo();
		~clientInfo();
		
		void					reqChunckClear();
		std::string 			getReqFull() const;
		void					setReqFull(std::string cnt);
		std::string 			getReqChunk() const;
		void					setReqChunk(std::string cnt);
		std::string 			getRoot();
		void					setRoot(std::string str);
		Request 				getReq() const;
		void					setReq(Request reqst);
		size_t	 				getBodySize() const;
		void					setBodySize(size_t bodySize);
		std::vector<Location>	getLoc() const;
		void					setLoc(std::vector<Location> loc);
};
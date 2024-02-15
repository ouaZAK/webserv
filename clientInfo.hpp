/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 09:05:40 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/02/15 18:56:51 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "Location.hpp"
#include "Request.hpp"
#include "Response.hpp"

class clientInfo
{
	private:
		std::string 			reqFull;
		std::string 			reqChunk;
		std::string 			root;
		std::vector<Location>	locationVec;
		Request					req;
		Response				res;
		size_t					bodySize;
		std::string				defFile;
		std::vector<std::string>	errorPages;
		bool autoIndx;
	public:
		clientInfo();
		~clientInfo();
		
		void					clearReqChunk();
		std::string 			getReqFull() const;
		void					setReqFull(std::string cnt);
		std::string 			getReqChunk() const;
		void					setReqChunk(std::string cnt);
		std::string 			getRoot();
		void					setRoot(std::string str);
		Request 				&getReq();
		void					setReq(Request reqst);
		Response 				&getRes();
		void					setRes(Response resp);
		size_t	 				getBodySize() const;
		void					setBodySize(size_t bodySize);
		std::vector<Location>	getLoc() const;
		void					setErrorPages(std::vector<std::string> erPages);
		std::vector<std::string> getErrorPages() const;
		void					setLoc(std::vector<Location> loc);
		void					setAutoIndx(bool ai);
		bool					getAutoIndx() const;
		void					setDefFile(std::string defF);
		std::string				getDefFile() const;
};
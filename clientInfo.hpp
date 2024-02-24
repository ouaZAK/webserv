/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asidqi <asidqi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/02/24 14:32:34 by asidqi           ###   ########.fr       */
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
		int						port;
		std::string 			reqFull;
		std::string 			servNam;
		std::string 			reqChunk;
		std::string 			root;
		std::string 			host;
		std::vector<Location>	locationVec;
		Request					req;
		Response				res;
		size_t					bodySize;
		std::string				defFile;
		std::string				globDefFile;
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
		void					setGlobDefFile(std::string defF);
		std::string				getGlobDefFile() const;
		void					setPort(int p);
		int						getPort();
		void					setHost(std::string hst);
		std::string				getHost();
		std::string				getServerNam() const;
		void				setServerNam(std::string);
};
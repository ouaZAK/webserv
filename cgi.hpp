/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asidqi <asidqi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:59:25 by asidqi            #+#    #+#             */
/*   Updated: 2024/02/23 19:15:02 by asidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
// #include "Response.hpp"
#include "Request.hpp"
#include "clientInfo.hpp"

class slowCgi
{
private: 
	std::map<std::string, std::string> 	cgiElem;
	Request 							nca;
	// std::string							reqBody;
	

public:
	slowCgi();
	slowCgi(clientInfo &);
	slowCgi(slowCgi const &other);
	~slowCgi();
	char	**mapToChars(std::map<std::string, std::string> inputMap);
	std::string slowCgiExecute(clientInfo &clientMap);
};
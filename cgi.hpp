/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 16:59:25 by asidqi            #+#    #+#             */
/*   Updated: 2024/03/04 12:55:39 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
// #include "Response.hpp"
#include "webserv.hpp"

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


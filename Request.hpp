/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:27:27 by hcharia           #+#    #+#             */
/*   Updated: 2024/04/22 12:42:01 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>

#define OK 200
#define CREATED 201
#define NO_CONTENT 204
#define MOVED_PERM 301
#define BAD_REQUEST 400
#define FORBIDDEN 403
#define NOT_FOUND 404
#define NOT_ALLOWED 405
#define CONFLICT 409
#define REQ_LARGE 413
#define REQ_URL_L 414
#define IN_SERV_ERR 500
#define NOT_IMPLIMENTED 501

class Request
{
private:
	int									status;
	std::string							method;
	std::string							path;
	std::string							version;
	std::map<std::string, std::string>	headers;
	std::string							postBody;
	std::string							query;
	std::string							orig_query;
public:
	Request();
	Request(std::string input);
	
	void								prl(std::string);
	std::string 						get_method();
	std::string							get_path();
	std::string							get_version();
	std::map<std::string, std::string>	get_headers();
	int									get_status();
	std::string							get_body();
	void								set_body(std::string&);
	std::string							get_file_name();
	void								set_status(int);
	void								set_query();
	std::string							get_query();
	std::string							get_oquery();
	std::string getCleanBody();
	~Request();
};

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:27:27 by hcharia           #+#    #+#             */
/*   Updated: 2024/01/29 16:58:48 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <map>

class Request
{
private:
	int									status;
	std::string							method;
	std::string							path;
	std::string							version;
	std::map<std::string, std::string>	headers;
	std::string							postBody;
public:
	Request();
	Request(std::string input);
	
	void								prl(std::string line);
	std::string 						get_method();
	std::string							get_path();
	std::string							get_version();
	std::map<std::string, std::string>	get_headers();
	int									get_status();
	std::string							get_body();
	void								set_body(std::string&);
	std::string							get_file_name();
	~Request();
};

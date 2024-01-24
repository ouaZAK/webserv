/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 11:27:27 by hcharia           #+#    #+#             */
/*   Updated: 2024/01/21 15:47:04 by zouaraqa         ###   ########.fr       */
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
	std::string							postmessage;
	Request();
public:
	Request(std::string input);
	void								prl(std::string line);
	std::string 						get_method();
	std::string							get_path();
	std::string							get_version();
	std::map<std::string, std::string>	get_headers();
	int									get_status();
	std::string							get_body();
	~Request();
};

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 10:29:22 by hcharia           #+#    #+#             */
/*   Updated: 2024/01/31 16:52:03 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include <vector>

class Response
{
private:
	std::string	res;
	std::string	head;
	std::string	error_body;
	std::string success_body;
	std::vector<std::string> headers;
	// map statusstr statusnbr 

public:
	Response();
	Response(Request &, const std::string);
	void	get_res(Request &r);
	void	set_body(Request &r);
	void	set_headers(Request &r);
	void	set_head(Request &r, std::string);
	~Response();
};


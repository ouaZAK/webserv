/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hcharia < hcharia@student.1337.ma>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 10:29:22 by hcharia           #+#    #+#             */
/*   Updated: 2024/02/14 11:18:10 by hcharia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include <vector>

class Response
{
private:
	std::string	responseHead;
	std::string	error_body;
	std::vector<std::string> headers;
	std::string html_error;
public:
	Response();
	Response(Request);
	Response(Request req, std::string url);
	void	set_html_err(Request &r);
	// void	set_OKheads(Request r);
	void	set_head(Request &r, std::string);
	std::string	getHead() const;
	std::string	getHtmlError() const;
	~Response();
};

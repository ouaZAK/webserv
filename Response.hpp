/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 10:29:22 by hcharia           #+#    #+#             */
/*   Updated: 2024/02/08 18:54:18 by zouaraqa         ###   ########.fr       */
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
	void	set_html_err(Request &r);
	// void	set_OKheads(Request r);
	void	set_head(Request &r, std::string);
	std::string	getHead() const;
	std::string	getHtmlError() const;
	~Response();
};

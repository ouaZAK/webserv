/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:33:34 by hcharia           #+#    #+#             */
/*   Updated: 2024/04/18 14:47:29 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"

void	Response::set_head(std::string s)
{
	responseHead = "HTTP/1.1 " + s + "\r\n";
	// std::cout << responseHead;
	for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); it++)
		responseHead += *it + "\r\n";
		
}

void	Response::set_html_err(Request &r)
{
	if (r.get_status() != OK)
		html_error = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Error page</title>\n</head>\n<body>\n<h1>" + responseHead + "</h1>\n<p>" + error_body + "</p>\n</body>\n</html>\n";
}

Response::Response(Request req)
{
	headers.push_back("Content-Type: text/html");

	if (req.get_status() == OK)
	{
		responseHead = "200 OK";
		set_head(responseHead);
		// set_OKheads(req);
		return ;
	}
	else if (req.get_status() == BAD_REQUEST)
	{
		responseHead = "400 Bad Request";
		error_body = "Your request could not be processed due to malformed syntax. Please check the request format and try again.";
		std::cout << "bad request ?? @@@@@@@@@@@@@@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@\n";
	}
	else if (req.get_status() == FORBIDDEN)
	{
		responseHead = "403 Forbidden";
		error_body = "You do not have permission to access the requested resource. If you believe this is an error, please contact support.";
	}
	else if (req.get_status() == NO_CONTENT)
	{
		responseHead = "204 No Content";
		error_body = "";
		headers.clear();
	}
	else if (req.get_status() == CREATED)
	{
		responseHead = "201 Created";
		error_body = "";
	}
	else if (req.get_status() == NOT_FOUND)
	{
		responseHead = "404 Not Found";
		error_body = "";
	}
	else if (req.get_status() == NOT_ALLOWED)
	{
		responseHead = "405 Not Allowed";
		error_body = "The HTTP method used is not allowed for the requested resource.";
	}
	else if (req.get_status() == CONFLICT)
	{
		responseHead = "409 Conflict";
		error_body = "The request could not be completed due to a conflict with the current state of the target resource.";
	}
	else if (req.get_status() == REQ_LARGE)
	{
		responseHead = "413 Request Entity Too Large";
		error_body = "The request is larger than the server is willing or able to process. Please reduce the size of your request and try again.";
	}
	else if (req.get_status() == REQ_URL_L)
	{
		responseHead = "414 Request-URI Too Long";
		error_body = "The URI provided was too long for the server to process. Consider reducing the length of the URI.";
	}
	else if (req.get_status() == IN_SERV_ERR)
	{
		responseHead = "500 Internal Server Error";
		error_body = "The server encountered an unexpected condition that prevented it from fulfilling the request.";
	}
	else if (req.get_status() == NOT_IMPLIMENTED)
	{
		responseHead = "501 Not Implemented";
		error_body = "The server does not support the functionality required to fulfill the request. This is the server's default response for unrecognized requests.";
	}
	set_html_err(req);
	set_head(responseHead);
	std::cout << "looook at meeeeeeee yaloahefi" << responseHead << std::endl;
}

Response::Response(Request req, std::string url)
{
	responseHead = "301 Moved Permanently";
	error_body = "";
	headers.push_back("Content-Type: text/html");
	headers.push_back("Location: " + url);
	set_html_err(req);
	set_head(responseHead);
}

std::string Response::delhtml()
{
	return "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Error page</title>\n</head>\n<body>\n<h1>Deleted</h1>\n<p>the file or directory requested has been deleted</p>\n</body>\n</html>\n";
}

std::string	Response::getHead() const
{
	return responseHead;
}

std::string	Response::getHtmlError() const
{
	return html_error;
}

Response::Response()
{
}

Response::~Response()
{
	
}

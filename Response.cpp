/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 11:33:34 by hcharia           #+#    #+#             */
/*   Updated: 2024/01/31 16:53:11 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"

void	Response::set_headers(Request &r)
{
	
}

void	Response::set_head(Request &r, std::string s)
{
	head = r.get_version() + " " + s + "\r\n";
}

// void	Response::get_res(Request &r)
// {
	
/*	http/1.1 + statusstatus + statusstr + \r\n
	contentlength: + cnt + \r\n
	contenttype: + type \r\n
	\r\n
	+ contentBody
*/
	/*"<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hello World Page</title>
</head>
<body>

    <h1>" + ErrorMsg + "</h1>

</body>
</html>"

}
*/

Response::Response(Request & req, const std::string s)
{
	headers.push_back("Content-Type: text/plain\r\n");
	
	if (req.get_status() == OK)
	{
		head = "200 OK";
	}
	else if (req.get_status() == BAD_REQUEST)
	{
		head = "400 Bad Request";
		error_body = "Your request could not be processed due to malformed syntax. Please check the request format and try again.";
		
	}
	else if (req.get_status() == FORBIDDEN)
	{
		head = "403 Forbidden";
		error_body = "You do not have permission to access the requested resource. If you believe this is an error, please contact support.";
	}
	else if (req.get_status() == NO_CONTENT)
	{
		head = "204 No Content";
		error_body = "";
		headers.clear();
	}
	// else if (req.get_status() == MOVED_PERM)
	// {
	// 	head = "301 Moved Permanently";
	// 	error_body = "The requested resource has been permanently moved to a new URL: http://www.example.com/newpage.";
	// }
	else if (req.get_status() == CREATED)
	{
		head = "201 Created";
		error_body = "";
	}
	else if (req.get_status() == NOT_FOUND)
	{
		head = "404 Not Found";
		error_body = "";
	}
	else if (req.get_status() == NOT_ALLOWED)
	{
		head = "405 Not Allowed";
		error_body = "The HTTP method used is not allowed for the requested resource.";
	}
	else if (req.get_status() == CONFLICT)
	{
		head = "409 Conflict";
		error_body = "The request could not be completed due to a conflict with the current state of the target resource.";
	}
	else if (req.get_status() == REQ_LARGE)
	{
		head = "413 Request Entity Too Large";
		error_body = "The request is larger than the server is willing or able to process. Please reduce the size of your request and try again.";
	}
	else if (req.get_status() == REQ_URL_L)
	{
		head = "414 Request-URI Too Long";
		error_body = "The URI provided was too long for the server to process. Consider reducing the length of the URI.";
	}
	else if (req.get_status() == IN_SERV_ERR)
	{
		head = "500 Internal Server Error";
		error_body = "The server encountered an unexpected condition that prevented it from fulfilling the request.";
	}
	else if (req.get_status() == NOT_IMPLIMENTED)
	{
		head = "501 Not Implemented";
		error_body = "The server does not support the functionality required to fulfill the request. This is the server's default response for unrecognized requests.";
	}
	
	set_head(req, head);
}

Response::Response()
{
	
}

Response::~Response()
{
	
}

/*
HTTP/1.1 200 OK\r\n
Server: nginx/1.25.2\r\n
Date: Mon, 29 Jan 2024 11:14:08 GMT\r\n
Content-Type: text/html\r\n
Content-Length: 615\r\n
Last-Modified: Fri, 26 Jan 2024 14:42:22 GMT\r\n
Connection: keep-alive\r\n
ETag: "65b3c4ce-267"\r\n
Accept-Ranges: bytes\r\n
\r\n
<!DOCTYPE html>
<html>
<head>
<title>Welcome to nginx!</title>
<style>
html { color-scheme: light dark; }
body { width: 35em; margin: 0 auto;
font-family: Tahoma, Verdana, Arial, sans-serif; }
</style>
</head>
<body>
<h1>Welcome to nginx!</h1>
<p>If you see this page, the nginx web server is successfully installed and
working. Further configuration is required.</p>

<p>For online documentation and support please refer to
<a href="http://nginx.org/">nginx.org</a>.<br/>
Commercial support is available at
<a href="http://nginx.com/">nginx.com</a>.</p>

<p><em>Thank you for using nginx.</em></p>
</body>
</html>
*/
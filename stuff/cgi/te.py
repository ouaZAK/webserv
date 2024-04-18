#!/usr/bin/env python3

import sys

# Print HTTP header
# print("Content-Length: 59\r")
# upload status code, delete, query
print("Content-type: text/html\r\n")

print("<html>")
print("<head>")
print("<title>Appended Text</title>")
print("</head>")
print("<body>")

print("<h2>im CGI:</h2>")
print("<h1>Done</h1>")

# Read and print the body of the request
print("<h3>Request Body:</h3>")
request_body = sys.stdin.read()
print("<pre>%s</pre>" % request_body)

print("<a href=\"javascript:history.back()\">Go Back</a>")
print("</body>")
print("</html>")

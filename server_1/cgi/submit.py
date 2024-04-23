#!/usr/bin/env python

import cgi

# Print HTTP header
print("Content-type: text/html\n")

# Get form data
form = cgi.FieldStorage()

# Get the values of the fields
username = form.getvalue('username')
password = form.getvalue('password')
data = form.getvalue('data')

# Print HTML response
print("<html>")
print("<head>")
print("<title>Form Submission Result</title>")
print("</head>")
print("<body>")

print("<h2>im CGI:</h2>")
print("<h1>Done</h1>")

print("<h2>Form Submission Result</h2>")
print("<p>Username: %s</p>" % username)
print("<p>Password: %s</p>" % password)
print("<p>Additional Data:</p>")
print("<pre>%s</pre>" % data)
print("</body>")
print("</html>")

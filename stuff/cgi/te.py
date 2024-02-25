#!/usr/bin/env python3

# Importing necessary modules
import cgi

# Define the content type as HTML
print("Content-Type: text/html\n")

# HTML content
html_content = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CGI Generated Page</title>
</head>
<body>
    <h1>CGI Generated Page</h1>
    <p>This is a dynamically generated HTML page using Python CGI.</p>
    <p>Below is a list of query parameters passed to this script:</p>
    <ul>
"""

# Get query parameters
form = cgi.FieldStorage()
for key in form.keys():
    html_content += f"<li><strong>{key}:</strong> {form[key].value}</li>"

# Closing HTML content
html_content += """
    </ul>
</body>
</html>
"""

# Print the HTML content
print(html_content)

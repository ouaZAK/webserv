#!/usr/bin/env python3

import sys

# Read the string from standard input
text = sys.stdin.read().strip()

# Print header
print("Content-Type: text/html")
print()

# HTML response
print("""
<!DOCTYPE html>
<html>
<head>
    <title>CGI Python Script Response</title>
</head>
<body>
    <h1>Text Received:</h1>
    <p>{}</p>
</body>
</html>
""".format(text))
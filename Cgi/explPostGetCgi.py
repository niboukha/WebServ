#!/usr/bin/env python

import cgi
import cgitb
import sys

# Enable CGI error reporting for debugging
cgitb.enable()

# Print the Content-type header indicating HTML response
print("Content-type: text/html\n")

# Check the request method
request_method = os.environ.get('REQUEST_METHOD', '')

if request_method == 'GET':
    # For a GET request, retrieve query parameters from the URL query string
    query_string = os.environ.get('QUERY_STRING', '')
    params = cgi.parse_qs(query_string)
elif request_method == 'POST':
    # For a POST request, retrieve form data from standard input
    form = cgi.FieldStorage()
    params = {key: form.getvalue(key) for key in form.keys()}
else:
    # Unsupported request method
    print("<h1>Error: Unsupported request method</h1>")
    sys.exit(1)

# Retrieve values from the query parameters
name = params.get('name', 'Anonymous')
age = params.get('age', 'Unknown')

# Generate HTML response
html_content = f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>CGI Example</title>
</head>
<body>
    <h1>Hello, {name}!</h1>
    <p>You are {age} years old.</p>
</body>
</html>
"""

# Print the HTML content as the response
print(html_content)

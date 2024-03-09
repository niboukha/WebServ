#!/usr/bin/env python

import cgi
import cgitb

# Enable CGI error reporting for debugging
cgitb.enable()

# Print the Content-type header indicating HTML response
print("Content-type: text/html\n")

# Read form data from the request
form = cgi.FieldStorage()

# Check if the form data contains the 'name' field
if 'name' in form:
    # Retrieve the value of the 'name' field
    name = form['name'].value
else:
    # Default value if 'name' field is not present
    name = 'Anonymous'

# Check if the form data contains the 'age' field
if 'age' in form:
    # Retrieve the value of the 'age' field
    age = form['age'].value
else:
    # Default value if 'age' field is not present
    age = 'Unknown'

# Generate HTML response
html_content = f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>CGI POST Example</title>
</head>
<body>
    <h1>Hello, {name}!</h1>
    <p>You are {age} years old.</p>
</body>
</html>
"""

# Print the HTML content as the response
print(html_content)

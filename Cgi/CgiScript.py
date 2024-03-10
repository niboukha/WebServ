#!/usr/bin/env python3

import cgi
import http.cookies
import os

# Function to set a cookie
def set_cookie(username):
    cookie = http.cookies.SimpleCookie()
    cookie["username"] = username
    cookie["username"]["path"] = "/"
    cookie["username"]["expires"] = 3600  # Cookie expires in 1 hour
    print(cookie)
    print("Content-type: text/html")
    print()
    print("<html><body>")
    print("<h1>Cookie Set Successfully!</h1>")
    print("</body></html>")

# Function to retrieve the username from the cookie
def get_username_from_cookie():
    cookie_str = os.environ.get("HTTP_COOKIE")
    if cookie_str:
        cookie = http.cookies.SimpleCookie(cookie_str)
        username = cookie.get("username")
        if username:
            return username.value
    return None

# Main CGI script
form = cgi.FieldStorage()

# If the form has been submitted with a username, set the cookie
if "username" in form:
    username = form.getvalue("username")
    set_cookie(username)
else:
    # Otherwise, check if a cookie exists and retrieve the username
    username = get_username_from_cookie()

# Print the HTML content
print("Content-type: text/html")
print()
print("<html><body>")
print("<h1>Cookie CGI Example</h1>")
if username:
    print("<p>Welcome back, {}!</p>".format(username))
    print("<p><a href=''>Refresh</a></p>")
else:
    print("<form action='/cgi-bin/cookie_example.py' method='post'>")
    print("Enter your ue: <input type='text' name='username'><br>")
    print("<input type='submit' value='Submit'>")
    print("</form>")
print("</body></html>")

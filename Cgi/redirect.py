import cgi
import cgitb
import http.cookies

# enable error reporting
cgitb.enable()

# get the form data submitted by the user
form = cgi.FieldStorage()

# get the value of the "name" parameter
name = form.getvalue("name")

# create a new cookie
cookie = http.cookies.SimpleCookie()
cookie["name"] = name

print("Status: 302 Found")
print("Location: /")
print()


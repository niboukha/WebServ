# WebServ

WebServ is a custom HTTP/1.1 web server implemented in C++98. It is designed to handle multiple client connections concurrently using I/O multiplexing with `select()`. The server can serve static files, handle dynamic content through CGI scripts (Python & PHP), and manage file uploads and deletions. Its behavior is highly customizable through a dedicated configuration file, allowing for the setup of multiple virtual servers.

## Features

- **HTTP/1.1 Core Compliance**: Supports `GET`, `POST`, and `DELETE` requests.
- **I/O Multiplexing**: Employs `select()` to handle multiple client connections without blocking or multi-threading.
- **Flexible Configuration**: Parses a detailed configuration file to define server behavior, including multiple virtual servers, custom error pages, and location-specific rules.
- **Static Content**: Serves various static files (HTML, CSS, images, etc.) with correct MIME types.
- **CGI Execution**: Interfaces with CGI scripts (Python 3 and PHP) to generate dynamic web pages.
- **File Management**:
    - **Uploads**: Handles file uploads via POST requests to specified directories.
    - **Deletions**: Processes DELETE requests to remove files or directories from the server.
- **Directory Listing**: Automatically generates an index page for directories when `autoindex` is enabled.
- **Redirection**: Supports HTTP redirection through the `return` directive.
- **Error Handling**: Provides default and customizable pages for various HTTP status codes (e.g., 404 Not Found, 500 Internal Server Error).

## Project Structure

The project is organized into several modules, each with a specific responsibility:

- **`Multiplexer`**: The core event loop that monitors file descriptors (sockets) for readability and writability using `select()`.
- **`Server`**: Manages server sockets, including creation, binding, listening, and accepting new client connections.
- **`Client`**: Represents a connected client, managing its state, request buffer, and the overall request-response cycle.
- **`ConfigFile`**: Responsible for parsing and validating the server configuration file.
- **`Request`**: Parses incoming HTTP requests, including the request line, headers, and URI.
- **`Response`**: Constructs and sends HTTP responses based on the request and server configuration.
- **`Get` / `Post` / `Delete`**: Contain the specific logic for handling `GET`, `POST`, and `DELETE` methods.
- **`Cgi`**: Manages the lifecycle of CGI script execution, including setting up environment variables, piping data, and capturing the output.
- **`configurationFiles`**: Contains the default server configuration, MIME type mappings, and default error pages.
- **`site`**: A sample website directory for testing the server's functionality.

## Getting Started

### Prerequisites

- A C++98 compliant compiler (e.g., g++, clang++)
- `make`

### Compilation

Clone the repository and compile the project using the provided Makefile:

```bash
git clone https://github.com/niboukha/WebServ.git
cd WebServ
make
```

This will create an executable named `webserv` in the root directory.

### Usage

You can run the server with the default configuration or provide a path to a custom one.

**To run with the default configuration:**
The server will use `configurationFiles/webserv.config` by default.

```bash
./webserv
```

**To run with a custom configuration file:**

```bash
./webserv [path/to/your.config]
```

The server will then be running and listening for connections on the ports specified in the configuration file. You can access it from your web browser (e.g., `http://localhost:9090`).

## Configuration File

The server's behavior is controlled by a configuration file. The syntax is similar to NGINX. A server is defined within a `server` block, and its routing rules are managed by `location` blocks.

### Example Configuration:

```nginx
server
	host 0.0.0.0
	server_name serv1
	port 9090
	client_max_body_size 24
	
	location /
		root    ./
		allow_methodes GET POST DELETE
		autoindex on
		cgi_pass .php
		upload_pass /path/to/upload/directory

	location /a/
		root    /path/to/site/
		allow_methodes GET
		index index.html
```

### Server Directives

- `host`: The IP address to bind to.
- `port`: The port to listen on.
- `server_name`: The virtual server name.
- `client_max_body_size`: The maximum allowed size for a request body.
- `error_page`: Defines a custom page for a specific HTTP error code.

### Location Directives

- `root`: The root directory for requests matching the location.
- `allow_methodes`: A list of allowed HTTP methods for this location.
- `index`: The default file to serve when a directory is requested.
- `autoindex`: If set to `on`, lists directory contents if no index file is found.
- `cgi_pass`: Specifies the extension of CGI scripts to be executed (e.g., `.py`, `.php`).
- `upload_pass`: The directory where uploaded files are stored.
- `return`: Configures a redirection (e.g., `return 301 http://new-location.com`).

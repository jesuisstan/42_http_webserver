# 42_http_webserver
## Foreword
This is one of the Ecole 42 common core projects.\
It is about implementing a simple http web server in c++ in team of 3 students.\
The project is written with the C++98 standard and provides good understanding of http protocol, basic html and threading.

## Description
The final 42_http_webserver includes:
- Compatibility with the Chrome and Mozilla web browsers.
- HTTP response status codes is accurate.
- Server has default error pages if none are provided.
- Clients are able to upload files.
- Implemented GET, POST and DELETE methods.
- Server is able to listen to multiple ports.

Full project description you can find in `en.subject.pdf`.

## Requirements
- installed `clang++`
- installed `Makefile`

## How to use
In project directory:
1. Run `make` to compile the project.
2. Execute `./webserv` to run 42_http_webserver.
3. Proceed to `http://127.0.0.1:8888/` to test 42_http_webserver.
4. Run `make fclean` to delete all created files.

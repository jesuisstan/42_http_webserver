#!/usr/bin/env python3
import os

CONTENT_TEXT = "Content-type:text/html\r\n\r\n"
CONTENT_STATUS = "Status: 200 OK\
Content-Type: text/html; charset=utf-8\r\n\r\n"
CONTENT_HTML = "Content-type:text/html\r\n\r\n"
MAIN_PAGE = """
	<html>
	<head>
	<title>21</title>
	</head>
	<body>
	<h2>cgi cyberserv, yeaah!</h2>
	</body>
	</html>
"""
line = input()
print(CONTENT_STATUS)

# todo print all enviroment
for key in sorted(os.environ):
	print(key, "=", f'|{os.environ.get(key)}|')

with open("py_cgi_out.txt", "w") as f:
	for key in sorted(os.environ):
		f.write(f"{key} = |{os.environ.get(key)}|{len(os.environ.get(key))}\n")
exit(0);
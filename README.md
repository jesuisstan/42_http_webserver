# http_webserver
implementation of a simple http web server in c++
## Foreword
This is one of the Ecole 42 common core projects.\
It is about implementing a simple shell (bash) in team of 3 students.\
The project is written with holly C language and provides good understanding of processes, file descriptors and popular bash commands.

## Description
The final 42_minishell includes:
- Show a prompt when waiting for a new command.
- Have a working History.
- Search and launch the right executable (based on the PATH variable)
- Implemented the builtins:
1. `echo` with option -n
2. `cd`
3. `pwd` with no options
4. `export` with no options
5. `unset` with no options
6. `env` with no options or arguments
7. `exit` with no options
- `’` inhibit all interpretation of a sequence of characters.
- `"` inhibit all interpretation of a sequence of characters except for $.
- Redirections:
1. `<` redirects input.
2. `>` redirects output.
3. `<<` reads input from the current source until a line containing only the delimiter is seen.
4. `>>` redirects output with append mode.
- Pipes | The output of each command in the pipeline is connected via a pipe to the input of the next command.
- Environment variables ($ followed by characters) expands to their values.
- `$?` expands to the exit status of the most recently executed foreground pipeline.
- `ctrl-C` `ctrl-D` `ctrl-\` works like in bash.

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
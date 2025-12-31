# Simple Shell 🐚

A simple UNIX command line interpreter - Holberton School Project

## Description 📝

This project is a simple implementation of a UNIX command line interpreter (shell) written in C. It replicates basic functionalities of the `sh` shell, allowing users to execute commands in both interactive and non-interactive modes. The shell reads commands from standard input, parses them, and executes them using system calls.

## Features ✨

- Display a prompt and wait for user input
- Execute commands with their complete path (e.g., `/bin/ls`)
- Execute commands found in the PATH environment variable
- Handle command line arguments
- Implement built-in commands: `exit` and `env`
- Handle the PATH environment variable
- Handle End-Of-File (EOF) condition (Ctrl+D)
- Handle errors and display appropriate error messages
- Non-interactive mode support

## Environment 🖥️

- **OS**: Ubuntu 20.04 LTS
- **Compiler**: gcc
- **Compilation flags**: `-Wall -Werror -Wextra -pedantic -std=gnu89`
- **Style Guidelines**: Betty style

## Installation 🔧

Clone the repository:

```bash
git clone https://github.com/AbdullahAldaghaym/holbertonschool-simple_shell.git
cd holbertonschool-simple_shell
```

## Compilation 🔨

Compile all `.c` files in the repository:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Usage 💻

### Interactive Mode

Run the shell:

```bash
./hsh
```

The shell will display a prompt `($)` and wait for your commands:

```bash
$ ./hsh
($) ls
file1 file2 file3
($) pwd
/home/user/simple_shell
($) echo "Hello World"
Hello World
($) exit
$
```

### Non-Interactive Mode

You can also pipe commands into the shell:

```bash
$ echo "/bin/ls" | ./hsh
hsh main.c shell.c
$
```

Or run commands from a file:

```bash
$ cat test_file
/bin/ls
/bin/pwd
$ cat test_file | ./hsh
hsh main.c shell.c test_file
/home/user/simple_shell
$
```

## Built-in Commands 🛠️

### exit

Exits the shell program.

**Usage:**
```bash
($) exit
```

You can also exit with a specific status:
```bash
($) exit [status]
```

### env

Prints all environment variables.

**Usage:**
```bash
($) env
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
HOME=/home/user
USER=user
...
($)
```

## Supported Commands 📋

The shell can execute any command found in the directories listed in the PATH environment variable, including:

- `ls` - List directory contents
- `pwd` - Print working directory
- `cat` - Concatenate and display files
- `echo` - Display a line of text
- `mkdir` - Create directories
- `rm` - Remove files or directories
- `cp` - Copy files
- `mv` - Move files
- And many more...

## Examples 💡

### Example 1: Basic Commands

```bash
$ ./hsh
($) ls -l
total 100
-rw-rw-r-- 1 user user  1234 Dec 31 10:00 main.c
-rw-rw-r-- 1 user user  5678 Dec 31 10:01 shell.c
($) pwd
/home/user/simple_shell
($)
```

### Example 2: Using Absolute Paths

```bash
($) /bin/echo "Testing absolute path"
Testing absolute path
($) /bin/ls
main.c shell.c hsh
($)
```

### Example 3: Environment Variables

```bash
($) env
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
HOME=/home/user
SHELL=/bin/bash
($)
```

## Error Handling ⚠️

The shell handles various error conditions:

- **Command not found:**
```bash
($) invalid_command
./hsh: 1: invalid_command: not found
($)
```

- **Permission denied:**
```bash
($) /root/private_file
./hsh: 1: /root/private_file: Permission denied
($)
```

## Testing 🧪

### Memory Leak Testing with Valgrind

Test for memory leaks:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./hsh
```

Expected output:
```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: X allocs, X frees, Y bytes allocated
==12345== 
==12345== All heap blocks were freed -- no leaks are possible
```

### Betty Style Testing

Check code style compliance:

```bash
betty *.c *.h
```

## File Structure 📁

```
holbertonschool-simple_shell/
├── shell.h              # Header file with function prototypes
├── main.c              # Main function and program entry point
├── execute.c           # Command execution functions
├── builtins.c          # Built-in command implementations
├── path.c              # PATH handling functions
├── string_utils.c      # String manipulation utilities
├── memory_utils.c      # Memory management functions
├── AUTHORS             # List of project contributors
├── README.md           # This file
└── man_1_simple_shell  # Manual page
```

## How It Works 🔄

1. **Initialization**: The shell sets up the environment and displays the prompt
2. **Reading Input**: Uses `getline()` to read user input from stdin
3. **Parsing**: Tokenizes the input string using `strtok()` to separate command and arguments
4. **Command Lookup**: Searches for the command in PATH directories if not an absolute path
5. **Execution**: Creates a child process using `fork()` and executes the command with `execve()`
6. **Waiting**: Parent process waits for child completion using `wait()`
7. **Loop**: Returns to step 2 and displays prompt again

## System Calls & Functions Used 📚

- `execve` - Execute a program
- `fork` - Create a child process
- `wait` / `waitpid` - Wait for process to change state
- `getline` - Read a line from stream
- `strtok` - Tokenize strings
- `malloc` / `free` - Memory allocation and deallocation
- `stat` - Get file status
- `access` - Check file accessibility
- `write` - Write to file descriptor
- `perror` - Print error message

## Limitations 🚫

This simple shell does **NOT** support:

- Piping (`|`)
- Redirections (`>`, `<`, `>>`)
- Logical operators (`&&`, `||`)
- Command separators (`;`)
- Wildcard characters (`*`, `?`)
- Background processes (`&`)
- Aliases
- Variables
- Comments (`#`)
- Special characters (quotes, backticks)

## Man Page 📖

View the manual page:

```bash
man ./man_1_simple_shell
```

## 👥 Authors

<div align="center">

### **Project Developers**

| **Mustafa Alalmi** | **Abdullah Aldaghaym** |
|:----------------:|:----------------------:|
| [![GitHub](https://img.shields.io/badge/📁_GitHub-Profile-black?style=for-the-badge&logo=github)](https://github.com/i1SaSa) | [![GitHub](https://img.shields.io/badge/📁_GitHub-Profile-black?style=for-the-badge&logo=github)](https://github.com/abdullahaldaghaym) |
| **C Developer** | **C Developer** |

</div>


## Resources 📚

- [Unix Shell - Wikipedia](https://en.wikipedia.org/wiki/Unix_shell)
- [Thompson Shell](https://en.wikipedia.org/wiki/Thompson_shell)
- [Ken Thompson](https://en.wikipedia.org/wiki/Ken_Thompson)
- [Everything you need to know to start coding your own shell](https://www.geeksforgeeks.org/making-linux-shell-c/)

## Acknowledgments 🙏

This project was created as part of the curriculum at Holberton School. Special thanks to all peers and mentors who provided guidance and support throughout the development process.

## License 📄

This project is part of the Holberton School curriculum.

---

**Holberton School - Simple Shell Project** 🎓

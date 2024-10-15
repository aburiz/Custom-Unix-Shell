# Custom Unix Shell - COEN 346 Project

## Overview

This project implements a custom Unix-like shell as part of the COEN 346 Operating Systems course. The shell supports basic command execution, background job control, input/output redirection, and piping. It is written in C and follows the specifications provided in the assignment document.

The key features include:

- **Basic command execution** (e.g., `echo`, `ls`, etc.)
- **Background execution** using `&` (e.g., `./sleeptest &`)
- **Input/output redirection** (e.g., `cat < input.txt`, `echo hello > output.txt`)
- **Piping** between commands (e.g., `cat file.txt | sort`)
- **Job control** (list and manage background jobs)

## Features

### 1. **Basic Command Execution**

The shell allows you to run basic commands, similar to a Unix terminal. For example:

```bash
echo "Hello, World!"
```

The shell will fork a new process to execute the command and wait for it to finish unless run in the background.

### 2. **Background Execution**

You can run a command in the background by appending `&` at the end:

```bash
./sleeptest &
```

This will execute the command in the background, allowing the shell to continue accepting input.

### 3. **Job Control**

Background jobs can be listed using the `jobs` command, which will display the process ID and command of each background process:

```bash
jobs
```

### 4. **Input and Output Redirection**

You can redirect input and output to and from files:

- Input redirection: `command < input_file`
- Output redirection: `command > output_file`

For example:

```bash
echo "COEN 346" > output.txt
cat < output.txt
```

### 5. **Pipes**

The shell supports simple and multiple pipes to connect the output of one command to the input of another:

```bash
cat records.txt | sort | uniq
```

### 6. **Job Management**

You can manage background jobs, list them using the `jobs` command, and wait for all background jobs to complete using the `wait` command.

## Project Structure

The project is composed of several files:

- `main.c`: Contains the main loop of the shell, handles user input, command execution, background jobs, and pipes.
- `parser.c`: Implements the command parser, including parsing for input/output redirection, background execution, and pipes.
- `parser.h`: Header file for command parsing, declarations of functions and data structures used across the project.
- `sleeptest.c`: A test program to simulate a command that runs for a specified number of seconds, used for testing background execution.
- `CMakeLists.txt`: The CMake build script for compiling the project.

### Files

- **`main.c`**: Contains the main shell loop that reads input, parses commands, handles job control, and executes commands.
- **`parser.c`**: Implements parsing logic for commands, input/output redirection, background execution, and pipes.
- **`parser.h`**: Header file defining the structure of the `cmdline` and declarations for parsing functions.
- **`sleeptest.c`**: A small utility program that sleeps for a specified number of seconds, useful for testing background jobs.

## Compilation and Running

### Prerequisites

- **GCC** or **Clang** for compiling C programs
- **CMake** for building the project

### Steps

1. Clone the repository:

```bash
git clone https://github.com/aburiz/Custom-Unix-Shell.git
cd Custom-Unix-Shell
```

2. Build the project using CMake:

```bash
mkdir build
cd build
cmake ..
make
```

3. Run the shell:

```bash
./shell
```

4. You can also compile the `sleeptest` program to simulate long-running background processes:

```bash
gcc sleeptest.c -o sleeptest
```

5. Run `sleeptest`:

```bash
./sleeptest &
```

### Testing Features

- **Basic Commands**:
    ```bash
    echo "Hello, COEN 346!"
    ```

- **Background Job**:
    ```bash
    ./sleeptest &
    ```

- **Job Control**:
    ```bash
    jobs
    ```

- **Redirection**:
    ```bash
    echo "Hello, World!" > output.txt
    cat < output.txt
    ```

- **Pipes**:
    ```bash
    cat records.txt | sort | uniq
    ```

## Sample Output

```bash
my_shell> echo "COEN 346"
COEN 346
my_shell> ./sleeptest 10 &
Running in background: PID 12345
my_shell> jobs
[1] 12345 ./sleeptest
my_shell> cat records.txt | sort
```

## Future Improvements

- **Signal Handling**: Handle `Ctrl+C` to kill foreground processes.
- **History**: Add support for command history.
- **Built-in Commands**: Extend built-in commands like `cd`, `exit`, etc.

## Acknowledgments

This project is part of the COEN 346 Operating Systems course at Concordia University.

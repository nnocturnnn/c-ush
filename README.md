# ush - Unix Shell

`ush` is a custom Unix shell designed to provide a rich set of built-in commands and functionalities. This shell aims to offer enhanced features while maintaining simplicity and efficiency.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Builtin Commands](#builtin-commands)
  - [cd](#cd)
  - [pwd](#pwd)
  - [export](#export)
  - [unset](#unset)
  - [fg](#fg)
  - [env](#env)
  - [which](#which)
  - [echo](#echo)
  - [exit](#exit)
  - [jobs](#jobs)
  - [return](#return)
  - [True](#true)
  - [False](#false)
  - [history](#history)
- [Contributing](#contributing)

## Installation

To install `ush`, follow these steps:

1. Clone the repository:

    ```sh
    git clone https://github.com/yourusername/ush.git
    ```

2. Navigate to the project directory:

    ```sh
    cd ush
    ```

3. Build the shell:

    ```sh
    make
    ```

4. Run the shell:

    ```sh
    ./ush
    ```

## Usage

Once `ush` is running, you can use it like any other Unix shell. It supports a variety of built-in commands as well as external commands.

## Builtin Commands

### cd

Change the current directory.

- `cd [dir]`: Change to the specified directory.
- Flags:
  - `-i`: Ignore symbolic links.
  - `-P`: Use physical directory structure without following symbolic links.
  - `-u`: Update the environment variables accordingly.

### pwd

Print the current working directory.

- `pwd`: Print the current directory.
- Flags:
  - `-L`: Print the logical current working directory (default).
  - `-P`: Print the physical current working directory without symbolic links.

### export

Set environment variables.

- `export [name=value]`: Export environment variables to the shell's environment.

### unset

Unset environment variables or shell functions.

- `unset [name]`: Unset the specified environment variable or shell function.

### fg

Bring a background job to the foreground.

- `fg [%n | %str]`: Resume the specified job in the foreground.
  - `%n`: Job number.
  - `%str`: Job string.

### env

Display or modify the environment.

- `env`: Display the current environment.
- Flags:
  - `-i`: Start with an empty environment.
  - `-P [path]`: Set the search path for commands.
  - `-u [name]`: Remove the specified environment variable.

### which

Locate a command.

- `which [command]`: Display the path of the command.
- Flags:
  - `-a`: Show all instances of the command in the PATH.
  - `-s`: No output, just return the status.

### echo

Display a line of text.

- `echo [text]`: Print the specified text.
- Flags:
  - `-n`: Do not output the trailing newline.
  - `-e`: Enable interpretation of backslash escapes.
  - `-E`: Disable interpretation of backslash escapes (default).

### exit

Exit the shell.

- `exit [n]`: Exit the shell with a status of `n`. If `n` is omitted, the exit status is that of the last command executed.

### jobs

List active jobs.

- `jobs`: Display the status of jobs in the current session.

### return

Return from a function.

- `return [n]`: Return with a status of `n`. If `n` is omitted, the return status is that of the last command executed.

### True

Do nothing, successfully.

- `True`: Return a successful status.

### False

Do nothing, unsuccessfully.

- `False`: Return an unsuccessful status.

### history

Display the command history.

- `history`: Show the history of commands entered.

## Contributing

Contributions are welcome! If you'd like to contribute to `ush`, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Implement your changes.
4. Test your changes thoroughly.
5. Commit and push your changes to your forked repository.
6. Submit a pull request with a detailed description of your changes.

# Dish

A basic shell implemented in C

# Usage

run `make`

and execute `./vsh`

## Assumptions

1. pinfo run on processes for which reading the executable path requires root access (although this is still handled by printing **readlink : Permission denied**
2. The path to the actual home of the user is at max 255 characters

# Features

## Builtin commands

### 1. cd

* cd  implemented as in actual bash shell

### 2. echo

- echo prints the arguments you entered to the command

### 3. pwd

- You can use the `pwd` command to print the absolute path of the directory you are currently in.

## ls

- cd  implemented as in actual bash shell

## Running system commands

**Foreground processes** - You can execute normal commands such as `sleep` or `nvim` with or without arguments as a foreground process. 

*  Ctrl + c teminates
* if it takes more than 2 seconds then it

**Background processes** - By appending `&` you can have a process run in the background.

* You can run multiple processes, and executing a background process .

* ON end prints approoprite info

## pinfo

- pinfo prints information about a certain process.
- If no pid is mentioned, the command runs pinfo on the current shell
- Else it runs on the pid mentioned, giving details such as:
  1. pid number
  2. process status
  3. memory (in kb)
  4. executable path



# discover

custom discover command which emulates the basics of the find command. The searches for files in a directory hierarchy.

* `discover   <target_dir> <typeflags> <filename>`

## history

a ‘history’ command which is similar to the actual history command. The default number of commands it should output is 10. The default number of commands the history should store is 20

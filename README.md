# Simple Shell

## Description
This project is a simple UNIX command line interpreter written in C, created as part of the Holberton School curriculum.  
It provides the basic functionality of a shell: reading user commands, executing them, and handling errors.

The goal is to understand how a real shell works under the hood by building one from scratch using system calls like `fork`, `execve`, and `wait`.

---

## Requirements
- All code files are compiled on **Ubuntu 20.04 LTS** using:
  ```bash
  gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh


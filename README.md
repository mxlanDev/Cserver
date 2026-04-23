
# Cserver

A web server written in the C language, currently supporting multithreading with epoll nonblocking I/O and a file cache on a thread-by-thread basis. This is primarily a passion project I started to eventually self-host a CV website on a physical server.

## Features

- POSIX socket-based I/O operations
- Thread-by-thread epoll setup to manage reading from file descriptors
- 5 worker threads executing a thread loop which receives HTTP requests and forms HTTP replies
- Currently only supporting GET requests, which will change in the future
- Return pages for error HTTP codes (Example: for code 400 write `errorPage400.html` to client)
- Per-thread file cache for serving static content efficiently

## Dependencies

- gcc
- make
- POSIX-compliant OS (Linux/Unix)
- pthreads

## Installation
```bash
git clone https://github.com/exaltdev/Cserver.git
cd Cserver
make
cd build
./server
```
Runs HTTP server on port 8080.

## Functionality

The server creates a listening socket on port 8080 and spawns 5 worker threads. Each thread runs its own epoll instance for nonblocking I/O. The main thread accepts incoming connections with `accept4` using `SOCK_NONBLOCK` and distributes them round-robin across the worker epoll instances.

Each worker thread runs a loop that:

1. Calls `epoll_wait` on its assigned file descriptors
2. Reads the HTTP request from the client
3. Parses the request into an `HttpRequest` struct
4. Forms an `HttpReply` based on the requested resource
5. Checks the per-thread file cache for the requested file
6. Sends HTTP response headers and body

## TODO

Rewrite this project with selfwritten libraries. (This project is currently not being developed).

## License

See LICENSE file for details


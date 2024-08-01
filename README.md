# Cserver

A webserver written in the C language, which currently supports multithreading with blocking I/O and a file cache on a thread by thread basis.<br>
This is primarely a passion project which I started on to eventually selfhost a CV website on a physical server.<br>

# Installation

```bash
make
cd build
./server -Runs HTTP server on port 8080
```

# Current Features:

-POSIX socket based I/O operations.<br>
-Joint file descriptor queue from which threads read HTTP requests (Currently the only blocking point, which will be removed in the future).<br>
-7 worker threads executing a thread loop which receives HTTP requests and forms HTTP replies.<br>
-Currently only supporting GET requests, which will change in the future.<br>
-Return pages for error HTTP codes (Example: for code 400 writes errorPage400.html to client).<br>

# TODO:
-Implement other request types.<br>
-Fully implement header processing instead of hardcoded magic code.<br>
-Remove magic variables entirely from the codebase.<br>
-Introduce config files for various server settings.<br>
-Implement TLS handshake and switch to TLS port.<br>

# Contribution

I will not accept contributions (at least for now), but will periodically check the issues tab for feature requests or found bugs.




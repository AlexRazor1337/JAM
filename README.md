# JAM
Just Another Messenger is an our messenger attempt. It's build in C and works on MacOS and Linux.

We used this libraries:
 - [dyad](https://github.com/rxi/dyad) - all networking is done by this library. It allows us to use event-based programming and does everything asynchronously.
 - [sqlite](https://www.sqlite.org/index.html) - very simple, small and fast database engine which embeds into our server app.
 - [vec](https://github.com/rxi/vec) - "A type-safe dynamic array implementation for C."
 - [json-c](https://github.com/json-c/json-c) - library for easy json handling in C.
 - GTK3 - graphical library that we used to create our client app.
 - [tiny-AES-c](https://github.com/kokke/tiny-AES-c) - AES encryption algorithm for networking(not implemented for now).

## Installation

All libraries except two(gtk3 and json-c) are included with source code.
gtk3 and json-c must be installed separately:  
**apt-get**:  
```apt-get install libgtk-3-dev libjson-c-dev```  
**pacman**:  
```pacman -S gtk3 json-c```  
**dnf**:  
```dnf install gtk3 json-c```  

When everything is installed just call ```make```.

## Usage

Start server, specifying it's port, 8080 for example: ```./uchat_server 8080```
It will daemonize server and print it's new PID.

Then, start client app with ip adress and port as arguments: ```./uchat 127.0.0.1 8080```
It will open or client window, so you can register new account or authorize to existing one(**note:** ```make reinstall``` deletes .db file, so be careful).

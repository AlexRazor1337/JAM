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
```apt-get install libgtk-3-dev json-c``` or ```pacman -S gtk3 json-c```

When everything is installed just call ```make```.

## Usage

TODO

# Documentation

JAM consists of two sides: client app and server.

## Server

Server side uses SQLite as a database to store info about users, messages and files.
<details>
  <summary>DB schema for server side:</summary>

  ![Server Schema](/docs/database_shema_server.png)
</details>


## Client

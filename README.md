# JAM
Just Another Messenger is an our messenger attempt.

## Installation

TODO

## Usage

TODO

# Documentation

JAM consists of two sides: client and server.

## Server

Server side uses SQLite as a database to store info about users, messages and files.
<details>
  <summary>DB schema for server side:</summary>
  ![Server Schema](docs/database_shema_server.png)
</details>


## Client
Client cashes all info in the local database, which is just little simplified version of the server one.
<details>
  <summary>DB schema for client side:</summary>
  ![Server Schema](docs/database_shema_client.png)
</details>
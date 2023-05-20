Copyright 2023 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

# Web Client - Project4

## Description

### Context

The project aims to implement a client meant to communicate with a server based off an REST API.
The client sends HTTP requests regarding access to an online library and parses the server response in order to inform the user about the status of their requested resource.
It manages a string storing the session cookie and a JWT token, created in order for the client to gain secure access to the library, when it wishes to enter it.
Both the cookie and the token are initially set to `NULL` and get changed back to `NULL` when the client logs-out, as the session comes to an end and we shouldn't grant access to the server anymore.

### Program Flow

The main logic behind the client is centered around taking commands indefinitely:

* Open a new connection for every command, in order to avoid getting timeout and blocking the server.

* Parse the input, and decide whether the command is supported by the API.

* Call the function corresponding to the command:

   * `register`

      * Get username and password.

      > **Note**: Both the username and password are taken in a loop in order to avoid having whitespaces in the string introduced by the user.

      * Create the data to be sent to the server, by liniarizing a json object.

      * Print the part of the response containing an error or a string congratulating the user for signing up.

   * `login`

      * Similar with `register`, the only difference being that we save the session cookie for future use.

   * `enter_library`

      * Save the token for future accessing the virtual library.

   * `get_books`

      * Parse the json object sent by the server and pretty print it using the `nlohmann` library.

   * `get_book`

      * Construct the path for the given id.

      * Parse the json object sent by the server.

   * `add_book`

      * Get all the info in order to register a new book.

      > **Note**: We get the page number of the book in a loop in order to avoid having non-int entries.

   * `delete_book`

      * Similar with `get_book`.

   * `logout`

      * Free and place both the cookie and the token on `NULL` in order to deny access for future unauthorized access.

> **Note**: The error messages for the various cases where the user introduced bad info are printed as they are got from the server.
This works really fine, except for the unauthorized access (the message is quite ambiguous for non-technical people), but one has only 24h a day and 5 projects at the same time :woman_shrugging:

### JSON Parser

I've decided to use C++ for this project **only** for the [Nlohmann library](https://github.com/nlohmann/json).
It has better documentation as opposed to the C variant and is relatively easier to use.

The functionalities that were used from the `Nlohmann` library were (to name a few):

```C++
json::parse
json::dump
json::contains
```

### Hierarchy

```console
.
├── buffer.cpp
├── buffer.h
├── client.cpp
├── commands.cpp
├── commands.h
├── helpers.cpp
├── helpers.h
├── Makefile
├── parser.cpp
├── parser.h
├── README.md
├── requests.cpp
├── requests.h
└── utils.h
```

&rarr; `buffer.cpp`, `buffer.h`: helper functions inspired by the [HTTP Protocol lab](https://gitlab.cs.pub.ro/pcom/pcom-laboratoare-public/-/tree/master/lab9), used for working with the buffers sent on network

&rarr; `client.cpp`: driver program

&rarr; `commands.cpp`, `commands.h` functions used for applying requests from users: `auth()`, `login()`, `enter_library()`, the lists goes on 

&rarr; `helpers.cpp`, `helpers.h`: functions ispired by the [HTTP Protocol lab](https://gitlab.cs.pub.ro/pcom/pcom-laboratoare-public/-/tree/master/lab9), used for establishing connections, closing them, sending and receiving data on network

&rarr; `parser.cpp`, `parser.h`: functions used for parsing the line of input introduced by the user

&rarr; `requests.cpp`, `requests.h`: functions used for creating HTTP requests (headers + payload) for GET, POST and DELETE

&rarr; `utils.h`: DIE and other usefull macros

## Observations Regarding the Project

The nicest project I had the "plasure" to work on.
Very thoroughly put together, even though I wish the team either worked on the checker more, or made it public earlier on.

All in all, I wish I did the parsing of the server response prettier and more modularized, but oh well....

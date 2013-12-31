## C-Actors

An object-orinted library implementing the actors model in C.

### Getting started

1. cd into the project directory and run the `scons` build tool

    $ scons

2. Run the example server

    $ ./bin/main_socket 6666

3. Create requests for the server

    $ ./tools/parallel_requests 11 1000 'http://localhost:6666'

### Copyright and license

Copyright 2013 Robert Welin

NoYt is licensed under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).

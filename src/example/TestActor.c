/**
 *  Copyright 2013 Robert Welin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "TestActor.h"

static void TestActor_receive(struct Actor * self, void * data) {
    char buffer[256];
    int socket = (long) data;
    int n;

#ifdef DEBUG
    printf("Actor %d received message '%d', handled by thread %d\n",
            self->id, socket, pthread_self());
#endif

    if (socket < 0) {
        perror("ERROR on accept");
    }

    memset(buffer, 0, 256);
    n = read(socket, buffer, 255);

    if (n < 0) {
        perror("ERROR reading from socket");
    }

    n = write(socket, "HTTP/1.0 200 OK\n\r\n", 17);

    if (n < 0) {
        perror("ERROR writing to socket");
    }

    close(socket);
}

static void * TestActor_constructor(void * _self, va_list * args) {
    struct Actor * self = ((const struct Class *) Actor)->constructor(_self, args);

    self->receive = TestActor_receive;

    return self;
}

static void * TestActor_destructor(void * self) {
    return self;
}

static bool TestActor_equals(void * self, void * other) {
    return self == other;
}

static const struct Class _TestActor = {
    sizeof(struct TestActor),
    TestActor_constructor,
    TestActor_destructor,
    TestActor_equals
};

const void * TestActor = & _TestActor;

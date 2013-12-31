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
#include "FirstActor.h"

static void receive(struct Actor * self, void * data) {
#ifdef DEBUG
    printf("Actor %d received message '%ld', handled by thread %d\n",
            self->id, (long) data, pthread_self());
#endif
}

static void * constructor(void * _self, va_list * args) {
    struct Actor * self = ((const struct Class *) Actor)->constructor(_self, args);

    self->receive = receive;

    return self;
}

static void * destructor(void * self) {
    return self;
}

static bool equals(void * self, void * other) {
    return self == other;
}

static const struct Class _FirstActor = {
    sizeof(struct FirstActor),
    constructor,
    destructor,
    equals
};

const void * FirstActor = & _FirstActor;

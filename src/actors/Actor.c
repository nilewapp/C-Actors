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
#include "Actor.h"

static void Actor_send(struct Actor * self, int receiver, void * data) {
    struct ActorSystem * system = self->system;
    system->send(system, new(Message, self->id, receiver, data));
}

static void * Actor_constructor(void * _self, va_list * args) {
    struct Actor * self = _self;

    self->send = Actor_send;
    self->receive = NULL;

    pthread_mutex_init(& self->mutex, NULL);

    self->system = NULL;
    self->id = -1;

    return self;
}

static void * Actor_destructor(void * _self) {
    struct Actor * self = _self;
    pthread_mutex_destroy(& self->mutex);
    return self;
}

static bool Actor_equals(void * self, void * other) {
    return self == other;
}

static const struct Class _Actor = {
    sizeof(struct Actor),
    Actor_constructor,
    Actor_destructor,
    Actor_equals
};

const void * Actor = & _Actor;

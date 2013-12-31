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
#include "Thread.h"

static int Thread_start(struct Thread * self) {
    return pthread_create(& self->thread, NULL, self->function, self->args);
}

static int Thread_join(struct Thread * self, void ** value_ptr) {
    return pthread_join(self->thread, value_ptr);
}

static int Thread_wait(struct Thread * self, pthread_cond_t * cond) {
    return pthread_cond_wait(cond, & self->mutex);
}

static int Thread_signal(struct Thread * self, pthread_cond_t * cond) {
    return pthread_cond_signal(cond);
}

static void * Thread_constructor(void * _self, va_list * args) {
    struct Thread * self = _self;

    pthread_mutex_init(& self->mutex, NULL);
    self->function = va_arg(* args, void * (*)(void *));
    self->args = va_arg(* args, void *);

    self->start = Thread_start;
    self->join = Thread_join;
    self->wait = Thread_wait;
    self->signal = Thread_signal;
    return self;
}

static void * Thread_destructor(void * _self) {
    struct Thread * self = _self;
    pthread_mutex_destroy(& self->mutex);
    return self;
}

static bool Thread_equals(void * self, void * other) {
    return self == other;
}

static const struct Class _Thread = {
    sizeof(struct Thread),
    Thread_constructor,
    Thread_destructor,
    Thread_equals
};

const void * Thread = & _Thread;

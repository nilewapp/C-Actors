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
#include "SafeQueue.h"

static void * SafeQueue_head(struct SafeQueue * self) {
    pthread_mutex_lock(& self->mutex);
    void * head = self->queue->head(self->queue);
    pthread_mutex_unlock(& self->mutex);
    return head;
}

static bool SafeQueue_pop(struct SafeQueue * self, void ** head) {
    pthread_mutex_lock(& self->mutex);
    bool b = self->queue->pop(self->queue, head);
    pthread_mutex_unlock(& self->mutex);
    return b;
}

static bool SafeQueue_push(struct SafeQueue * self, void * elem) {
    pthread_mutex_lock(& self->mutex);
    bool b = self->queue->push(self->queue, elem);
    pthread_mutex_unlock(& self->mutex);
    return b;
}

static int SafeQueue_size(struct SafeQueue * self) {
    pthread_mutex_lock(& self->mutex);
    int s = self->queue->size(self->queue);
    pthread_mutex_unlock(& self->mutex);
    return s;
}

static bool SafeQueue_is_empty(struct SafeQueue * self) {
    pthread_mutex_lock(& self->mutex);
    bool e = self->queue->is_empty(self->queue);
    pthread_mutex_unlock(& self->mutex);
    return e;
}

static bool SafeQueue_is_full(struct SafeQueue * self) {
    pthread_mutex_lock(& self->mutex);
    bool f = self->queue->is_full(self->queue);
    pthread_mutex_unlock(& self->mutex);
    return f;
}

static void * SafeQueue_constructor(void * _self, va_list * args) {
    struct SafeQueue * self = _self;

    self->head = SafeQueue_head;
    self->pop = SafeQueue_pop;
    self->push = SafeQueue_push;
    self->size = SafeQueue_size;
    self->is_empty = SafeQueue_is_empty;
    self->is_full = SafeQueue_is_full;

    int type = va_arg(* args, int);
    int size = va_arg(* args, int);
    self->queue = new(Queue, type, size);

    pthread_mutex_init(& self->mutex, NULL);

    return self;
}

static void * SafeQueue_destructor(void * _self) {
    struct SafeQueue * self = _self;
    delete(self->queue);
    pthread_mutex_destroy(& self->mutex);
    return self;
}

static bool SafeQueue_equals(void * self, void * other) {
    return self == other;
}

static const struct Class _SafeQueue = {
    sizeof(struct SafeQueue),
    SafeQueue_constructor,
    SafeQueue_destructor,
    SafeQueue_equals
};

const void * SafeQueue = & _SafeQueue;

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
#include "Queue.h"

const int QUEUE_PRIMITIVE = 0;
const int QUEUE_COMPLEX   = 1;

static void * Queue_head(struct Queue * self) {
    if (self->empty) {
        return NULL;
    } else {
        return self->data[self->front_index];
    }
}

static bool Queue_pop(struct Queue * self, void ** head) {
    if (self->empty) {
        * head = NULL;
        return false;
    } else {
        * head = self->data[self->front_index];
        if (self->front_index == self->max_size - 1) {
            self->front_index = 0;
        } else {
            self->front_index++;
        }
        self->empty = self->front_index == self->back_index;
        self->full = false;
        return true;
    }
}

static bool Queue_push(struct Queue * self, void * elem) {
    if (self->full) {
        return false;
    } else {
        self->data[self->back_index] = elem;
        if (self->back_index == self->max_size - 1) {
            self->back_index = 0;
        } else {
            self->back_index++;
        }
        self->empty = false;
        self->full = self->front_index == self->back_index;
        return true;
    }
}

static int Queue_size(struct Queue * self) {
    if (self->empty) {
        return 0;
    } else if (self->full) {
        return self->max_size;
    } else if (self->front_index < self->back_index) {
        return self->back_index - self->front_index;
    } else {
        return self->back_index + self->max_size - self->front_index;
    }
}

static bool Queue_is_empty(struct Queue * self) {
    return self->empty;
}

static bool Queue_is_full(struct Queue * self) {
    return self->full;
}

static void * Queue_constructor(void * _self, va_list * args) {
    struct Queue * self = _self;

    self->head = Queue_head;
    self->pop = Queue_pop;
    self->push = Queue_push;
    self->size = Queue_size;
    self->is_empty = Queue_is_empty;
    self->is_full = Queue_is_full;

    self->type = va_arg(* args, int);
    self->max_size = va_arg(* args, int);
    self->front_index = 0;
    self->back_index = 0;
    self->empty = true;
    self->full = false;
    self->data = (void **) malloc(sizeof(void *) * self->max_size);

    return self;
}

static void * Queue_destructor(void * _self) {
    struct Queue * self = _self;
    if (self->type == QUEUE_COMPLEX && !self->empty) {
        if (self->full) {
            for (int i = 0; i < self->max_size; i++) {
                delete(self->data[i]);
            }
        } else if (self->front_index < self->back_index) {
            for (int i = self->front_index; i < self->back_index; i++) {
                delete(self->data[i]);
            }
        } else {
            for (int i = self->front_index; i < self->max_size; i++) {
                delete(self->data[i]);
            }
            for (int i = 0; i < self->back_index; i++) {
                delete(self->data[i]);
            }
        }
    }
    free(self->data);
    return self;
}

static bool Queue_equals(void * self, void * other) {
    return self == other;
}

static const struct Class _Queue = {
    sizeof(struct Queue),
    Queue_constructor,
    Queue_destructor,
    Queue_equals
};

const void * Queue = & _Queue;

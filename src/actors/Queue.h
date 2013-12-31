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
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <pthread.h>

#include "ooc.h"

const int QUEUE_PRIMITIVE;
const int QUEUE_COMPLEX;

struct Queue {
    const void * class;

    void * (* head)(struct Queue * self);
    bool (* pop)(struct Queue * self, void ** head);
    bool (* push)(struct Queue * self, void * elem);
    int (* size)(struct Queue * self);
    bool (* is_empty)(struct Queue * self);
    bool (* is_full)(struct Queue * self);

    int type;
    int max_size;
    int front_index;
    int back_index;
    bool empty;
    bool full;
    void ** data;
};

const void * Queue;

#endif

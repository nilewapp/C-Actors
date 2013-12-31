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
#ifndef _SAFE_QUEUE_H_
#define _SAFE_QUEUE_H_

#include <stdbool.h>
#include <pthread.h>
#include <stdarg.h>

#include "ooc.h"
#include "Queue.h"

struct SafeQueue {
    const void * class;

    void * (* head)(struct SafeQueue * self);
    bool (* pop)(struct SafeQueue * self, void ** head);
    bool (* push)(struct SafeQueue * self, void * elem);
    int (* size)(struct SafeQueue * self);
    bool (* is_empty)(struct SafeQueue * self);
    bool (* is_full)(struct SafeQueue * self);

    struct Queue * queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

const void * SafeQueue;

#endif


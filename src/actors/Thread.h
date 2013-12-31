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
#ifndef _THREAD_H_
#define _THREAD_H_

#include <stdbool.h>
#include <stdarg.h>
#include <pthread.h>

#include "ooc.h"

struct Thread {
    const void * class;

    int (* start)(struct Thread * self);
    int (* join)(struct Thread * self, void ** value_ptr);
    int (* wait)(struct Thread * self, pthread_cond_t * cond);
    int (* signal)(struct Thread * self, pthread_cond_t * cond);

    pthread_t thread;
    pthread_mutex_t mutex;
    void * (* function)(void *);
    void * args;
};

const void * Thread;

#endif

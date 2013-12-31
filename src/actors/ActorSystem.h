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
#ifndef _ACTOR_SYSTEM_H_
#define _ACTOR_SYSTEM_H_

#include <stdarg.h>
#include <stdbool.h>
#include <pthread.h>

#include "ooc.h"
#include "Thread.h"
#include "SafeQueue.h"
#include "Message.h"
#include "Actor.h"

struct Actor;

struct ActorSystem {
    const void * class;

    void (* start)(struct ActorSystem * self);
    void (* stop)(struct ActorSystem * self);
    int (* add_actor)(struct ActorSystem * self, struct Actor * actor);
    bool (* send)(struct ActorSystem * self, struct Message * message);
    void (* receive)(struct ActorSystem * self);
    struct Thread * (* current_thread)(struct ActorSystem * self);

    int actors_max;
    int actors_size;
    struct Actor ** actors;

    struct SafeQueue * message_queue;

    int worker_threads_size;
    struct Thread ** worker_threads;
    pthread_cond_t thread_cond;
    pthread_mutex_t thread_mutex;
    bool cancel;
};

const void * ActorSystem;

#endif

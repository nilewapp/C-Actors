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
#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <stdarg.h>
#include <stdbool.h>
#include <pthread.h>

#include "ooc.h"
#include "ActorSystem.h"

struct ActorSystem;

struct Actor {
    const void * class;

    void (* send)(struct Actor * self, int receiver, void * data);
    void (* receive)(struct Actor * self, int sender, void * data);

    pthread_mutex_t mutex;
    struct ActorSystem * system;
    int id;
};

const void * Actor;

#endif

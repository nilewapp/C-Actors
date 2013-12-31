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
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#include "actors.h"
#include "FirstActor.h"

struct ActorSystem * actor_system;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void stop() {

    actor_system->stop(actor_system);

#ifdef DEBUG
    printf("Stopped actor system\n");
#endif

    delete(actor_system);

#ifdef DEBUG
    printf("Deleted actor system\n");
#endif
}

void signal_handler(int signal) {
    if (signal == SIGINT) {
        stop();
        exit(130);
    }
}

int main(int argc, char *argv[]) {

    signal(SIGINT, signal_handler);

    /*
     * Create an ActorSystem with 2 actors, a message queue of 256
     * addresses and 4 worker threads
     */
    actor_system = new(ActorSystem, 2, 256, 4);

    /* Add two TestActors to the ActorSystem */
    printf("FirstActor = '%x'\n", FirstActor);
    struct Actor * actor1 = new(FirstActor);
    struct Actor * actor2 = new(FirstActor);
    actor_system->add_actor(actor_system, actor1);
    actor_system->add_actor(actor_system, actor2);

    actor_system->start(actor_system);

#ifdef DEBUG
    printf("Started actor system\n");
#endif

    int queue_max = -1;
    long start;

    int num_messages = 10000;
    long sleep_ns = 100000;

    if (argc == 3) {
        num_messages = atoi(argv[1]);
        sleep_ns = 1000000000 / atoi(argv[2]);
    }

    /* Receive a finite number of messages before exiting */
    for (int i = 0; i < num_messages; i++) {

        /* Start measuring time from the first request */
        if (i == 0) {
            start = time(NULL);
        }

        /* Choose one of the actors to send the request to */
        struct Actor * actor = i % 2 ? actor1 : actor2;

        /* Send a message to one of the Actors */
        actor->send(actor, (void *) ((long) rand() % 10));

        /* Monitor the maximum queue length */
        int queue_size = actor_system->message_queue->size(actor_system->message_queue);
        queue_max = queue_max >= queue_size ? queue_max : queue_size;

        nanosleep((struct timespec[]){{0, sleep_ns}}, NULL);
    }

    printf("INFO Sent %d messages at %d messages per second\n",
            num_messages, 1000000000 / sleep_ns);
    printf("INFO Maximum queue size: %d\n", queue_max);

    stop();

    return 0;
}


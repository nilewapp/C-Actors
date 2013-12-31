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
#include "ActorSystem.h"

static void ActorSystem_start(struct ActorSystem * self) {
    for (int i = 0; i < self->worker_threads_size; i++) {
        self->worker_threads[i]->start(self->worker_threads[i]);
    }
}

static void ActorSystem_stop(struct ActorSystem * self) {
    self->cancel = true;

    for (int i = 0; i < self->worker_threads_size; i++) {
        pthread_cond_signal(& self->thread_cond);
    }

    for (int i = 0; i < self->worker_threads_size; i++) {
        pthread_join(self->worker_threads[i]->thread, NULL);
    }
}

static int ActorSystem_add_actor(struct ActorSystem * self, struct Actor * actor) {
    if (self->actors_size < self->actors_max) {
        actor->system = self;
        actor->id = self->actors_size;
        self->actors[self->actors_size] = actor;
        self->actors_size++;
        return actor->id;
    } else {
        return -1;
    }
}

static bool ActorSystem_send(struct ActorSystem * self, struct Message * message) {
    struct SafeQueue * message_queue = self->message_queue;
    bool sent = message_queue->push(message_queue, message);
    pthread_cond_signal(& self->thread_cond);
    return sent;
}

static void ActorSystem_receive(struct ActorSystem * self) {
    struct Message * message;
    struct Thread * current_thread = self->current_thread(self);
    while (!self->cancel) {

        struct SafeQueue * queue = self->message_queue;
        while (queue->pop(queue, (void **) & message)) {
            struct Actor * receiver = self->actors[message->receiver];
            pthread_mutex_lock(& receiver->mutex);
            receiver->receive(receiver, message->sender, message->content);
            pthread_mutex_unlock(& receiver->mutex);
            delete(message);
        }

        if (!self->cancel) {
            current_thread->wait(current_thread, & self->thread_cond);
        }
    }
}

static struct Thread * ActorSystem_current_thread(struct ActorSystem * self) {
    pthread_t current_thread = pthread_self();
    for (int i = 0; i < self->worker_threads_size; i++) {
        struct Thread * thread = self->worker_threads[i];
        if (thread->thread == current_thread) {
            return thread;
        }
    }
    return NULL;
}

static void * ActorSystem_constructor(void * _self, va_list * args) {
    struct ActorSystem * self = _self;

    self->start = ActorSystem_start;
    self->stop = ActorSystem_stop;
    self->add_actor = ActorSystem_add_actor;
    self->send = ActorSystem_send;
    self->receive = ActorSystem_receive;
    self->current_thread = ActorSystem_current_thread;

    self->actors_max = va_arg(* args, int);
    self->actors_size = 0;
    self->actors = (struct Actor **) malloc(sizeof(struct Actor *) * self->actors_max);

    self->message_queue = new(SafeQueue, QUEUE_COMPLEX, va_arg(* args, int));

    self->worker_threads_size = va_arg(* args, int);
    self->worker_threads = (struct Thread **) malloc(sizeof(struct Thread *) * self->worker_threads_size);

    for (int i = 0; i < self->worker_threads_size; i++) {
        self->worker_threads[i] = new(Thread, self->receive, self);
    }

    pthread_cond_init(& self->thread_cond, NULL);
    pthread_mutex_init(& self->thread_mutex, NULL);
    self->cancel = false;

    return self;
}

static void * ActorSystem_destructor(void * _self) {
    struct ActorSystem * self = _self;

    for (int i = 0; i < self->actors_size; i++) {
        delete(self->actors[i]);
    }
    free(self->actors);

    delete(self->message_queue);

    pthread_cond_destroy(& self->thread_cond);
    pthread_mutex_destroy(& self->thread_mutex);

    for (int i = 0; i < self->worker_threads_size; i++) {
        delete(self->worker_threads[i]);
    }

    free(self->worker_threads);

    return self;
}

static bool ActorSystem_equals(void * self, void * other) {
    return self == other;
}

static const struct Class _ActorSystem = {
    sizeof(struct ActorSystem),
    ActorSystem_constructor,
    ActorSystem_destructor,
    ActorSystem_equals
};

const void * ActorSystem = & _ActorSystem;

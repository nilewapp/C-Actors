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
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "actors.h"
#include "SocketActor.h"

struct ActorSystem * actor_system;
int sockfd;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void stop() {

    close(sockfd);

#ifdef DEBUG
    printf("Closed socket\n");
#endif

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
    struct Actor * actor1 = new(SocketActor);
    struct Actor * actor2 = new(SocketActor);
    actor_system->add_actor(actor_system, actor1);
    actor_system->add_actor(actor_system, actor2);

    long newsockfd;
    int portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

    actor_system->start(actor_system);

#ifdef DEBUG
    printf("Started actor system\n");
#endif

    int queue_max = -1;
    long start;
    int num_messages = 10000;

    /* Receive a finite number of messages before exiting */
    for (int i = 0; i < num_messages; i++) {

        /* Wait for a connection on the listening socket */
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        /* Start measuring time from the first request */
        if (i == 0) {
            start = time(NULL);
        }

        /* Choose one of the actors to send the request to */
        struct Actor * actor = i % 2 ? actor1 : actor2;

        /* Send a message to one of the Actors */
        actor->send(actor, (void *) newsockfd);

        /* Monitor the maximum queue length */
        int queue_size = actor_system->message_queue->size(actor_system->message_queue);
        queue_max = queue_max >= queue_size ? queue_max : queue_size;
    }

    printf("INFO Time to process %d messages: %ld s\n", num_messages, time(NULL) - start);
    printf("INFO Maximum queue size: %d\n", queue_max);

    stop();

    return 0;
}

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
#include "Message.h"

static void * Message_constructor(void * _self, va_list * args) {
    struct Message * self = _self;
    self->receiver = va_arg(* args, int);
    self->content = va_arg(* args, void *);
    return self;
}

static void * Message_destructor(void * self) {
    return self;
}

static bool Message_equals(void * self, void * other) {
    return self == other;
}

static const struct Class _Message = {
    sizeof(struct Message),
    Message_constructor,
    Message_destructor,
    Message_equals
};

const void * Message = & _Message;

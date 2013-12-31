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
#ifndef _OOC_H_
#define _OOC_H_

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

struct Class {
    size_t size;
    void * (* constructor)(void * self, va_list * args);
    void * (* destructor)(void * self);
    bool (* equals)(void * self, void * other);
};

void * new(const void * _class, ...);

void delete(void * self);

#endif

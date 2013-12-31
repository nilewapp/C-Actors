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
#include "ooc.h"

void * new(const void * _class, ...) {
    const struct Class * class = _class;
    void * obj = malloc(class->size);

    assert(obj);
    * (const struct Class **) obj = class;

    if (class->constructor) {
        va_list ap;

        va_start(ap, _class);
        obj = class->constructor(obj, & ap);
        va_end(ap);
    }

    return obj;
}

void delete(void * self) {
    struct Class ** class = self;
    if (self && * class && (* class)->destructor) {
        self = (* class)->destructor(self);
    }
    free(self);
}

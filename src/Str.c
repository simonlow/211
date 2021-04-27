#include <string.h>

#include "Str.h"
#include "Vec.h"

static char NULL_CHAR = '\0';

Str Str_value(size_t capacity)
{
    Str s = Vec_value(capacity + 1, sizeof(char));
    Vec_set(&s, 0, &NULL_CHAR);
    return s;
}

void Str_drop(Str *self)
{
    Vec_drop(self);
}

size_t Str_length(const Str *self)
{
    return Vec_length(self) - 1;
}

const char* Str_cstr(const Str *self)
{
    return (char*) Vec_ref(self, 0);
}

char* Str_ref(const Str *self, const size_t index)
{
    return (char*) Vec_ref(self, index);
}

//Functions have yet to be tested, are pending success of Vec
Str Str_from(const char *cstr) {
    char c;
    int count = 0;
    while ((c = cstr[count]) != '\0') {
        count++;
    }
    count++;
    Str s = Str_value(count);
    s.length = count;
    char * buffer = s.buffer;
    memcpy(buffer, cstr, count);
    return s;
}

void Str_splice (Str *self, size_t index, size_t delete_count, const char *cstr, size_t insert_count) {
    if (index == Str_length(self) + 1) {
        Vec_splice(self, index -1, delete_count, cstr, insert_count);
    }
    Vec_splice(self, index, delete_count, cstr, insert_count);
}

void Str_append(Str *self, const char *cstr) {
    size_t count = 0;
    char c;
    while ((c = cstr[count]) != '\0') {
        count++;
    }
    //count++;
    self->capacity += count;
    size_t length = Str_length(self);
    Vec_splice(self, length, 0, cstr, count);
    //memcpy( buffer + sizeof(char)*length, cstr, count); 
}

char Str_get(const Str*self, size_t index) {
    char * c;
    c = Vec_ref(self, index);
    return *c;
}

void Str_set(Str *self, size_t index, const char value) {
    if (index == Str_length(self)+1) {
        //Str_append(self, &value);
        Vec_set(self, index, &value);
    }
    else if (index == Str_length(self)) {
        Vec_set(self, index, &value);
    }
    else {
        Vec_set(self, index, &value);
    }
}

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Guards.h"

#include "Vec.h"

/* Constructor / Destructor */
static void _ensure_capacity (Vec*self, size_t n);


Vec Vec_value(size_t capacity, size_t item_size)
{
    Vec vec = {
        item_size,
        0,
        capacity,
        calloc(capacity, item_size)
    };
    OOM_GUARD(vec.buffer, __FILE__, __LINE__);
    return vec;
}

void Vec_drop(Vec *self)
{
    free(self->buffer);
    self->buffer = NULL;
    self->capacity = 0;
    self->length = 0;
}

/* Accessors */

size_t Vec_length(const Vec *self)
{
    return self->length;
}

void* Vec_ref(const Vec *self, size_t index)
{
    if (index < self->length) {
        return self->buffer + (index * self->item_size);
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

//Start of functions implemented by me
void Vec_get(const Vec *self, size_t index, void *out) {
   if (index > self->length) {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
   }
   else {
       memcpy(out, Vec_ref(self, index), self->item_size); 
   }

}
void Vec_set(Vec *self, size_t index, const void *value) {
    if(index == self->length) {
        Vec_splice(self, index, 0, value, 1);
    }
    else {
        Vec_splice(self, index, 1, value, 1);
    }
}

bool Vec_equals (const Vec *self, const Vec *other) {
    
    if(Vec_length(self) != Vec_length(other) ) {
        return false; 
    }
    else {
        for (size_t i = 0; i < self->length; i++) {
            size_t* self_element = Vec_ref(self, i);
            size_t* other_element = Vec_ref(other, i);
            if (*self_element != *other_element) {    
                return false;
            }
        }
        return true;           
    }
    return false;

}

void Vec_splice(Vec *self, size_t index, size_t delete_count, const void *items, size_t insert_count) {
    if (delete_count > (self->length - index) ){
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    if(index > self->length) { 
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    } 
    _ensure_capacity(self, index + insert_count);

    //Version of Vec that was turned in for grading on the last lab is commented out
    //The version in use here is in collaboration with another student
    //This collaboration was advised by a TA
    //Previous Vec somehow worked for Str, StrVec, Scanner and passed the autograder with 100%
    //For this reason other code is used in order to work Parser
    if (delete_count > 0) {
        int items_left = delete_count;
        while(items_left > 0) {
            for(size_t i = index; i < self->length - 1; i++) {
                char *destination = (char*)(self->buffer) + (i*self->item_size);
                char *source = (char*)(self->buffer) + ((i+1)*self->item_size);
                memcpy(destination, source, self->item_size); 
            }
            self->length--;
            items_left--;
        }
    }
    
    if (insert_count > 0) {
        size_t items_left = insert_count;
        while (items_left > 0) {
            for(size_t i = self->length; i> index; i--) {
                char *destination = (char*)(self->buffer) + (i*self->item_size);
                char *source = (char*)(self->buffer) + ((i-1)*self->item_size);
                memcpy(destination, source, self->item_size);
            }
            char *destination = (char*)(self->buffer) + (index*self->item_size);
            char *source = (char*)(items);
            memcpy(destination, source, self->item_size);
            items += self->item_size;
            index++;
            self->length++;
            items_left--;
        }    
    }
    /*
    size_t new_size = self->length + (insert_count - delete_count);
    size_t orig_length = self->length;
    self->length = new_size; 
    _ensure_capacity(self, new_size);
    void * temp_array = calloc(new_size, sizeof(self->item_size));
    memcpy(temp_array, self->buffer, index*self->item_size);
    memcpy(temp_array + index*(self->item_size), items, insert_count*(self->item_size));
    memcpy(temp_array + (index + insert_count)*(self->item_size), self->buffer +(self->item_size)*(index + delete_count), (orig_length - delete_count - index) * self->item_size);
    memcpy(self->buffer, temp_array, new_size*(self->item_size));
    if (index == 0 && delete_count == 0 && insert_count == 1) {
        return;
    }
    else {
        free(temp_array);
    }
    */
}

static void _ensure_capacity(Vec *self, size_t n) {
    if (n > self->capacity) {
        size_t new_capacity = n * 2;
        self->buffer = realloc(self->buffer, new_capacity * self->item_size);
        OOM_GUARD(self->buffer, __FILE__, __LINE__);
        self->capacity = new_capacity;
    }
}

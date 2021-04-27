#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"

#include <stdlib.h>
#include "Vec.h"
}

/*
 * Unit tests established during the implementation of Vec.c
 * These tests are permitted to reach through the abstraction
 * and access members of a Vec struct directly.
 */

TEST(VecImpl, value) {
    Vec v = Vec_value(2, sizeof(int16_t));
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(2, v.capacity);
    ASSERT_STREQ((char*)v.buffer, "");
    Vec_drop(&v);
}

TEST(VecImpl, drop) {
    Vec v = Vec_value(2, sizeof(int16_t));
    Vec_drop(&v);
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(0, v.capacity);
    ASSERT_EQ(nullptr, v.buffer);
}

TEST(VecImpl, ref) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    v.length = 2;
    ASSERT_EQ(&buffer[0], Vec_ref(&v, 0));
    ASSERT_EQ(&buffer[1], Vec_ref(&v, 1));
    Vec_drop(&v);
}

TEST(VecImpl, ref_out_of_bounds) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    ASSERT_DEATH({
       Vec_ref(&v, 3);
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

//Start of my tests

TEST(VecImpl, get) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t first = 3;
    int16_t second = 5;
    int16_t *buff = (int16_t*) v.buffer;
    buff[0] = 7;
    buff[1] = 0;
    v.length = 2;
   
    Vec_get(&v, 0, &first);
    Vec_get(&v, 1, &second);
    ASSERT_EQ(7, first);
    ASSERT_EQ(0, second);
    Vec_drop(&v);
}

TEST(VecImpl, get_set) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *vals = (int16_t*) calloc(1, 2);
    vals[0] = 1;
    int16_t * out = (int16_t*) calloc(1,2);
    Vec_set(&v, 0, &vals[0]);
    Vec_get(&v, 0, out);
    ASSERT_EQ(*out, vals[0]);
    free(vals);
    free(out);
}
//Code commented out until further notice

TEST(VecImpl, get_out_of_bounds) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t dest = 2; 
    ASSERT_DEATH( {
        Vec_get(&v, 3, &dest);
    }, ".* - Out of Bounds");
    Vec_drop(&v);    

}

TEST(VecImpl, set) {
    Vec v = Vec_value(1, sizeof(int16_t));
    int16_t a = 10;
    int16_t b = 20;
    Vec_set(&v, 0, &a);
    ASSERT_EQ(1, Vec_length(&v));
    Vec_set(&v, 1, &b);
    ASSERT_EQ(2, Vec_length(&v)); 
    Vec_drop(&v);
}

TEST(VecImpl, set_new_index) {
    Vec v = Vec_value(3, sizeof(int16_t));
    v.length = 3;
    int16_t a = 5;
    Vec_set(&v, 3, &a);
    ASSERT_EQ(4, v.length);
    int16_t * buffer = (int16_t*) v.buffer;
    ASSERT_EQ(5, buffer[3]);
    Vec_drop(&v);
}

TEST(VecImpl, set_out_of_bounds) {
   Vec v = Vec_value(2, sizeof(int16_t));
   int16_t a = 3;
    ASSERT_DEATH( {
        Vec_set(&v, 3, &a); 
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, equals) {
    Vec one =Vec_value(3, sizeof(int16_t));
    Vec two = Vec_value(3, sizeof(int16_t));
    one.length = 3;
    two.length = 3; 
    int16_t * buffone = (int16_t*) one.buffer;
    int16_t * bufftwo = (int16_t*) two.buffer;

    buffone[0] = 4;
    bufftwo[0] = 4;
    buffone[1] = 8;
    bufftwo[1] = 8;
    buffone[2] = 12;
    bufftwo[2] = 12; 
  
    ASSERT_TRUE (Vec_equals (&one, &two));
    
    Vec_drop(&one);
    Vec_drop(&two);
}

TEST(VecImpl, not_equal_length) { 
    Vec four = Vec_value(2, sizeof(int16_t));
    Vec two = Vec_value(3, sizeof(int16_t));
    four.length = 2;
    four.length = 3;

    int16_t * bufftwo = (int16_t*) two.buffer;
    int16_t * bufffour = (int16_t*) four.buffer;
    bufftwo[0] = 4;
    bufftwo[1] = 8;
    bufftwo[2] = 12; 

    bufffour[0] = 4;
    bufffour[1] = 8;

    ASSERT_FALSE(Vec_equals(&two, &four));
    Vec_drop(&two);
    Vec_drop(&four);
}

TEST(VecImpl, not_equal_elements) {
    Vec one = Vec_value(3, sizeof(int16_t));
    Vec three = Vec_value(3, sizeof(int16_t));
    one.length = 3;
    three.length = 3; 
    int16_t * buffone = (int16_t*) one.buffer; 
    int16_t * buffthree = (int16_t*) three.buffer;
    
    buffone[0] = 4;
    buffone[1] = 8;
    buffone[2] = 12;

    buffthree[0] = 4;
    buffthree[1] = 8;
    buffthree[2] = 11;

    ASSERT_FALSE(Vec_equals(&one, &three));
    Vec_drop(&one);
    Vec_drop(&three);
}

TEST(VecImpl, splice_delete_none_add_all) {
    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(3, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;

    int16_t * buffa = (int16_t*) a.buffer;  

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;
    
    int16_t * buff_a = (int16_t*) a.buffer;  

    Vec_splice(&a, 2, 0, newvals, 3);
    ASSERT_EQ(buff_a[0], 2);
    ASSERT_EQ(buff_a[1], 4);
    ASSERT_EQ(buff_a[2], 1);
    ASSERT_EQ(buff_a[3], 3);
    ASSERT_EQ(buff_a[4], 5);
    ASSERT_EQ(buff_a[5], 6);
    ASSERT_EQ(buff_a[6], 8);
    ASSERT_EQ(buff_a[7], 10);
    free(newvals);
    Vec_drop(&a);
}

TEST(VecImpl, splice_delete_some_add_some) {

    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(3, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;

    int16_t * buffa = (int16_t*) a.buffer;  

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;

    Vec_splice(&a, 1, 3, newvals, 2);

    int16_t * buff_a = (int16_t*) a.buffer;  
    
    ASSERT_EQ(buff_a[0], 2);
    ASSERT_EQ(buff_a[1], 1);
    ASSERT_EQ(buff_a[2], 3);
    ASSERT_EQ(buff_a[3], 10);
    free(newvals);
    Vec_drop(&a);
}

TEST(VecImpl, splice_delete_some_add_none) {
    
    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(3, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;

    int16_t * buffa = (int16_t*) a.buffer;  

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;

    Vec_splice(&a, 2, 2, newvals, 0);

    int16_t * buff_a = (int16_t*) a.buffer;  
    
    ASSERT_EQ(buff_a[0], 2);
    ASSERT_EQ(buff_a[1], 4);
    ASSERT_EQ(buff_a[2], 10);
    free(newvals);
    Vec_drop(&a);
}

TEST(VecImpl, splice_delete_some_add_all) {

    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(3, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;

    int16_t * buffa = (int16_t*) a.buffer;  

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;
    Vec_splice(&a, 2, 2, newvals, 3);
     
    int16_t * buff_a = (int16_t*) a.buffer;  
    
    ASSERT_EQ(buff_a[0], 2);
    ASSERT_EQ(buff_a[1], 4);
    ASSERT_EQ(buff_a[2], 1);
    ASSERT_EQ(buff_a[3], 3);
    ASSERT_EQ(buff_a[4], 5);
    ASSERT_EQ(buff_a[5], 10);
    free(newvals);
    Vec_drop(&a);
}

TEST(VecImpl, splice_delete_none_add_none) {

    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(3, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;

    int16_t * buffa = (int16_t*) a.buffer;  

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;
    
    Vec_splice(&a, 0, 0, newvals, 0);
    int16_t * buff_a = (int16_t*) a.buffer;  
    
    ASSERT_EQ(buff_a[0], 2);
    ASSERT_EQ(buff_a[1], 4);
    ASSERT_EQ(buff_a[2], 6);
    ASSERT_EQ(buff_a[3], 8);
    ASSERT_EQ(buff_a[4], 10);
    Vec_drop(&a);
    free(newvals);
}


TEST(VecImpl, splice_delete_none_add_some) {

    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(3, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;

    int16_t * buffa = (int16_t*) a.buffer;  

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;
    
    Vec_splice(&a, 3, 0, newvals, 1);
    int16_t * buff_a = (int16_t*) a.buffer;  
    
    ASSERT_EQ(buff_a[0], 2);
    ASSERT_EQ(buff_a[1], 4);
    ASSERT_EQ(buff_a[2], 6);
    ASSERT_EQ(buff_a[3], 1);
    ASSERT_EQ(buff_a[4], 8);
    ASSERT_EQ(buff_a[5], 10);
    free(newvals);
    Vec_drop(&a);
}
TEST(VecImpl, splice_delete_all_add_all) {

    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(3, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;

    int16_t * buffa = (int16_t*) a.buffer;  

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;
   
    Vec_splice(&a, 0, 5, newvals, 3);
    int16_t * buff_a = (int16_t*) a.buffer;  
    
    ASSERT_EQ(buff_a[0], 1);
    ASSERT_EQ(buff_a[1], 3);
    ASSERT_EQ(buff_a[2], 5);
    free(newvals);
    Vec_drop(&a);
}

TEST(VecImpl, splice_delete_all_add_some){

    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(3, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;

    int16_t * buffa = (int16_t*) a.buffer;  

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;
   
    Vec_splice(&a, 0, 5, newvals, 2);
    int16_t * buff_a = (int16_t*) a.buffer;  
    
    ASSERT_EQ(buff_a[0], 1);
    ASSERT_EQ(buff_a[1], 3);
    free(newvals);
    Vec_drop(&a);
}
TEST(VecImpl, splice_delete_all_add_none) {
    
    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(3, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;

    int16_t * buffa = (int16_t*) a.buffer;  

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;
    
    void * out = calloc(1, sizeof(int16_t));
    Vec_splice(&a, 0, 5, newvals, 0); 
    ASSERT_DEATH({
        Vec_get(&a, 0, out);
        }, ".* - Out of Bounds"); 
    free(newvals);
    Vec_drop(&a);
}

TEST(VecImpl, splice_delete_too_much) {
    Vec a = Vec_value(5, sizeof(int16_t));
    a.length = 5;
    int16_t* newvals = (int16_t*) calloc(6, sizeof(int16_t));
    newvals[0] = 1;
    newvals[1] = 3;
    newvals[2] = 5;
    newvals[3] = 7;
    newvals[4] = 9;
    newvals[5] = 11;
    
    int16_t * buffa = (int16_t*) a.buffer;

    buffa[0] = 2;
    buffa[1] = 4;
    buffa[2] = 6;
    buffa[3] = 8;
    buffa[4] = 10;
    
    ASSERT_DEATH({
       Vec_splice(&a, 0, 6, newvals, 3);
    }, ".* - Out of Bounds");
    Vec_drop(&a);
    free(newvals);
}

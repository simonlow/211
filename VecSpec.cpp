#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

/**
 * The purpose of these tests is to prove correctness of the
 * abstraction barrier from the user's point-of-view. You should
 * not access any member of the Vec directly.
 */

TEST(VecSpec, values_init_empty) {
    Vec v = Vec_value(10, sizeof(double));
    ASSERT_EQ(0, Vec_length(&v));
    Vec_drop(&v);
}

TEST(VecSpec, set_and_length) {
    Vec v = Vec_value(5, sizeof(int16_t));
    int16_t vals [3] = {1,2,3};
    Vec_set(&v, 0, &vals[0]);
    Vec_set(&v, 1, &vals[1]);
    Vec_set(&v, 2, &vals[2]);
    
    int16_t * ptr = (int16_t*) Vec_ref(&v, 0);
    ASSERT_EQ(*ptr, vals[0]); 
    ASSERT_EQ(Vec_length(&v), 3);
    Vec_drop(&v);
}

TEST(VecSpec, get) {
    Vec v = Vec_value(2, sizeof(int16_t));   
    int16_t * out= (int16_t*) calloc(1, sizeof(int16_t));
    //int16_t vals [3] = {1,2,3};
    int16_t * vals = (int16_t*)calloc(3, sizeof(int16_t));
    vals[0] = 1;
    vals[1] = 2;
    vals[2] = 3;
    
    Vec_set(&v, 0, &vals[0]); 
    Vec_get(&v, 0, out); 
    ASSERT_EQ(*out, vals[0]);
    free(out);
    free(vals);
    Vec_drop(&v);
}

TEST(VecSpec, equals) {
    Vec a = Vec_value(5, sizeof(int16_t));
    Vec b = Vec_value(5, sizeof(int16_t));
    int16_t vals [3] = {1,2,3};
    Vec_set(&a, 0, &vals[0]); 
    Vec_set(&b, 0, &vals[0]); 
    Vec_set(&a, 1, &vals[1]); 
    Vec_set(&b, 1, &vals[1]); 
    Vec_set(&a, 2, &vals[2]); 
    Vec_set(&b, 2, &vals[2]); 
    
    ASSERT_TRUE(Vec_equals(&a, &b));
    Vec_drop(&a);
    Vec_drop(&b);
}

TEST(VecSpec, splice) {
    Vec v = Vec_value(5, sizeof(int16_t));
    int16_t splice_vals [3] = {7,8,9};
    int16_t vals [3] = {1,2,3};
    Vec_set(&v, 0, &vals[0]);
    Vec_set(&v, 1, &vals[1]);
    Vec_set(&v, 2, &vals[2]);
    Vec_splice(&v, 1, 1, &splice_vals, 3);
    int16_t * x = (int16_t*)Vec_ref(&v, 0);
    ASSERT_EQ(*x, vals[0]);
    int16_t * y = (int16_t*) Vec_ref(&v, 1);
    ASSERT_EQ(*y, splice_vals[0]);
    int16_t* z = (int16_t*) Vec_ref(&v, 3);
    ASSERT_EQ(*z, splice_vals[2]);
    int16_t * a =(int16_t*) Vec_ref(&v, 4);
    ASSERT_EQ(*a, vals[2]);

    Vec_drop(&v);
}

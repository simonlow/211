#include "gtest/gtest.h"

extern "C" {
#include "Str.h"
#include "string.h"
}
TEST(StrImpl, value) {
    Str s = Str_value(2);
    ASSERT_EQ(1, s.length); // Vec's length includes null char
    ASSERT_EQ(3, s.capacity); // Requested space + 1 for null char
    char *buffer = (char*) s.buffer;
    ASSERT_EQ('\0', buffer[0]);
    Str_drop(&s);
}

TEST(StrImpl, drop) {
    Str s = Str_value(2);
    Str_drop(&s);
    ASSERT_EQ(0, s.capacity);
    ASSERT_EQ(0, s.length);
    ASSERT_EQ(nullptr, s.buffer);
}
/*
 * Helper fixture to setup a Str whose buffer
 * is prefilled with "abcd". 
 */
Str fixture_abcd()
{
    Str s = Str_value(4);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';
    buffer[3] = 'd';
    buffer[4] = '\0';
    s.length = 5; // Includes null char
    return s;
}

TEST(StrImpl, length) {
    Str s = Str_value(2);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);

    s = fixture_abcd();
    ASSERT_EQ(4, Str_length(&s));
    Str_drop(&s);
}

TEST(StrImpl, cstr) {
    Str s = fixture_abcd();
    ASSERT_STREQ("abcd", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrImpl, ref) {
    Str s = fixture_abcd();
    char *buffer = (char*) s.buffer;
    for (size_t i = 0; i < s.length; ++i) {
        ASSERT_EQ(&buffer[i], Str_ref(&s, i));
    }
    Str_drop(&s);
}

//My own tests
TEST(StrImpl, from_empty_string) {
    Str s;
    s = Str_from("");
    ASSERT_STREQ(Str_cstr(&s), "");
    Str_drop(&s);
}

TEST(StrImpl, from) {
    Str s;
    const char * c = "hello";
    s = Str_from(c);
    ASSERT_STREQ(Str_cstr(&s), "hello");
    Str_drop(&s);
}

TEST(StrImpl, splice) {
   Str s = fixture_abcd();
   const char * vals = "ef";
   Str_splice(&s, 2, 1, vals, 2);
   ASSERT_STREQ(Str_cstr(&s), "abefd");
    Str_drop(&s);
}

TEST(StrImpl, splice_delete_none_add_some) {
    Str s = fixture_abcd();
    const char * vals = "ef";
    Str_splice(&s, 2, 0, vals, 2);
    ASSERT_STREQ(Str_cstr(&s), "abefcd");
    Str_drop(&s);
}

TEST(StrImpl, splice_delete_some_add_none) {
    Str s = fixture_abcd();
    const char * vals = "ef";
    Str_splice(&s, 2, 1, vals, 0);
    ASSERT_STREQ(Str_cstr(&s), "abd");
    Str_drop(&s);
}
/*
TEST(StrImpl, splice_delete_too_much) { 
    Str s = fixture_abcd();
    const char * vals = "ef";
    ASSERT_DEATH( {
       Str_splice(&s, 2, 5, vals, 1);
       }, ".* - Out of Bounds"); 
    Str_drop(&s);
}
*/
TEST(StrImpl, append_char) {
    Str s = fixture_abcd();
    const char *c = "l";
    Str_append(&s, c);
    ASSERT_STREQ(Str_cstr(&s), "abcdl");
    Str_drop(&s);
}

TEST(StrImpl, append_string) { 
    Str s = fixture_abcd();
    const char *c = "xyz";
    Str_append(&s, c);
    ASSERT_STREQ(Str_cstr(&s), "abcdxyz");
    Str_drop(&s);
}

TEST(StrImpl, append_nothing) {
    Str s = fixture_abcd();
    const char *c = "";
    Str_append(&s, c);
    ASSERT_STREQ(Str_cstr(&s), "abcd");
    Str_drop(&s);
    
}

TEST(StrImpl, get) {
    Str s = fixture_abcd();
    char c = Str_get(&s, 2);
    ASSERT_EQ(*Str_ref(&s,2), c);
    Str_drop(&s);
}

TEST(StrImpl, get_out_of_bounds) {
    Str s = fixture_abcd();
    ASSERT_DEATH( {
        Str_get(&s, 5);
    }, ".* - Out of Bounds");
    Str_drop(&s);
}
TEST(StrImpl, get_null) {
    Str s = fixture_abcd();
    char c = Str_get(&s, 4);
    ASSERT_EQ(c,*Str_ref(&s,4));
    Str_drop(&s);

}
TEST(StrImpl, set) {
    Str s = fixture_abcd();
    const char* c = "x";
    Str_set(&s, 2, *c);
    ASSERT_STREQ(Str_cstr(&s), "abxd");
    Str_drop(&s);
}

TEST(StrImpl, set_index) {
    Str s = fixture_abcd();
    const char* c = "x";
    Str_set(&s, 4, *c);
    ASSERT_STREQ(Str_cstr(&s), "abcdx");
    Str_drop(&s);
}

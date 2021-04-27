#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Str.h"
}

/**
 * The purpose of these tests is to prove correctness of the Str
 * abstraction barrier from the user's point-of-view.
 */
TEST(StrSpec, values_init_empty) {
    Str s = Str_value(10);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);
}

TEST(StrSpec, values_init_cstr) {
    Str s = Str_value(10);
    ASSERT_STREQ("", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrSpec, from) {
    const char *str = "hello";
    Str s = Str_from(str);
    ASSERT_STREQ(str, Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrSpec, splice) {
    const char * str = "heabclo";
    Str s = Str_value(8);
    s = Str_from("hello");
    const char * ins = "abc";
    Str_splice(&s, 2, 1, ins, 3);
    ASSERT_STREQ(Str_cstr(&s), str);
    Str_drop(&s);
}

TEST(StrSpec, append) {
    Str s = Str_value(8);
    const char * str = "abc";
    const char * full = "hello";
    s = Str_from(full);
    Str_append(&s, str);
    const char * final = "helloabc";
    ASSERT_STREQ(final, Str_cstr(&s));
    Str_drop(&s);

}

TEST(StrSpec, get) {
    Str s = Str_from("hello");
    char x = Str_get(&s, 1);
    ASSERT_EQ('e', x);
    Str_drop(&s);
}

TEST(StrSpec, set) {
    Str s = Str_from("hello");
    Str_set(&s, 5, 'x');
    ASSERT_STREQ(Str_cstr(&s), "hellox"); 
    Str_drop(&s);
}

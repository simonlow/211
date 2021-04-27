#include <ctype.h>
#include <stdio.h>
#include "Scanner.h"

static bool _is_character (char c);
static void _create_token(Scanner *self);
static Str _create_word_lex (CharItr *char_itr);
static void _eat_white (CharItr *char_itr); 

Scanner Scanner_value(CharItr char_itr)
{
    Token next = {
       END_TOKEN,
       Str_from("")
    }; 
    Scanner scan = {
        char_itr,
        next
    };
    _create_token(&scan);
    return scan;
}

bool Scanner_has_next(const Scanner *self) {
    if (self->next.type == PIPE_TOKEN) {
        return true;
    }
    else if (self->next.type == WORD_TOKEN){
        return true;
    }
    else {
        return false;
    }
}

Token Scanner_peek(const Scanner *self)
{
    return self->next;
}

Token Scanner_next(Scanner *self){
    Token last_next = self->next;
    _create_token(self);
    return last_next;
}
    

static bool _is_character(char c) {
    switch(c){ 
        case('|'):
            return false;
        case('\n'):
            return false;
        case('\t'):
            return false;
        case(EOF):
            return false;
        case(' '):
            return false;
        case('\0'):
           return false;
        default:
          return true; 

    }
}

static void _create_token(Scanner *self) {
    CharItr *char_itr = &(self->char_itr);
    _eat_white(char_itr);
    if (CharItr_has_next(char_itr)) {        
        char c = CharItr_peek(char_itr);
        switch(c) {
           case ('|'):
               self->next.type = PIPE_TOKEN;
               self->next.lexeme = Str_from("|");
               CharItr_next(char_itr);
               return;
           case (EOF):
               self->next.type= END_TOKEN;
               self->next.lexeme = Str_from(""); 
               CharItr_next(char_itr);
               return;
           case ('\0'):
               self->next.type = END_TOKEN;
               self->next.lexeme = Str_from(""); 
               CharItr_next(char_itr);
               return;
           default:
               self->next.type = WORD_TOKEN; 
               self->next.lexeme = _create_word_lex(char_itr);
        }  
    }
    else {
        self->next.type = END_TOKEN;
        self->next.lexeme = Str_from("");
    }
}

static Str _create_word_lex (CharItr *char_itr) {
    Str str = Str_from(""); 
    char c[2];
    c[1] = '\0';
    while(CharItr_has_next(char_itr)) {
        c[0] = CharItr_peek(char_itr);
        if (_is_character(c[0])){
            Str_append(&str, c); 
            CharItr_next(char_itr);
        }
        else {
            break;
        }
    }
    return str;
}

static void _eat_white (CharItr *char_itr) {
    char c;
    while (CharItr_has_next(char_itr)) { 
        c = CharItr_peek(char_itr);
        if ( (c == ' ') || (c == '\n') || (c == '\t') ) {
            CharItr_next(char_itr);
        }
        else {
            return;
        }
    } 

}

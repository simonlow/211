#include "Parser.h"
#include "Scanner.h"

static Node* _add_command(Scanner *scanner);
static Node* _add_pipe(Node *node, Scanner *scanner);
static Node* _find_error_node (Node *node); 

Node* parse(Scanner *scanner)
{
    // Initialize node so it has a type
    Node *node = ErrorNode_new("initial"); 
    if (Scanner_has_next(scanner)) {
        Token tok = Scanner_peek(scanner);
        
        // make sure node starts with a word
        if (tok.type != WORD_TOKEN) {
            const char *msg = "Node doesn't start with word";
            node = ErrorNode_new(msg);
            return node; 
        }
         
        while (Scanner_has_next(scanner)) {
            Token temp = Scanner_peek(scanner);
            if (temp.type == WORD_TOKEN) {
                if (node->type != PIPE_NODE) {
                    node = _add_command(scanner);
                }
                else {
                    if(node->data.pipe.right->type == ERROR_NODE) {
                        node->data.pipe.right = _add_command(scanner);
                        node->data.pipe.right->type = COMMAND_NODE;
                    }
                    else {
                        if (node->data.pipe.right->data.pipe.right->type == ERROR_NODE) {
                            node->data.pipe.right->data.pipe.right = _add_command(scanner);
                        }
                        else {   
                           Node *rhs = _find_error_node(node);
                           rhs = _add_command(scanner);
                           rhs->type = COMMAND_NODE;
                        }
                    }   
                }
            }
            else if (temp.type == PIPE_TOKEN) {
                if (node->type == COMMAND_NODE) {
                    node = _add_pipe(node, scanner);
                }           
                else if (node->type == ERROR_NODE) {
                    return node;
                }
                else {
                    if (node->data.pipe.right->type == ERROR_NODE) {
                        return node->data.pipe.right; 
                    }
                    else {
                        node = _add_pipe(node, scanner);   
                    }
                } 
            }
            else if (temp.type == END_TOKEN) {
                return node;
            }
            else {
                Node *error = ErrorNode_new("token isn't a pipe or word\n");
                node = error;
            }    
        }
        return node;
    }
    else {
        node = ErrorNode_new("No input from Scanner");
        return node;
    }
}

static Node* _add_command(Scanner *scanner) {
    StrVec command = StrVec_value(20); 
    StrVec_push(&command, Scanner_next(scanner).lexeme);
    while((Scanner_has_next(scanner)) && (Scanner_peek(scanner).type == WORD_TOKEN) ){
        if (command.length == command.capacity) {
            command.capacity++;
        }
        StrVec_push(&command, Scanner_next(scanner).lexeme);
    }
    Node *command_node = CommandNode_new(command);
    return command_node;
}

static Node* _add_pipe(Node *node, Scanner *scanner) {
    Scanner_next(scanner);
    if (node->type == COMMAND_NODE) {
        Node * left_node = node;
        Node * right_node = {ErrorNode_new("right node is error")};
        Node *pipe_node = PipeNode_new(left_node, right_node);
        return pipe_node; 
    }
    else {
        Node *left_node = node->data.pipe.left;
        Node *err = ErrorNode_new("new right node");
        Node *right_node = PipeNode_new(node->data.pipe.right, err);
        Node *pipe_node = PipeNode_new(left_node, right_node);
        return pipe_node;
    }
}

static Node* _find_error_node (Node *node) {
    Node *right = node->data.pipe.right;
    if (right->type != ERROR_NODE) {
        _find_error_node(right);
    }
    return right;

}

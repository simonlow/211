#include "Node.h"
#include "Guards.h"

static void _free_command(Node*self);
static void _free_pipe (Node *self); 

Node* ErrorNode_new(const char *msg)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = ERROR_NODE;
    node->data.error = msg;
    return node;
}

Node* CommandNode_new(StrVec words)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = COMMAND_NODE;
    node->data.command = words;
    return node;
}

Node* PipeNode_new(Node *left, Node *right)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = PIPE_NODE;
    node->data.pipe.left = left;
    node->data.pipe.right = right;
    return node;
}

void* Node_drop(Node *self)
{
    if (self->type == COMMAND_NODE) { 
        _free_command(self);
        return self;
    }
    else if (self->type == PIPE_NODE){
        _free_pipe(self);
        return self;
    }
    else {
        free(self);
        return self;
    }

}

static void _free_command (Node *self) {
    StrVec_drop(&(self->data.command));
    free(self);
}

static void _free_pipe (Node *self) {
    if (self->data.pipe.left->type == COMMAND_NODE) {
        _free_command(self->data.pipe.left);
    }

    if (self->data.pipe.right->type == PIPE_NODE) {
        _free_pipe(self->data.pipe.right); 
    } 
    else if ( self->data.pipe.right->type == COMMAND_NODE) {
        _free_command(self->data.pipe.right);
    }
    else {
        free(self->data.pipe.right);
    }   
    free(self);
}

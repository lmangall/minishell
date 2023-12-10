#ifndef NODE_H
#define NODE_H

enum node_type_e
{
    ROOT,/* simple command */
    VAR, /* variable name (or simply, a word) */
	SPECIAL,
	MASTER,
};

typedef enum e_operator {
	NONE,                 //
	RDR_OUT_REPLACE,      //    >
	RDR_OUT_APPEND,       //    >>
	RDR_INPUT,  	      //    <
	RDR_INPUT_UNTIL,      //    <<
	PIPE,  		          //    |
}				t_operator;

struct node_s
{
    enum   node_type_e type;
    enum   e_operator operator;
    char	*str;
    int    children; 
    struct node_s *first_child;
    struct node_s *next_sibling;
    struct node_s *prev_sibling;
};

struct node_type_master
{
    enum   node_type_e type;   
    char	*str;   
    int    nbr_root_nodes;
    struct node_s **root_nodes;
    //FILE DESCRIPTOR STUFF

};

struct node_special
{
    enum   node_type_e type;    /* type of this node */
    char	*str;        /* value of this node */
    //FILE DESCRIPTOR STUFF
};


/**
 * @brief  creates a new node and sets it's type field.
 */
struct  node_s *new_node(enum node_type_e type);
/**
 * @brief expands the AST of a simple command by adding a new child node
 *  and incrementing the root node's children field. 
 * If the root node has no children,
 * the new child is assigned to the first_child field of the root node. 
 * Otherwise, the child is appended to the end of the children's list.
 */
int add_child_node(struct node_s *parent, struct node_s *child);

/**
 * @brief 
 * 
 * @param type 
 * @return struct node_s* 
 */
void    free_node_tree(struct node_s *node);
/**
 * @brief sets a node's value to the given string. 
 * It copies the string to a newly allocated memory space, 
 * then sets the val_type and val.str fields accordingly. 
 */
void    set_node_str(struct node_s *node, char *val);

int add_sibling_node(struct node_s *parent, struct node_s *child);




// t_operator get_operator(char *operator);
// void set_operators(struct node_type_master *master);

#endif
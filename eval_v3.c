//
//  parser.c
//
//
//  Created by David Samuel on 2017-11-23.
//
//
//


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define COUNT 8
/* *****************************************************************************
 OBJECTIVE
 ******************************************************************************* */

/*
 NEED TO CREATE A PARSER USING THE SCANNER I MADE FOR HOMEWORK ONE.
 
 */

/* *****************************************************************************
 NOTES FOR SELF
 ******************************************************************************* */

// MORE TESTING!!

/* *****************************************************************************
 FUNCTION DECLARATIONS, GLOBAL ARRAY DECLARATIONS, AND TOKEN DECLARATION
 ******************************************************************************* */


// Define Token type, and associated uses of type Token
typedef enum {IDENTIFIER,BOOLEAN,REAL,PUNCTUATION,KEYWORD,
    PLUS,INTEGER,ERROR} Token;

// an array of Keywords specified by the language thats going to be implemented.
char *keywords[] = {"var","fun","if","else","return", "read","write","not", "or", "and","call","while"};

// This buffer will hold line by line input.
char *line_Buffer;

// This variable will keep track of the line_Buffer index.
int line_Buffer_index;

// This buffer will be used to hold a token.
char *token_Buffer;

// This index will keep track of the token_Buffer index.
int token_Buffer_index = 0;

/* *****************************************************************************
 LINKED LISTED IMPLEMENTATION TO HOLD TOKENS
 ******************************************************************************* */
/* The linked list is the data structure I will use to store tokens.*/

int num_insertions = 0;

/* Define node for linked list */
struct node {
    char* value;
    struct node *next;
    Token type;
};

struct node *head = NULL; //head of linked list. Initially points to nothing
struct node *currentNode = NULL; // a pointer to the endmost node in list
struct node *node_for_parsing =NULL;

int numOfElements = 0; // number of elements

// Insert will add nodes to the end of the linked list.
void insert(char *value_To_Insert, Token tok) {
    
    if (numOfElements == 0) { //Creating the first node of the linked list.
        struct node *ptr = (struct node*)malloc(sizeof(struct node));
        if (ptr == NULL) {
            printf("Failed to allocate memory\n");
        }
        // ptr->value = malloc(sizeof(char)*100);
        ptr->value = strdup(value_To_Insert);
        ptr->next = NULL;
        ptr ->type = tok;
        
        head = ptr; // make the head point to this newly created node.
        currentNode = ptr;
        node_for_parsing = ptr;
        numOfElements++;
    }
    else { //elements are already in list
        struct node *ptr = (struct node*)malloc(sizeof(struct node));
        if (ptr == NULL) {
            printf("Failed to allocate memory\n");
        }
        ptr->value = strdup(value_To_Insert);
        ptr->next = NULL;
        ptr ->type = tok;
        currentNode->next = ptr; //make the endpoint node's link point to newly created node
        currentNode = ptr; // make this new node the endpoint
        numOfElements++;
    }
    num_insertions++;
}

int delete() { // this function will remove values from the front of the list
    
    struct node *deletionNode = head;
    
    if (deletionNode == NULL) {
        
        return -1;
        
    }
    
    
    head = deletionNode->next;
    node_for_parsing = deletionNode->next;
    if (head == NULL){ currentNode = NULL;}
    
    free(deletionNode);
    deletionNode = NULL;
    
    numOfElements--;
    
    return 0;
}

void delete_End_Of_List() {
    if (numOfElements == 1 || numOfElements == 0) { delete(); }
    else {
        struct node *temp =head;
        struct node *t;
        while(temp->next != NULL)
        {
            t=temp;
            temp=temp->next;
        }
        free(t->next);
        t->next=NULL;
        currentNode = t;
        numOfElements--;
    }
    
}

int how_Many() { // will return how many elements are in the list. For testing purposes
    
    return numOfElements;
}

// This function will iterate through the linked list and print the values stored in
// each node.
void printList(){
    struct node *ptr = head;
    
    while(ptr != NULL) {
        if (ptr->type == REAL) {
            printf("REAL: %s\n",ptr->value);
        }
        else if (ptr->type == INTEGER) {
            printf("INT: %s\n",ptr->value);
        }
        else if (ptr->type == IDENTIFIER) {
            printf("IDENTIFIER: %s\n",ptr->value);
        }
        else if (ptr->type == KEYWORD) {
            printf("KEYWORD: %s\n",ptr->value);
        }
        else if (ptr->type == BOOLEAN) {
            printf("BOOLEAN: %s\n",ptr->value);
        }
        else if (ptr->type == PUNCTUATION) {
            printf("PUNCTUATION: %s\n",ptr->value);
        }
        
        
        ptr = ptr->next;
    }
}


/* *****************************************************************************
 SCANNER IMPLEMENTATION
 ******************************************************************************* */




// This function will be used to reset the global line_buffer array.
void reset_Line_Buffer() {
    memset(line_Buffer,'\n',1000);
}

// This function will be used to reset the global token buffer array, and its index.
void reset_Token_Buffer() {
    
    memset(token_Buffer,'\0',100);
    token_Buffer_index = 0;
}

void clr_white_space(){
    while ((isspace(line_Buffer[line_Buffer_index])) != 0) {
        line_Buffer_index++;
        
    }
}

// This function will determine if input from line_Buff is a REAL number token or an INTEGER token.
// The function will return Tokens of type REAL or INTEGER if a valid token is found, otherwise the
// token of type ERROR will be returned.
Token is_Number() {
    
    // Use the isdigit function to check if the character of interest in the line_Buffer is a number.
    if(isdigit(line_Buffer[line_Buffer_index])) {
        
        // This loop will iterate while characters in the line_Buffer array are numbers.
        // The characters that are numbers will be stored in the token_Buffer array, to
        // start the creation of a possible INTEGER token or REAL token.
        while((isdigit(line_Buffer[line_Buffer_index])) != 0) {
            
            token_Buffer[token_Buffer_index] = line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
        }
        
        // This if statement will check the possibility of having a REAL number inputted.
        // If the program enters this if statement, then the token of type REAL will be returned.
        if(line_Buffer[line_Buffer_index] == '.') {
            
            // Store the decimal place in the token_Buffer array and increase the indexes
            // of both global arrays.
            token_Buffer[token_Buffer_index] = line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            // Now check if the characters following the '.' are numbers and store them
            while((isdigit(line_Buffer[line_Buffer_index])) != 0) {
                
                token_Buffer[token_Buffer_index] = line_Buffer[line_Buffer_index];
                line_Buffer_index++; token_Buffer_index++;
                
            }
            // Add the null character to the end of the token buffer to create a valid string
            // in C.
            token_Buffer[token_Buffer_index] = '\0';
            
            // Insert the token in the linked list.
            insert(token_Buffer,REAL);
            
            // return the REAL token
            return REAL;
            
        } else { // Go to the else statement if no decimal place was found.
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,INTEGER);
            return INTEGER;
        }
        
    } else if (line_Buffer[line_Buffer_index] == '.') {
        //This is the case if someone entered a real number with nothing before the decimal
        // If you're in this else if statement then REAL will be returned.
        
        // These lines of code will store the '.' and increment the global array indexes.
        token_Buffer[token_Buffer_index] = line_Buffer[line_Buffer_index];
        line_Buffer_index++; token_Buffer_index++;
        
        // Iterate through the line_Buffer array and store the characters representing numbers
        // in the token_Buffer array.
        if ((isdigit(line_Buffer[line_Buffer_index])) != 0) {
            while((isdigit(line_Buffer[line_Buffer_index])) != 0) {
                
                token_Buffer[token_Buffer_index] = line_Buffer[line_Buffer_index];
                line_Buffer_index++; token_Buffer_index++;
            }
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,REAL);
            return REAL;
        } else {
            return ERROR;
        }
        
    } else { // Return ERROR if input from line_Buffer did not meet the other requirements.
        return ERROR;
    }
    
    
}

// This function will be used in the is_Identifier().
// The function will take a string as input and see if it matches with the strings in the global
// keyword array.
int keyword_check(char *str) {
    int index = 0;
    
    while (index < 12) {
        if ((strcmp(str,keywords[index]))== 0) {
            return 1;
        }
        index++;
    }
    
    return -1;
    
}

// This function will check if characters in the line_Buffer make up an IDENTIFIER token or KEYWORD token.
// The function will return INDENTIFIER or KEYWORD if valid tokens are found,otherwise it will return false.
Token is_Identifier() {
    
    
    // First the function checks for chars a-z or A-Z
    if((line_Buffer[line_Buffer_index] >= 'a' && line_Buffer[line_Buffer_index] <= 'z')
       || (line_Buffer[line_Buffer_index] >= 'A' && line_Buffer[line_Buffer_index] <= 'Z')) {
        
        // store the beginning char.
        token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
        line_Buffer_index++; token_Buffer_index++;
        
        // Check if the following chars are from a-z and stores them
        while ((line_Buffer[line_Buffer_index] >= 'a' && line_Buffer[line_Buffer_index] <= 'z') ||
               (line_Buffer[line_Buffer_index] >= 'A' && line_Buffer[line_Buffer_index] <= 'Z') ||
               ((isdigit(line_Buffer[line_Buffer_index])) != 0)) {
            
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
        }
        
        token_Buffer[token_Buffer_index] = '\0';
        
        // this if statement will call the keyword_check function which returns one if the newly stored
        // token matches one of the keywords in the keyword array.
        if ((keyword_check(token_Buffer)) == 1) { insert(token_Buffer,KEYWORD); return KEYWORD; }
        else { insert(token_Buffer,IDENTIFIER); return IDENTIFIER; }
        
        
    }
    return ERROR;
}

// This function will determine if input in the line_Buffer is a token of type BOOLEAN
// The function will return the token BOOLEAN if it successfully finds the correct token otherwise
// the function returns the token ERROR.
Token is_Boolean(){
    if (line_Buffer[line_Buffer_index] == '#') {
        token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
        line_Buffer_index++; token_Buffer_index++;
        if (line_Buffer[line_Buffer_index] == 'f' || line_Buffer[line_Buffer_index] == 't') {
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,BOOLEAN);
            return BOOLEAN;
            
        }
    }
    
    return ERROR;
}

// This function will determine if the input from line buffer matches the punctuation of the given language
// Returns the token PUNCTUATION on succeess otherwise the function returns the token ERROR.
Token is_Punctuation() {
    
    
    // This function will match the char in line_Buffer with the punctuation chars with a switch statement
    // and then store them in the token buffer. Some cases will require additional checks as they are made
    // up of more than one chars.
    switch(line_Buffer[line_Buffer_index]) {
        case '(' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            break;
        case ')' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case '=' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case ';' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case '{' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
        case '}' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case ',' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case '+' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case '-' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case '*' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case '/' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case '%' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            token_Buffer[token_Buffer_index] = '\0';
            insert(token_Buffer,PUNCTUATION);
            return PUNCTUATION;
            
            break;
            
        case ':' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            if (line_Buffer[line_Buffer_index] == '=') {
                token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
                line_Buffer_index++; token_Buffer_index++;
                token_Buffer[token_Buffer_index] = '\0';
                insert(token_Buffer,PUNCTUATION);
                return PUNCTUATION;
                
            } else {line_Buffer_index--;  return ERROR;}
            break;
            
        case '!' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            if (line_Buffer[line_Buffer_index] == '=') {
                token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
                line_Buffer_index++; token_Buffer_index++;
                token_Buffer[token_Buffer_index] = '\0';
                insert(token_Buffer,PUNCTUATION);
                
                return PUNCTUATION;
                
            } else {line_Buffer_index--;  return ERROR; }
            
            break;
            
        case '<' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            if (line_Buffer[line_Buffer_index] == '=') {
                token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
                line_Buffer_index++; token_Buffer_index++;
                token_Buffer[token_Buffer_index] = '\0';
                insert(token_Buffer,PUNCTUATION);
                return PUNCTUATION;
                
            } else { token_Buffer[token_Buffer_index] = '\0';
                insert(token_Buffer,PUNCTUATION);
                return PUNCTUATION;
            }
            break;
            
        case '>' :
            token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
            line_Buffer_index++; token_Buffer_index++;
            
            if (line_Buffer[line_Buffer_index] == '=') {
                
                token_Buffer[token_Buffer_index]=  line_Buffer[line_Buffer_index];
                line_Buffer_index++; token_Buffer_index++;
                token_Buffer[token_Buffer_index] = '\0';
                insert(token_Buffer,PUNCTUATION);
                return PUNCTUATION;
                
            } else { token_Buffer[token_Buffer_index] = '\0';
                insert(token_Buffer,PUNCTUATION);
                return PUNCTUATION;
            }
            
            break;
            
            
        default : return ERROR;
            
    }
    return ERROR;
}

// This function will check if the global array line_buffer has valid tokens. If it does then it will return
// the token KEYWORD to indicate success, otherwise it returns the token ERROR to indicate failure.
Token scanner () {
    
    // check the
    while(line_Buffer[line_Buffer_index] != '\0') {
        //num_insertions = 0;
        //int num_tokens_added = 0;
        
        // These lines reset the token buffer to be used.
        token_Buffer = calloc(sizeof(char),100);
        reset_Token_Buffer();
        
        // This function will be used to keep track of the number of Errors returned from the functions
        // that identify specific tokens.
        // If error_value == 4, this indicates that the parameter input had invalid tokens.
        int error_value = 0;
        
        while ((isspace(line_Buffer[line_Buffer_index])) != 0) { // added this to deal with white space in front
            line_Buffer_index++;
            
        }
        
        if ( is_Number() == ERROR) {error_value++;}
        reset_Token_Buffer();
        
        
        if ( is_Identifier() == ERROR) {error_value++;}
        reset_Token_Buffer();
        
        if ( is_Boolean() == ERROR){error_value++;}
        
        reset_Token_Buffer();
        
        if ( is_Punctuation() == ERROR) {error_value++;}
        
        // this loop skips any white space encountered in the line_buffer array.
        while ((isspace(line_Buffer[line_Buffer_index])) != 0) {
            line_Buffer_index++;
            
        }
        // return error if all token checking functions fail.
        if (error_value == 4) {
            //printf("NUM OF INSERTIONS %d\n",num_insertions);
            return ERROR;}
    }
    return KEYWORD;
}
/* *****************************************************************************
 PARSER IMPLEMENTATION
 ******************************************************************************* */

typedef enum {ASSIGNMENT,IF,WHILE,RETURN,BLOCK,READ,WRITE,SKIP,ID,CALL,RELOP,ADDOP,MULOP,INT,FLOAT,BOOL,LOGIC,SIGN} type;

struct bt_node {
    char* data;
    struct bt_node *next;
    struct bt_node *child;
    type val;
};

typedef struct bt_node bt_node;



bt_node * new_node(char* data, type value)
{
    bt_node *new_node = malloc(sizeof(bt_node));
    
    if ( new_node ) {
        new_node->next = NULL;
        new_node->child = NULL;
        new_node->data = data;
        new_node->val = value;
    }
    
    return new_node;
}

bt_node * add_sibling(bt_node * n,bt_node * add)
{
    if ( n == NULL )
        return NULL;
    
    while (n->next)
        n = n->next;
    
    return (n->next = add);
}

bt_node * add_child(bt_node * n,bt_node *add)
{
    if ( n == NULL )
        return NULL;
    
    if ( n->child )
        return add_sibling(n->child, add);
    else
        return (n->child = add);
}


void print2DUtil(bt_node *root, int space)
{
    if (root == NULL)
        return;
    
    
    space += COUNT;
    
    
    print2DUtil(root->child, space);
    
    while (root != NULL) {
        printf("\n");
        for (int i = COUNT; i < space; i++)
            printf(" ");
        printf("%s\n", root->data);
        root = root -> next;
        if( root !=NULL) {
            print2DUtil(root->child, space);
        }
    }
    
}


void print2D(bt_node *root)
{
    print2DUtil(root, 0);
}


bt_node* program();
bt_node* id();
bt_node* bool_literal();
bt_node* int_literal();
bt_node* real_literal();
bt_node* constant();
bt_node* id_list();
bt_node* var();
bt_node* vars();
bt_node* func();
bt_node* funcs();
bt_node* cmd();
bt_node* cmds();
bt_node* block();
bt_node* expr();
bt_node* exprs();
bt_node* simple_expr();
bt_node* term();
bt_node* factor();
bt_node* not_op();
bt_node* sign_op();
bt_node* add_op();
bt_node* mul_op();
bt_node* rel_op();

void iterate(){
    node_for_parsing = node_for_parsing -> next;
}


bt_node* parser(){
    
    bt_node* tree = program();
    
    if (node_for_parsing != NULL) { return NULL;}
    
    return tree;
}

bt_node* program(){
    bt_node *head = new_node("PROGRAM",SKIP);
    add_child(head,vars());
    add_child(head,funcs());
    add_child(head,cmds());
    
    return head;
}

bt_node* vars() {
    bt_node* head = new_node("VARS",SKIP);
    
    while ( (node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"var")) == 0)) {
        add_child(head,var());
    }
    
    return head;
}

bt_node* var() {
    bt_node* head = new_node("VAR",SKIP);
    if ((node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"var")) == 0)) {
        iterate();
        
        bt_node* idlist = id_list();
        
        if  ((node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,";")) == 0)) {
            iterate();
            add_child(head,idlist);
        }
        
        
    }
    
    return head;
}

bt_node* id_list() {
    bt_node* head = id();
    
    while( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,",")) == 0)) {
        iterate();
        add_sibling(head,id());
    }
    
    return head;
}

bt_node* id() {
    bt_node *head = NULL;
    if ( (node_for_parsing != NULL) && (node_for_parsing->type == IDENTIFIER)) {
        head = new_node(node_for_parsing->value,ID);
        iterate();
    }
    
    return head;
}

bt_node* funcs(){
    bt_node* head = new_node("FUNCS",SKIP);
    
    while ( (node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"fun")) == 0)) {
        add_child(head,func());
    }
    
    return head;
    
}

bt_node* func() {
    bt_node* head = new_node("FUNC",SKIP);
    if( (node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"fun")) == 0)) {
        iterate();
        bt_node *name = id();
        if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"(")) == 0)) {
            
            bt_node *left_parenth = new_node("(",SKIP);
            iterate();
            bt_node *params = id_list();
            
            if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,")")) == 0)) {
                
                bt_node *right_parenth = new_node(")",SKIP);
                iterate();
                
                bt_node *bloc = block();
                
                add_child(head,name);
                add_child(head,left_parenth);
                add_child(head,params);
                add_child(head,right_parenth);
                add_child(head,bloc);
                
            }
        }
    }
    
    return head;
}

bt_node* block() {
    bt_node *head = new_node("BLOCK",BLOCK);
    int flag = 0;
    
    if ( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"{")) == 0)) {
        insert(";",PUNCTUATION); // use this to force failure is second brace is not included.
        iterate();
        bt_node *n = vars();
        bt_node *m = cmds();
        //flag++;
        // insert(";",PUNCTUATION); // use this to force failure is second brace is not included.
        //printf("Current node value: %s",node_for_parsing->value);
        
        if ( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"}")) == 0)) {
            delete_End_Of_List();
            iterate();
            add_child(head,n);
            add_child(head,m);
            flag++;
            
        } else {
            printf("INVALID BLOCK, THEREFORE PROGRAM WILL CALL FAILURE ON ;\n");
        }
    }
    
    return head;
    
    
    
}

bt_node* cmds(){
    bt_node* head = new_node("CMDS",SKIP);
    bt_node* ref = new_node("FILLER",SKIP);
    
    while ( (node_for_parsing != NULL) && ((node_for_parsing->type == KEYWORD) ||(node_for_parsing->type == IDENTIFIER && (node_for_parsing->next != NULL) && ((strcmp(":=",node_for_parsing->next->value) == 0) )) || ((node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"{") == 0)))))  {
        
        ref = cmd();
        add_child(head,ref);
    }
    return head;
}

bt_node* cmd(){
    bt_node* head = new_node("CMD",SKIP);
    bt_node* fail = NULL;
    
    if ((node_for_parsing != NULL)&& (node_for_parsing->type == IDENTIFIER) &&(node_for_parsing->next != NULL) && ((strcmp(node_for_parsing->next->value,":="))==0)) {
        bt_node* id1 = id();
        if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,":=")) == 0)) {
            bt_node* opr = new_node(":=",ASSIGNMENT);
            iterate();
            if( (node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"call")) == 0)) {
                bt_node* call = new_node("call",CALL);
                iterate();
                bt_node* id2 = id();
                
                if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"(")) == 0)) {
                    bt_node* left_parenth = new_node("(",SKIP);
                    iterate();
                    bt_node* params = exprs();
                    
                    if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,")")) == 0)) {
                        bt_node* right_parenth = new_node(")",SKIP);
                        iterate();
                        
                        if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,";")) == 0)) {
                            iterate();
                            
                            add_child(head,opr);
                            add_child(opr,id1);
                            add_child(opr,id2);
                            add_child(opr, left_parenth);
                            add_child(opr,params);
                            add_child(opr, right_parenth);
                            
                        }
                    }
                }
            } else {
                
                bt_node* params = expr();
                if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,";")) == 0)) {
                    iterate();
                    add_child(head,opr);
                    add_child(opr,id1);
                    add_child(opr,params);
                }
                
            }
        }
    } else if ((node_for_parsing != NULL)&& (node_for_parsing->type == KEYWORD )) {
        if ((strcmp(node_for_parsing->value,"if")) == 0) {
            bt_node* i_f = new_node("if",IF);
            iterate();
            
            if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"(")) == 0)) {
                iterate();
                
                bt_node* paras = expr();
                
                if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,")")) == 0))  {
                    iterate();
                    
                    bt_node* bloc = block();
                    
                    add_child(head,i_f);
                    add_child(i_f,paras);
                    add_child(i_f,bloc);
                }
            }
        } else if ((strcmp(node_for_parsing->value,"while")) == 0) {
            bt_node* whil = new_node("while",WHILE);
            iterate();
            
            if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"(")) == 0)) {
                iterate();
                
                bt_node* paras = expr();
                
                if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,")")) == 0))  {
                    iterate();
                    
                    bt_node* bloc = block();
                    
                    add_child(head,whil);
                    add_child(whil,paras);
                    add_child(whil,bloc);
                }
            }
            
        }else if ((strcmp(node_for_parsing->value,"return")) == 0) {
            
            bt_node* ret = new_node("return",RETURN);
            iterate();
            bt_node* paras = expr();
            if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,";")) == 0)) {
                iterate();
                add_child(head,ret);
                add_child(ret,paras);
            }
            
            
        } else if ((strcmp(node_for_parsing->value,"read")) == 0) {
            
            bt_node* read = new_node("read",READ);
            iterate();
            bt_node* paras = id();
            if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,";")) == 0)) {
                iterate();
                add_child(head,read);
                add_child(read,paras);
            }
            
        }else if ((strcmp(node_for_parsing->value,"write")) == 0) {
            bt_node* write = new_node("write",WRITE);
            iterate();
            bt_node* paras = id();
            if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,";")) == 0)) {
                iterate();
                add_child(head,write);
                add_child(write,paras);
            }
            
            
        }
    } else if ((node_for_parsing != NULL)&& (node_for_parsing->type == PUNCTUATION )) {
        if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"{")) == 0)) {
            bt_node* bloc = block();
            
            add_child(head,bloc);
        }
    } else {
        head = NULL;
    }
    
    
    return head;
}

bt_node* exprs(){
    bt_node* head = expr();
    
    while( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,",")) == 0)) {
        iterate();
        add_sibling(head,expr());
    }
    
    return head;
}

bt_node* expr() {
    bt_node* head = new_node("EXPR",SKIP);
    bt_node* a = simple_expr();
    if ( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) &&
        (((strcmp(node_for_parsing->value,"=")) == 0) ||
         ((strcmp(node_for_parsing->value,"!=")) == 0) ||
         ((strcmp(node_for_parsing->value,"<")) == 0) ||
         ((strcmp(node_for_parsing->value,">")) == 0) ||
         ((strcmp(node_for_parsing->value,"<=")) == 0)||
         ((strcmp(node_for_parsing->value,">=")) == 0))) {
            bt_node *opr = new_node(node_for_parsing->value,RELOP);
            iterate();
            bt_node* b = simple_expr();
            
            add_child(opr,a);
            add_child(opr,b);
            
            add_child(head,opr);
        } else {
            add_child(head,a);
        }
    
    return head;
}

bt_node* simple_expr(){
    bt_node* sign = NULL;
    if ( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && (((strcmp(node_for_parsing->value,"+") ) == 0)|| (strcmp(node_for_parsing->value,"-")  == 0))){
        sign = sign_op();
    }
    bt_node* temp =  NULL;
    
    bt_node* head = term();
    if (sign != NULL) { add_sibling(sign,head); head = sign; }
    //if ( sign != NULL) { temp = head; head = sign;}
    while( (node_for_parsing != NULL) && (
                                          ((node_for_parsing->type == PUNCTUATION) && (((strcmp(node_for_parsing->value,"+")) == 0) ||((strcmp(node_for_parsing->value,"-")) == 0))) || ((node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"or")) == 0) )))
    {
        
        bt_node* plus = add_op();
        bt_node* ter2 = term();
        
        //if ( sign != NULL) { add_child(plus,head); add_sibling(head,temp);} else
        //{
        //if (sign != NULL) { add_child(plus,sign);}
        add_child(plus,head);
        //}
        add_child(plus,ter2);
        head = plus;
    }
    
    //if (sign != NULL)  { add_sibling(sign,head); head = sign;}
    
    return head;
}

bt_node* term(){
    bt_node* head = factor();
    while( (node_for_parsing != NULL) &&
          (((node_for_parsing->type == PUNCTUATION) && (((strcmp(node_for_parsing->value,"*")) == 0) ||((strcmp(node_for_parsing->value,"/")) == 0) || ((strcmp(node_for_parsing->value,"%")) == 0))) ||((node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"and")) == 0) )) ) {
        bt_node* mul = mul_op();
        
        bt_node* fac2 = factor();
        add_child(mul,head);
        add_child(mul,fac2);
        head = mul;
    }
    
    return head;
}

bt_node* factor(){
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == BOOLEAN ) ||(node_for_parsing->type == INTEGER ) || (node_for_parsing->type == REAL ) )) {
        head = constant();
    }
    else if( (node_for_parsing != NULL) && (node_for_parsing->type == IDENTIFIER) ) {
        head = id();
    }
    else if( (node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"not")) == 0)) {
        bt_node* not = not_op();
        head = factor();
        
        add_sibling(not,head);
        head = not;
    }
    else if ( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"(")) == 0)){
        iterate();
        bt_node* e = expr();
        
        if ( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,")")) == 0)) {
            iterate();
            head = e;
        }
        
    }
    
    
    return head;
}

bt_node* constant() {
    bt_node* head =NULL;
    if ( (node_for_parsing != NULL) && (node_for_parsing->type == BOOLEAN )) {
        
        head = bool_literal();
        
    } else if ( (node_for_parsing != NULL) && (node_for_parsing->type == REAL )) {
        head = real_literal();
    } else if ( (node_for_parsing != NULL) && (node_for_parsing->type == INTEGER )) {
        head = int_literal();
    }
    
    return head;
}

bt_node* bool_literal(){
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == BOOLEAN )) && ((strcmp(node_for_parsing->value,"#t")) == 0)) {
        head = new_node("#t",BOOL);
        iterate();
    } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == BOOLEAN )) && ((strcmp(node_for_parsing->value,"#f")) == 0))  {
        head = new_node("#f",BOOL);
        iterate();
        
    }
    return head;
}


bt_node* int_literal() {
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == INTEGER ))) {
        head = new_node(node_for_parsing->value,INT);
        iterate();
    }
    return head;
}

bt_node* real_literal() {
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == REAL ))) {
        head = new_node(node_for_parsing->value,FLOAT);
        iterate();
    }
    return head;
}



bt_node* not_op() {
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == KEYWORD)) && ((strcmp(node_for_parsing->value,"not")) == 0)) {
        head = new_node("not",LOGIC);
        iterate();
    }
    
    return head;
}

bt_node* sign_op(){
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"+")) == 0)) {
        head = new_node("+",SIGN);
        iterate();
    } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"-")) == 0))  {
        head = new_node("-",SIGN);
        iterate();
        
    }
    return head;
}

bt_node* add_op(){
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"+")) == 0)) {
        head = new_node("+",ADDOP);
        iterate();
    } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"-")) == 0))  {
        head = new_node("-",ADDOP);
        iterate();
        
    }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == KEYWORD )) && ((strcmp(node_for_parsing->value,"or")) == 0)) {
        head = new_node("or",ADDOP);
        iterate();
    }
    return head;
}

bt_node* mul_op(){
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"*")) == 0)) {
        head = new_node("*",MULOP);
        iterate();
    } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"/")) == 0))  {
        head = new_node("/",MULOP);
        iterate();
        
    }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,"%")) == 0)) {
        head = new_node("%",MULOP);
        iterate();
    }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == KEYWORD)) && ((strcmp(node_for_parsing->value,"and")) == 0)) {
        head = new_node("and",MULOP);
        iterate();
    }
    return head;
}

bt_node* rel_op(){
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"=")) == 0)) {
        head = new_node("=",RELOP);
        iterate();
    } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"!=")) == 0))  {
        head = new_node("!=",RELOP);
        iterate();
        
    }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,"<")) == 0)) {
        head = new_node("<",RELOP);
        iterate();
    }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,">")) == 0)) {
        head = new_node(">",RELOP);
        iterate();
    }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,"<=")) == 0)) {
        head = new_node("<=",RELOP);
        iterate();
    }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,">=")) == 0)) {
        head = new_node(">=",RELOP);
        iterate();
    }
    
    
    return head;
}


/* *****************************************************************************
 PARSER IMPLEMENTATION END
 ******************************************************************************* */

/* typedef enum {ASSIGNMENT,IF,WHILE,RETURN,BLOCK,READ,WRITE,SKIP,ID,CALL,RELOP,ADDOP,MULOP,INT,FLOAT,BOOL,LOGIC,SIGN} type;*/

struct data {
    int x; float y; char z; int flag; // 1= int, 2 = float, 3 = bool
};

typedef struct data data;
struct symbol_table_node{
    
    struct symbol_table_node* next; // pointer to another symbol table. Inner scope if need be.
    char* name;
   // int flag; // to indicate which value is being used in union. 1 = int, 2 = double 3 = bool 4 = func
    data* info;
};

typedef struct symbol_table_node st;

void print_st(st* a) {
    st* temp  = a;
    
    while(temp != NULL) {
        if (temp -> info -> flag == 1)  {// INT
            printf("%s := %d\n",temp->name,temp->info->x);
        } else if (temp -> info -> flag == 2) {
            printf("%s := %f\n", temp->name,temp->info->y);
        } else if (temp -> info -> flag == 3) {
            printf("%s := %c\n", temp->name,temp->info->z);

        }
        
         temp = temp -> next;
    }
}
data* less_equal(data* LHS, data* RHS) {
    data* a = NULL;
    char flag = 'f';
    if (LHS->flag == 1 && RHS->flag == 1) {
        if (LHS->x <= RHS->x) flag = 't';
        
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        if ((float)LHS->x <= RHS->y) flag = 't';
        
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        if (LHS->y <= (float)RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        if (LHS->y <= RHS->y) flag = 't';
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (LHS->x <= stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (stor <= RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        if (LHS->y <= stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        if (stor <= RHS->y) flag = 't';
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        if (stor1 <= stor2) flag = 't';
    }
    a = (data*) malloc(sizeof(data));
    a -> flag = 3;
    a -> z = flag;
    return a;
    
}
data* equal(data* LHS, data* RHS) {
    data* a = NULL;
    char flag = 'f';
    if (LHS->flag == 1 && RHS->flag == 1) {
        if (LHS->x == RHS->x) flag = 't';
        
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        if ((float)LHS->x == RHS->y) flag = 't';
        
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        if (LHS->y == (float)RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        if (LHS->y == RHS->y) flag = 't';
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (LHS->x == stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (stor == RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        if (LHS->y == stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        if (stor == RHS->y) flag = 't';
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        if (stor1 == stor2) flag = 't';
    }
    a = (data*) malloc(sizeof(data));
    a -> flag = 3;
    a -> z = flag;
    return a;
    
}
data* not_equal(data* LHS, data* RHS) {
    data* a = NULL;
    char flag = 'f';
    if (LHS->flag == 1 && RHS->flag == 1) {
        if (LHS->x != RHS->x) flag = 't';
        
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        if ((float)LHS->x != RHS->y) flag = 't';
        
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        if (LHS->y != (float)RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        if (LHS->y != RHS->y) flag = 't';
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (LHS->x != stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (stor != RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        if (LHS->y != stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        if (stor != RHS->y) flag = 't';
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        if (stor1 != stor2) flag = 't';
    }
    a = (data*) malloc(sizeof(data));
    a -> flag = 3;
    a -> z = flag;
    return a;
    
}

data* less(data* LHS, data* RHS) {
    data* a = NULL;
    char flag = 'f';
    if (LHS->flag == 1 && RHS->flag == 1) {
        if (LHS->x < RHS->x) flag = 't';
        
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        if ((float)LHS->x < RHS->y) flag = 't';
        
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        if (LHS->y < (float)RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        if (LHS->y < RHS->y) flag = 't';
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (LHS->x < stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (stor < RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        if (LHS->y < stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        if (stor < RHS->y) flag = 't';
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        if (stor1 < stor2) flag = 't';
    }
    a = (data*) malloc(sizeof(data));
    a -> flag = 3;
    a -> z = flag;
    return a;
    
}
data* greater(data* LHS, data* RHS) {
    data* a = NULL;
    char flag = 'f';
    if (LHS->flag == 1 && RHS->flag == 1) {
        if (LHS->x > RHS->x) flag = 't';
        
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        if ((float)LHS->x > RHS->y) flag = 't';
        
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        if (LHS->y > (float)RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        if (LHS->y > RHS->y) flag = 't';
     }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (LHS->x > stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
      if (stor > RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        if (LHS->y > stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        if (stor > RHS->y) flag = 't';
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        if (stor1 > stor2) flag = 't';
    }
    a = (data*) malloc(sizeof(data));
    a -> flag = 3;
    a -> z = flag;
    return a;
    
}
data* greater_equal(data* LHS, data* RHS) {
    data* a = NULL;
    char flag = 'f';
    if (LHS->flag == 1 && RHS->flag == 1) {
        if (LHS->x >= RHS->x) flag = 't';
        
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        if ((float)LHS->x >= RHS->y) flag = 't';
        
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        if (LHS->y >= (float)RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        if (LHS->y >= RHS->y) flag = 't';
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (LHS->x >= stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        if (stor >= RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        if (LHS->y >= stor) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        if (stor >= RHS->y) flag = 't';
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        if (stor1 >= stor2) flag = 't';
    }
    a = (data*) malloc(sizeof(data));
    a -> flag = 3;
    a -> z = flag;
    return a;
    
}
data* or(data* LHS, data* RHS) {
    data* a = NULL;
    char flag = 'f';
    if (LHS->flag == 1 && RHS->flag == 1) {
        if (LHS->x || RHS->x) flag = 't';
      
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        if (LHS->x || RHS->y) flag = 't';

    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        if (LHS->y || RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        if (LHS->y || RHS->y) flag = 't';
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
       if (LHS->x || (RHS->z == 't')) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        if ((LHS->z == 't') || RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
       if (LHS->y || (RHS->z == 't')) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        if ((LHS->z == 't') || RHS->y) flag = 't';
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
         if ((LHS->z == 't') || (RHS->z == 't') ) flag = 't';
    }
    a = (data*) malloc(sizeof(data));
    a -> flag = 3;
    a -> z = flag;
    return a;
    
}
data* and(data* LHS, data* RHS) {
    data* a = NULL;
    char flag = 'f';
    if (LHS->flag == 1 && RHS->flag == 1) {
        if (LHS->x && RHS->x) flag = 't';
        
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        if (LHS->x && RHS->y) flag = 't';
        
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        if (LHS->y && RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        if (LHS->y && RHS->y) flag = 't';
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        if (LHS->x && (RHS->z == 't')) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        if ((LHS->z == 't') && RHS->x) flag = 't';
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        if (LHS->y && (RHS->z == 't')) flag = 't';
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        if ((LHS->z == 't') && RHS->y) flag = 't';
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        if ((LHS->z == 't') && (RHS->z == 't') ) flag = 't';
    }
    a = (data*) malloc(sizeof(data));
    a -> flag = 3;
    a -> z = flag;
    return a;
    
}


data* sub(data* LHS, data* RHS) {
    data* a = NULL;
    if (LHS->flag == 1 && RHS->flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = LHS->x - RHS->x;
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = (float)LHS->x - RHS->y;
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = LHS->y - (float)RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = LHS->y - RHS->y;
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = LHS->x - stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = stor - RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = LHS->y - stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = stor - RHS->y;
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = stor1 - stor2;
    }
    
    return a;
    
}
data* add(data* LHS, data* RHS) {
    data* a = NULL;
    if (LHS->flag == 1 && RHS->flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = LHS->x + RHS->x;
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = (float)LHS->x + RHS->y;
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = LHS->y + (float)RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = LHS->y + RHS->y;
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = LHS->x + stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = stor + RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = LHS->y + stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = stor + RHS->y;
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = stor1 + stor2;
    }
    
    return a;

}

data* not(data* a) {
    data *b;
    char boolean = 't';
    if (a -> flag == 1) {
        if (a -> x) boolean = 'f';
    } else if ( a -> flag == 2) {
        if (a -> y) boolean = 'f';
    } else if (a -> flag == 3) {
        if (a -> z == 't') {boolean = 'f';}
        else { boolean = 't';}
    }
    b = (data*) malloc(sizeof(data));
    b -> flag = 3;
    b -> z = boolean;
    
    return b;
}
data* modulo(data* LHS, data* RHS) {
    data* a = NULL;
    if (LHS->flag == 1 && RHS->flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = LHS->x % RHS->x;
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = LHS->x % (int)RHS->y;
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = (int)LHS->y % RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = (int)LHS->y % (int)RHS->y;
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = LHS->x % stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = stor % RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = (int)LHS->y % stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = stor % (int)RHS->y;
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = stor1 % stor2;
    }
    
    return a;
    
}

data* mul(data* LHS, data* RHS) {
    data* a = NULL;
    if (LHS->flag == 1 && RHS->flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = LHS->x * RHS->x;
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = (float)LHS->x * RHS->y;
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = LHS->y * (float)RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = LHS->y * RHS->y;
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = LHS->x * stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = stor * RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = LHS->y * stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = stor * RHS->y;
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = stor1 * stor2;
    }
    
    return a;
    
}

data* divide(data* LHS, data* RHS) {
    data* a = NULL;
    if (LHS->flag == 1 && RHS->flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        
        if (RHS->x == 0) {printf("TRYING TO DIVIDE BY ZERO. THIS IS NOT ALLOWED\n."); a =NULL; return a;}
        a -> x = LHS->x / RHS->x;
    } else if ( LHS ->flag == 1 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        if (RHS->y == (float)0.0) {printf("TRYING TO DIVIDE BY ZERO. THIS IS NOT ALLOWED\n."); a =NULL;return a;}
        a -> y = (float)LHS->x / RHS->y;
    } else if ( LHS ->flag == 2 && RHS -> flag == 1) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        if (RHS->x == 0) {printf("TRYING TO DIVIDE BY ZERO. THIS IS NOT ALLOWED\n."); a =NULL;return a;}
        a -> y = LHS->y / (float)RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 2) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        if (RHS->y == (float)0.0) {printf("TRYING TO DIVIDE BY ZERO. THIS IS NOT ALLOWED\n."); a =NULL;return a;}
        a -> y = LHS->y / RHS->y;
    }else if ( LHS ->flag == 1 && RHS -> flag == 3) {
        int stor = 0;
        if (RHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        if (stor == 0) {printf("TRYING TO DIVIDE BY ZERO. THIS IS NOT ALLOWED\n."); a =NULL;return a;}

        a -> x = LHS->x / stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 1) {
        int stor = 0;
        if (LHS -> z == 't') { stor = 1;} else {stor = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        if (RHS->x == 0) {printf("TRYING TO DIVIDE BY ZERO. THIS IS NOT ALLOWED\n."); a =NULL;return a;}
        a -> x = stor / RHS->x;
    } else if ( LHS ->flag == 2 && RHS -> flag == 3) {
        float stor = 0.0;
        if (RHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        if (stor == 0) {printf("TRYING TO DIVIDE BY ZERO. THIS IS NOT ALLOWED\n.");a =NULL; return a;}
        

        a -> y = LHS->y / stor;
    } else if ( LHS ->flag == 3 && RHS -> flag == 2) {
        float stor = 0.0;
        if (LHS -> z == 't') { stor = 1.0;} else {stor = 0.0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        if (RHS->y == (float)0.0) {printf("TRYING TO DIVIDE BY ZERO. THIS IS NOT ALLOWED\n.");a =NULL; return a;}

        a -> y = stor / RHS->y;
    } else if (LHS -> flag == 3 && RHS -> flag== 3) {
        int stor1 = 0; int stor2 = 0;
        if (LHS -> z == 't') { stor1 = 1;} else {stor1 = 0;}
        if (RHS -> z == 't') { stor2 = 1;} else {stor2 = 0;}
        
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        if (stor2 == 0) {printf("TRYING TO DIVIDE BY ZERO. THIS IS NOT ALLOWED\n.");a =NULL; return a;}
        a -> x = stor1 / stor2;
    }
    
    return a;
    
}

data* eval_expr(bt_node* node,st *head) {
    data* a =NULL;
    if (node->val == INT) {
        //printf("HELLO\n");
        a = (data*) malloc(sizeof(data));
        a -> flag = 1;
        a -> x = atoi(node -> data);
    } else if (node -> val == FLOAT) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 2;
        a -> y = atof(node -> data);
    } else if ( node -> val == BOOL) {
        a = (data*) malloc(sizeof(data));
        a -> flag = 3;
        
        if (strcmp("#t",node-> data) == 0) {
             a -> z = 't';
        } else {
            a -> z = 'f';
        }
        
    } else if ( node -> val == ADDOP) {
        bt_node* t = node;data* RHS;
        data* LHS = eval_expr(node->child,head);
        if (node -> child -> val == SIGN ) {
            RHS = eval_expr(node->child->next->next,head); } else { RHS = eval_expr(node->child->next,head); }
        
        if (strcmp(node ->data,"+") == 0 ) { a = add(LHS,RHS);}
        else if (strcmp(node ->data,"-") == 0 ){ a = sub(LHS,RHS);}
        else if (strcmp(node->data,"or") == 0) {a = or(LHS,RHS);}


    } else if (node -> val == RELOP) {
        bt_node* t = node;data* RHS;
        data* LHS = eval_expr(node->child,head);
        if (node -> child -> val == SIGN ) {
            RHS = eval_expr(node->child->next->next,head); } else { RHS = eval_expr(node->child->next,head); }
        if(strcmp(node ->data,">") == 0 ) { a = greater(LHS,RHS);}
        else if(strcmp(node ->data,"<") == 0 ) { a = less(LHS,RHS);}
        else if (strcmp(node ->data,"<=") == 0 ) { a = less_equal(LHS,RHS);}
        else if (strcmp(node ->data,">=") == 0 ) { a = greater_equal(LHS,RHS);}
        else if (strcmp(node ->data,"!=") == 0 ) { a = not_equal(LHS,RHS);}
        else if (strcmp(node ->data,"=") == 0 ) { a = equal(LHS,RHS);}

                
    } else if ( node -> val == LOGIC) {

        if(strcmp(node ->data,"not") == 0 ) {
            bt_node* t = node;
            data* arg = eval_expr(node->next,head);
            a = not(arg);
        }
    } else if (node -> val == MULOP) {
        bt_node* t = node;data* RHS;
        data* LHS = eval_expr(node->child,head);
        if (node -> child -> val == SIGN ) {
            RHS = eval_expr(node->child->next->next,head); } else { RHS = eval_expr(node->child->next,head); }
        if(strcmp(node ->data,"*") == 0 ) { a = mul(LHS,RHS);}
        else if (strcmp(node ->data,"/") == 0) { a = divide(LHS,RHS);}
        else if (strcmp(node ->data,"%") == 0) { a = modulo(LHS,RHS);}
        else if (strcmp(node ->data,"and") == 0) { a = and(LHS,RHS);}

        
    }else if (node -> val == SIGN) {
        int one = 1;
        if (strcmp(node ->data,"-") == 0 ) { one = -1;}
        a = eval_expr(node->next,head);
       // printf("HERE I AM\n");
        if (a -> flag == 3) {
            //printf("CANNONT CHANGE THE SIGN OF A BOOLEAN VALUE\n");
            if (a -> z == 't') {
                a -> flag = 1;
                a -> x = 1 * one;
            } else {
                a -> flag = 1;
                a -> x = 0;
            }

            
        } else if (a->flag == 2) {
            a -> y = a -> y * one;
        } else {
            
             a -> x = a -> x * one;
        }
    } else if (node -> val == ID) {
        st* temp = head;
        
        while ( temp != NULL) {
            if ( strcmp(node -> data,temp->name) == 0) { a = temp -> info; break;}
            temp = temp -> next;
        }
    } else if (node -> val == SKIP) {
        //printf("HERE I AM\n");
        a = eval_expr(node -> child,head);
    }
    
    return a;
}

st* process_assignment(bt_node* node,st *head ){
    st* temp = head;
    st* a = NULL;
    int flag = 0;
    while (temp != NULL) {if (strcmp(temp->name,node->child->data) == 0) { flag = 1; break;} temp = temp -> next;}
    
    if ( flag) {
        temp -> info = eval_expr(node -> child -> next->child,head); return a;

    } else {
     a = (st*) malloc(sizeof(st));
   
    a -> name = strdup(node -> child -> data);
    a -> next = NULL;
    a -> info = eval_expr(node -> child -> next->child,head);
   
    }
    return a;
}

st* eval_cmd(bt_node* node,st *head) {
    bt_node* temp = node;
    temp = temp -> child;
    st* ret = NULL;
    
    if (temp-> val == ASSIGNMENT) {
        ret = process_assignment(temp,head);
    } else if ( temp -> val == READ) {
        printf("NOTHING\n");
    }
    
    return ret;
}



st* evaluate(bt_node* root){
    if (strcmp("PROGRAM",root->data) == 0) {
        bt_node* cp = root;
        cp = cp -> child;
        cp = cp -> next;
        cp = cp -> next;
        int flag = 0;
        
        if (cp != NULL) {
            // printf("%s\n",cp->data);
            if ( strcmp("CMDS",cp->data) == 0) {
               
                cp = cp -> child;
                st* a = NULL;
                st* b = NULL;
                while (cp != NULL) {
                    //printf("HELLO\n");
                    if ( a == NULL) { a = eval_cmd(cp,b); b = a;}
                    else {
                        st * c = eval_cmd(cp,b);
                        
                        if ( c != NULL) {
                            a -> next = c;
                        a = a-> next;
                        }
                    }
                    cp = cp -> next;
                }
                return b;

            }
        }}}

        


int main() {
    // initialize the global array
    line_Buffer = calloc(sizeof(char),1000);
    int flag = 1;
    // this loop stores input from stdin into the global array. *Note input can be maximum of 1000 chars.
    while ((fgets(line_Buffer, 1000, stdin)) != NULL ) {
        
        // if a new line is encountered, exit the program
        //if (line_Buffer[0] == '\n') break;
        
        printf("ORIGINAL INPUT: %s",line_Buffer);
        clr_white_space();
        // scan the input for valid tokens
        
        if (line_Buffer[line_Buffer_index] != '\0') {
        num_insertions = 0;
        if ( scanner() != ERROR) { // if scanner is successful list tokens making up the input
            
            
        } else {
            printf("INVALID LINE INPUT: %s\n",line_Buffer);
            
            flag = 0; break;
            
        }
        
        reset_Line_Buffer();
        line_Buffer_index=0;
        
        }
    }
    if (flag) {
        printList();
        bt_node* tree = parser();
        
        if (tree == NULL){ printf("PARSE FAILED on the following TOKEN: %s\n",node_for_parsing->value); }
        else{ printf("SUCCESSFUL PARSE\n");  print2D(tree); printf("\n");st* a = evaluate(tree);
            if ( a == NULL) {
                printf("INTERPREATION FAILED\n");
            }
            else {printf("SYMBOL TABLE\n");print_st(a);
            }}
    }
    
    //int a = 5/0;
    
}

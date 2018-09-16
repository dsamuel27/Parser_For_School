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


struct bt_node {
    char* data;
    struct bt_node *next;
    struct bt_node *child;
};

typedef struct bt_node bt_node;



bt_node * new_node(char* data)
{
    bt_node *new_node = malloc(sizeof(bt_node));
    
    if ( new_node ) {
        new_node->next = NULL;
        new_node->child = NULL;
        new_node->data = data;
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
    bt_node *head = new_node("PROGRAM");
    add_child(head,vars());
    add_child(head,funcs());
    add_child(head,cmds());
    
    return head;
}

bt_node* vars() {
    bt_node* head = new_node("VARS");
    
    while ( (node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"var")) == 0)) {
        add_child(head,var());
    }
    
    return head;
}

bt_node* var() {
    bt_node* head = new_node("VAR");
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
        head = new_node(node_for_parsing->value);
        iterate();
    }
    
    return head;
}

bt_node* funcs(){
    bt_node* head = new_node("FUNCS");
    
    while ( (node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"fun")) == 0)) {
        add_child(head,func());
    }
    
    return head;

}

bt_node* func() {
    bt_node* head = new_node("FUNC");
    if( (node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"fun")) == 0)) {
        iterate();
        bt_node *name = id();
        if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"(")) == 0)) {
            
            bt_node *left_parenth = new_node("(");
            iterate();
            bt_node *params = id_list();
            
            if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,")")) == 0)) {
                
                    bt_node *right_parenth = new_node(")");
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
    bt_node *head = new_node("BLOCK");
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
    bt_node* head = new_node("CMDS");
    bt_node* ref = new_node("FILLER");
    
    while ( (node_for_parsing != NULL) && ((node_for_parsing->type == KEYWORD) ||(node_for_parsing->type == IDENTIFIER && (node_for_parsing->next != NULL) && ((strcmp(":=",node_for_parsing->next->value) == 0) )) || ((node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"{") == 0)))))  {
               
               ref = cmd();
        add_child(head,ref);
    }
         return head;
}

bt_node* cmd(){
    bt_node* head = new_node("CMD");
    bt_node* fail = NULL;
    
    if ((node_for_parsing != NULL)&& (node_for_parsing->type == IDENTIFIER) &&(node_for_parsing->next != NULL) && ((strcmp(node_for_parsing->next->value,":="))==0)) {
        bt_node* id1 = id();
        if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,":=")) == 0)) {
            bt_node* opr = new_node(":=");
            iterate();
             if( (node_for_parsing != NULL) && (node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"call")) == 0)) {
                    bt_node* call = new_node("call");
                    iterate();
                    bt_node* id2 = id();
                 
                 if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,"(")) == 0)) {
                     bt_node* left_parenth = new_node("(");
                     iterate();
                     bt_node* params = exprs();
                     
                     if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,")")) == 0)) {
                         bt_node* right_parenth = new_node(")");
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
            bt_node* i_f = new_node("if");
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
            bt_node* whil = new_node("while");
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
           
            bt_node* ret = new_node("return");
             iterate();
            bt_node* paras = expr();
            if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,";")) == 0)) {
                iterate();
                add_child(head,ret);
                add_child(ret,paras);
            }
                
            
        } else if ((strcmp(node_for_parsing->value,"read")) == 0) {
            
            bt_node* read = new_node("read");
            iterate();
            bt_node* paras = id();
            if( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) && ((strcmp(node_for_parsing->value,";")) == 0)) {
                iterate();
                add_child(head,read);
                add_child(read,paras);
            }

        }else if ((strcmp(node_for_parsing->value,"write")) == 0) {
            bt_node* write = new_node("write");
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
    bt_node* head = new_node("EXPR");
    bt_node* a = simple_expr();
    if ( (node_for_parsing != NULL) && (node_for_parsing->type == PUNCTUATION) &&
        (((strcmp(node_for_parsing->value,"=")) == 0) ||
         ((strcmp(node_for_parsing->value,"!=")) == 0) ||
         ((strcmp(node_for_parsing->value,"<")) == 0) ||
         ((strcmp(node_for_parsing->value,">")) == 0) ||
         ((strcmp(node_for_parsing->value,"<=")) == 0)||
         ((strcmp(node_for_parsing->value,">=")) == 0))) {
            bt_node *opr = new_node(node_for_parsing->value);
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
  
    bt_node* head = term();
    while( (node_for_parsing != NULL) && (
    ((node_for_parsing->type == PUNCTUATION) && (((strcmp(node_for_parsing->value,"+")) == 0) ||((strcmp(node_for_parsing->value,"-")) == 0))) || ((node_for_parsing->type == KEYWORD) && ((strcmp(node_for_parsing->value,"or")) == 0) )))
                                           {
                                             
        bt_node* plus = add_op();
        bt_node* ter2 = term();
        add_child(plus,head);
        add_child(plus,ter2);
        head = plus;
    }
    
    if (sign != NULL)  { add_sibling(sign,head); head = sign;}
    
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
        head = new_node("#t");
        iterate();
    } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == BOOLEAN )) && ((strcmp(node_for_parsing->value,"#f")) == 0))  {
        head = new_node("#f");
        iterate();
        
    }
    return head;
}
 
                        
bt_node* int_literal() {
        bt_node* head = NULL;
       if ( (node_for_parsing != NULL) && ((node_for_parsing->type == INTEGER ))) {
           head = new_node(node_for_parsing->value);
           iterate();
        }
    return head;
}

bt_node* real_literal() {
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == REAL ))) {
    head = new_node(node_for_parsing->value);
    iterate();
    }
    return head;
}
  

                        
bt_node* not_op() {
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == KEYWORD)) && ((strcmp(node_for_parsing->value,"not")) == 0)) {
            head = new_node("not");
            iterate();
        }
        
        return head;
}
                        
bt_node* sign_op(){
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"+")) == 0)) {
            head = new_node("+");
            iterate();
        } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"-")) == 0))  {
            head = new_node("-");
            iterate();
            
    }
    return head;
}
    
bt_node* add_op(){
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"+")) == 0)) {
            head = new_node("+");
            iterate();
        } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"-")) == 0))  {
            head = new_node("-");
            iterate();
            
        }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == KEYWORD )) && ((strcmp(node_for_parsing->value,"or")) == 0)) {
            head = new_node("or");
            iterate();
        }
    return head;
}
    
bt_node* mul_op(){
    bt_node* head = NULL;
    if ( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"*")) == 0)) {
            head = new_node("*");
            iterate();
        } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"/")) == 0))  {
            head = new_node("/");
            iterate();
            
        }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,"%")) == 0)) {
            head = new_node("%");
            iterate();
        }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == KEYWORD)) && ((strcmp(node_for_parsing->value,"and")) == 0)) {
            head = new_node("and");
            iterate();
        }
        return head;
}
    
bt_node* rel_op(){
        bt_node* head = NULL;
        if ( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"=")) == 0)) {
            head = new_node("=");
            iterate();
        } else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION )) && ((strcmp(node_for_parsing->value,"!=")) == 0))  {
            head = new_node("!=");
            iterate();
            
        }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,"<")) == 0)) {
            head = new_node("<");
            iterate();
        }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,">")) == 0)) {
            head = new_node(">");
            iterate();
        }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,"<=")) == 0)) {
            head = new_node("<=");
            iterate();
        }else if( (node_for_parsing != NULL) && ((node_for_parsing->type == PUNCTUATION)) && ((strcmp(node_for_parsing->value,">=")) == 0)) {
            head = new_node(">=");
            iterate();
        }


        return head;
}


/* *****************************************************************************
 PARSER IMPLEMENTATION END
 ******************************************************************************* */

int main() {
    // initialize the global array
    line_Buffer = calloc(sizeof(char),1000);
    int flag = 1;
    // this loop stores input from stdin into the global array. *Note input can be maximum of 1000 chars.
    while ((fgets(line_Buffer, 1000, stdin)) != NULL ) {
        
        // if a new line is encountered, exit the program
        if (line_Buffer[0] == '\n') break;
        
        printf("ORIGINAL INPUT: %s",line_Buffer);
        
        // scan the input for valid tokens
        
        num_insertions = 0;
        if ( scanner() != ERROR) { // if scanner is successful list tokens making up the input
            
            
        } else {
            printf("INVALID LINE INPUT: %s\n",line_Buffer);
           
            flag = 0; break;
        
        }
        
        reset_Line_Buffer();
        line_Buffer_index=0;
       
        
    }
    if (flag) {
    printList();
    bt_node* tree = parser();
    
        if (tree == NULL){ printf("PARSE FAILED on the following TOKEN: %s\n",node_for_parsing->value); }
    else{ printf("SUCCESSFUL PARSE\n");  print2D(tree); printf("\n");}
    }

}

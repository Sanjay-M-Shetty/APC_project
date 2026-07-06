
/*  NAME:SANJAY M SHETTY
    ADMISSION NUMBER :25048_56
    TITLE:ARBITRARY PRECISION CALCULATOR(APC)
    DATE:26-06-2026
    DESCRIPTION:APC is a C program that performs arithmetic operations on large integers represented as linked lists. 
    It supports addition, subtraction, multiplication, and division of numbers that exceed standard data type limits.
    The program reads two large integers and an operator from command-line arguments, processes the operation using linked list manipulation, and outputs the result.
*/




#ifndef APC_H /* Guard macro check: prevents duplicate header compilation errors */
#define APC_H /* Defines the guard macro to mark this file as included */

#include <stdio.h>
#include <stdlib.h> /* Standard library: needed for dynamic heap memory allocation via malloc and free */
#include <string.h> /* String library: needed for computing character array sizes via strlen */

/* Global Status Constants */
#define SUCCESS  0 /* Status flag: returned across the engine when execution completes error-free */
#define FAILURE -1 /* Status flag: returned across the engine when an invalid state or error occurs */

/* Magnitude Dominance Evaluation Constants */
#define OPERAND1 1 /* Comparison flag: indicates the first absolute operand is numerically larger */
#define OPERAND2 2 /* Comparison flag: indicates the second absolute operand is numerically larger */
#define SAME     0 /* Comparison flag: indicates both absolute operands are numerically equal */

/*
 * struct _node
 * Self-referential node layout for bidirectional digit-stream traversal.
 * Stores a single decimal digit value clamped strictly from 0 to 9.
 */
typedef struct _node /* Defines a custom structured data type representing an element in a list */
{
    int data;           /* Memory block: stores a single integer numerical digit strictly from 0 to 9 */
    struct _node *prev; /* Pointer reference: stores the memory address of the preceding higher-order node */
    struct _node *next; /* Pointer reference: stores the memory address of the subsequent lower-order node */
} node; /* Type definition alias: allows referencing 'struct _node' concisely as just 'node' */

/* Core List Manipulations & Boundary Management */
int cla_validation(int argc, char *argv[]); /* Evaluates if the exact required count of arguments was passed */
int create_list(char *opr, node **head, node **tail); /* Parses input string arguments into structural numerical list nodes */
int insert_first(node **head, node **tail, int data); /* Prepends a newly allocated node directly at the head boundary */
int insert_last(node **head, node **tail, int data); /* Appends a newly allocated node directly at the tail boundary */
int delete_list(node **head, node **tail); /* Deallocates every single node in a list to prevent system heap leaks */
void print_list(node *head); /* Iterates through the list nodes forward to print the value to standard output */
int list_len(node *head); /* Linearly counts and returns the absolute horizontal length of a linked list */
int compare_list(node *head1, node *head2); /* Compares the absolute magnitude weights of two numeric node layouts */
void remove_pre_zeros(node **head); /* Deallocates and strips away useless leading zero nodes from a list */

/* Unsigned Arithmetic Processing Engines */
void addition(node *tail1, node *tail2, node **headR, node **tailR); /* Adds absolute values column-by-column moving right-to-left */
void subtraction(node *tail1, node *tail2, node **headR, node **tailR); /* Subtracts absolute values column-by-column with column borrowing */
void multiplication(node *tail1, node *tail2, node **headR, node **tailR); /* Multiplies absolute digits sequentially and shifts intermediate rows */
int division(node *head1, node *head2, node **headR, node **tailR); /* Divides values via continuous subtraction to compute quotient results */

/* Algebraic Sign Interception Routing Wrappers */
void handle_sign_addition(node *head1, node *tail1, int sign1, node *head2, node *tail2, int sign2, node **headR, node **tailR, int *res_sign); /* Routes signed addition variants to base addition or subtraction core */
void handle_sign_subtraction(node *head1, node *tail1, int sign1, node *head2, node *tail2, int sign2, node **headR, node **tailR, int *res_sign); /* Routes signed subtraction variants to base addition or subtraction core */
void handle_sign_multiplication(node *head1, node *tail1, int sign1, node *head2, node *tail2, int sign2, node **headR, node **tailR, int *res_sign); /* Runs the absolute core multiplication and assigns appropriate polar signs */
int handle_sign_division(node *head1, node *tail1, int sign1, node *head2, node *tail2, int sign2, node **headR, node **tailR, int *res_sign); /* Runs absolute core division, processes exit statuses, and assigns polar signs */

#endif /* End of header guard condition block: closes the APC_H inclusion check boundary */
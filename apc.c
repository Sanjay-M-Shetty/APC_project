#include "apc.h" 

/*
 * Validates execution framework parameter count matching exact requirements.
 */
int cla_validation(int argc, char *argv[])
{
    if (argc != 4) return FAILURE; // Enforce requirement of exactly 3 command-line arguments
    return SUCCESS; // Return success status if argument count matches criteria
}

/*
 * Decodes an input string and generates structural digits inside the list layout.
 */
int create_list(char *opr, node **head, node **tail)
{
    int len = strlen(opr); // Calculate the length of the raw operand string
    if (len == 0) return FAILURE; // Fail immediately if the input string is completely empty

    for (int i = 0; i < len; i++) // Iterate linearly through each character of the string
    {
        if (opr[i] >= '0' && opr[i] <= '9') // Confirm character falls within valid numeric bounds
        {
            if (insert_last(head, tail, opr[i] - '0') == FAILURE) return FAILURE; // Convert ASCII to int and append to list
        }
        else
        {
            return FAILURE; // Abort structural construction upon data contamination
        }
    }
    return SUCCESS; // Return success after processing the entire string validly
}

/*
 *Inserts a tracking item at the front address (Most Significant Digit placement).
 */
int insert_first(node **head, node **tail, int data)
{
    node *new_node = malloc(sizeof(node)); // Allocate memory on heap for the new digit node
    if (!new_node) return FAILURE; // Check if the system memory allocation failed
    new_node->data = data; // Assign the integer digit value to node data field
    new_node->prev = NULL; // Initialize previous pointer to NULL as front node
    new_node->next = *head; // Point the next pointer to the current head node

    if (*head == NULL) // Check if the list is completely empty
    {
        *tail = new_node; // Set tail pointer to the newly created single node
    }
    else
    {
        (*head)->prev = new_node; // Connect old head backward to the new node
    }
    *head = new_node; // Update the external head reference to the new node
    return SUCCESS; // Return success after modifying boundary entry pointers
}

/*
 * Inserts a tracking item at the back address (Least Significant Digit placement).
 */
int insert_last(node **head, node **tail, int data)
{
    node *new_node = malloc(sizeof(node)); // Allocate memory on heap for the new node item
    if (!new_node) return FAILURE; // Safeguard against system-level heap allocation failures
    new_node->data = data; // Assign the integer digit value to node data field
    new_node->next = NULL; // Initialize next pointer to NULL as the last node
    new_node->prev = *tail; // Point previous pointer to the current tail node

    if (*tail == NULL) // Check if the list is completely empty
    {
        *head = new_node; // Set head pointer to the newly created single node
    }
    else
    {
        (*tail)->next = new_node; // Connect old tail forward to the new node
    }
    *tail = new_node; // Update the external tail reference to the new node
    return SUCCESS; // Return success status code back to invocation frame
}

/*
 * Iteratively clears memory spaces occupied by list objects to bypass runtime leaks.
 */
int delete_list(node **head, node **tail)
{
    node *curr = *head; // Initialize tracking pointer to the start of list
    while (curr != NULL) // Loop sequentially until reaching the end of the list
    {
        node *next = curr->next; // Cache the address of the next node before freeing
        free(curr); // Safely deallocate current node structure from heap space
        curr = next; // Advance tracking pointer to the cached next address node
    }
    *head = NULL; // Reset external head pointer reference to ground state
    *tail = NULL; // Reset external tail pointer reference to ground state
    return SUCCESS; // Return success status confirming complete resource cleanup
}

/*
 * Outputs structured numbers down to stdout streams sequentially.
 */
void print_list(node *head)
{
    if (head == NULL) // Check if list is completely devoid of digit objects
    {
        printf("0\n"); // Print explicit zero character to indicate zero value
        return; // Terminate execution function context immediately
    }
    while (head != NULL) // Walk through list nodes sequentially from left to right
    {
        printf("%d", head->data); // Print integer digit data contents directly to console
        head = head->next; // Advance reference to the lower-order structural digit
    }
    printf("\n"); // Append newline character formatting to conclude stream printing
}

/*
 * Tracks and calculates the literal horizontal positional link size of a scalar array.
 */
int list_len(node *head)
{
    int len = 0; // Initialize counter variable to zero tracking magnitude depth
    while (head) // Step through each node link within the active stream
    {
        len++; // Increment counter variable to log present node item
        head = head->next; // Progress pointer forward onto downstream node memory blocks
    }
    return len; // Return final accumulated numerical count of nodes checked
}

/*
 * Evaluates magnitude dominance properties across absolute arrays.
 */
int compare_list(node *head1, node *head2)
{
    remove_pre_zeros(&head1); // Strip extraneous zeros to prevent comparison skewing
    remove_pre_zeros(&head2); // Strip extraneous zeros from second list parameter
    
    int len1 = list_len(head1); // Compute node count length of first numerical argument
    int len2 = list_len(head2); // Compute node count length of second numerical argument

    if (len1 > len2) return OPERAND1; // Higher length indicates greater magnitude absolute value directly
    if (len2 > len1) return OPERAND2; // Second list is larger based on structural length evaluation

    while (head1 != NULL && head2 != NULL) // Scan digits left-to-right when lists share equal lengths
    {
        if (head1->data > head2->data) return OPERAND1; // First higher-order variant discovered scales dominant value
        if (head2->data > head1->data) return OPERAND2; // Second list dominant digit resolves absolute weight mismatch
        head1 = head1->next; // Step forward onto the lower-order position digit node
        head2 = head2->next; // Advance secondary list reference across identical digit indexes
    }
    return SAME; // Return identifier confirming identical numeric values after complete checks
}

/*
 * Trims away structurally unnecessary historical zeroes from mutating calculations.
 */
void remove_pre_zeros(node **head)
{
    while (*head != NULL && (*head)->data == 0 && (*head)->next != NULL) // Verify head contains non-essential zero digit
    {
        node *temp = *head; // Keep tracking location reference of targeted dummy element
        *head = (*head)->next; // Move the primary head reference forward to next item
        (*head)->prev = NULL; // Sever backwards link reference chain to isolate node entry
        free(temp); // Free allocated storage block of old zero node item
    }
}
#include "apc.h" 
/*
 * Unsigned long division layout engine.
 * Employs continuous positional extraction merged with sequential, repeated subtraction logic.
 */
int division(node *head1, node *head2, node **headR, node **tailR) 
{ 
    // Prevent division operations from breaking due to zero divisor faults
    if (head2 == NULL || (head2->next == NULL && head2->data == 0)) // Check if divisor list is empty or represents a single digit value of zero
    { 
        printf("Error: Division by zero\n"); // Print explicit division by zero error message to standard output
        return FAILURE; // Abort function execution immediately and pass back a failure status flag
    } 

    // Direct mapping configuration check if the primary dividend equals zero
    if (head1 == NULL || (head1->next == NULL && head1->data == 0)) // Check if dividend list is empty or represents a single digit value of zero
    {
        insert_last(headR, tailR, 0); // Directly append a single node containing zero into the result list layout
        return SUCCESS; // Return a success code back to caller as calculation is completed early
    } 

    node *tail2 = head2; // Initialize a temporary node pointer to track down the tail of the divisor list
    while (tail2->next != NULL) tail2 = tail2->next; // Traverse through the divisor list horizontally until the final node is reached

    node *working_head = NULL, *working_tail = NULL; // Instantiate localized head and tail pointers to manage the current dividend chunk
    node *curr1 = head1; // Instantiate a traversal pointer initialized to the start of the primary dividend list

    while (curr1 != NULL) // Loop sequentially through every positional digit of the primary dividend from left to right
    { 
        insert_last(&working_head, &working_tail, curr1->data); // Pull current digit from the dividend and append it to the working list buffer
        remove_pre_zeros(&working_head); // Clean up the working list buffer by stripping out any unneeded leading zeros

        int count = 0; // Initialize a local loop counter to track successful subtractions (the quotient digit)
        // Repeatedly subtract the divisor value away from the current extracted portion
        while (compare_list(working_head, head2) == OPERAND1 || compare_list(working_head, head2) == SAME) // Loop as long as working chunk magnitude is greater than or equal to divisor
        { 
            node *sub_head = NULL, *sub_tail = NULL; // Declare temporary perimeter pointers to capture intermediate subtraction results
            subtraction(working_tail, tail2, &sub_head, &sub_tail); // Subtract divisor from working chunk and write difference to the temp list
            
            delete_list(&working_head, &working_tail); // Deallocate heap blocks of old working list to prevent active runtime memory leaks
            working_head = sub_head; // Update working head pointer to track the new difference node sequence
            working_tail = sub_tail; // Update working tail pointer to track the new difference node sequence
            count++; // Increment quotient accumulator digit for every valid subtraction completed
        } 

        insert_last(headR, tailR, count); // Append the computed subtraction count as the next digit in the result list
        curr1 = curr1->next; // Move the traversal pointer forward to extract the next sequential dividend digit
    } 

    delete_list(&working_head, &working_tail); // Free dynamic allocations held by the temporary working buffer post-calculation
    remove_pre_zeros(headR); // Clean up final output list by dropping any structural leading zeros generated
    return SUCCESS; // Return success status flag indicating problem completed without computational faults
} 

/*
 * Formats output sign allocations cleanly following complete division operations.
 */
int handle_sign_division(node *head1, node *tail1, int sign1, node *head2, node *tail2, int sign2, node **headR, node **tailR, int *res_sign) // Define routing wrapper to intercept algebraic signs for division
{ 
    if (division(head1, head2, headR, tailR) == FAILURE) // Execute core unsigned division function and check if an internal fault occurred
    { 
        return FAILURE; // Forward the execution failure status back up to the main control layer
    } 

    if (*headR == NULL || ((*headR)->next == NULL && (*headR)->data == 0)) // Check if final quotient is physically empty or structural absolute zero
    { 
        *res_sign = 1; // Enforce a positive sign configuration because numeric zero cannot carry negative polarity
    } 
    else // Handle cases where the computed quotient is a non-zero value
    { 
        *res_sign = (sign1 != sign2) ? -1 : 1; // Assign negative polarity if input signs mismatch, else assign positive polarity
    }
    return SUCCESS; // Return a success flag showing that operation and sign configuration completed cleanly
} 
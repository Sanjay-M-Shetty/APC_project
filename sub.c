#include "apc.h" 
/*
 * Core unsigned column subtraction processing loop.
 */
void subtraction(node *tail1, node *tail2, node **headR, node **tailR)
{
    int borrow = 0; // Initialize column borrow state to zero

    while (tail1 != NULL) // Loop sequentially from right to left while operand 1 has digits
    {
        int d1 = tail1->data; // Extract current digit value from operand 1 list node
        int d2 = (tail2 != NULL) ? tail2->data : 0; // Fallback to 0 if operand 2 digit stream runs out

        int sub = d1 - d2 - borrow; // Compute the net difference of current column including borrow

        if (sub < 0) // Check if result requires a borrow from the next higher position
        {
            sub += 10; // Adjust negative value by adding base 10 to current column digit
            borrow = 1; // Raise borrow flag to subtract from next higher positional column
        }
        else // Execute when column calculation yields a non-negative value
        {
            borrow = 0; // Clear borrow flag since current column evaluation is self-sufficient
        }

        insert_first(headR, tailR, sub); // Prepend the calculated column digit to the result list front

        tail1 = tail1->prev; // Advance operand 1 node reference to next higher positional digit
        if (tail2 != NULL) tail2 = tail2->prev; // Advance operand 2 node reference forward if node exists
    }

    remove_pre_zeros(headR); // Prune any superfluous leading zero nodes from result list front
}

/*
 * Decodes algebraic sign rules and patterns to map subtraction states cleanly.
 */
void handle_sign_subtraction(node *head1, node *tail1, int sign1, node *head2, node *tail2, int sign2, node **headR, node **tailR, int *res_sign)
{
    int cmp = compare_list(head1, head2); // Evaluate magnitude dominance relationship between both inputs

    if (sign1 == 1 && sign2 == 1) // Handle operation mapping for positive minus positive state: (+A) - (+B)
    {
        if (cmp == SAME) // Match when magnitude of operand 1 equals operand 2 precisely
        {
            *res_sign = 1; // Map absolute calculation polarity to standard positive layout
            insert_first(headR, tailR, 0); // Populate result list explicitly with a single zero node
        }
        else if (cmp == OPERAND1) // Match when magnitude of operand 1 is strictly greater than operand 2
        {
            *res_sign = 1; // Identity of greater positive operand keeps final result positive
            subtraction(tail1, tail2, headR, tailR); // Subtract absolute layout 2 from absolute layout 1 directly
        }
        else // Match when magnitude of operand 2 is strictly greater than operand 1
        {
            *res_sign = -1; // Subtracting a larger number yields an overall negative value polarity
            subtraction(tail2, tail1, headR, tailR); // Subtract absolute layout 1 from dominant layout 2 value
        }
    }
    else if (sign1 == -1 && sign2 == 1) // Handle operation mapping for negative minus positive state: (-A) - (+B)
    {
        *res_sign = -1; // Coerce sign output to negative since both components aggregate downwards
        addition(tail1, tail2, headR, tailR); // Execute absolute values addition to find the sum magnitude
    }
    else if (sign1 == 1 && sign2 == -1) // Handle operation mapping for positive minus negative state: (+A) - (-B)
    {
        *res_sign = 1; // Double negation converts statement to purely positive value addition sequence
        addition(tail1, tail2, headR, tailR); // Execute absolute values addition to find the sum magnitude
    }
    else if (sign1 == -1 && sign2 == -1) // Handle operation mapping for negative minus negative state: (-A) - (-B)
    {
        if (cmp == SAME) // Match when magnitude of operand 1 equals operand 2 precisely
        {
            *res_sign = 1; // Map canceling identical components to absolute zero status value
            insert_first(headR, tailR, 0); // Populate result list explicitly with a single zero node
        }
        else if (cmp == OPERAND1) // Match when absolute magnitude of negative operand 1 dominates value
        {
            *res_sign = -1; // Result retains negative polarity since operand 1 magnitude is larger
            subtraction(tail1, tail2, headR, tailR); // Subtract absolute layout 2 away from absolute layout 1
        }
        else // Match when absolute magnitude of negative operand 2 dominates value
        {
            *res_sign = 1; // Result shifts positive since negated negative operand 2 is larger
            subtraction(tail2, tail1, headR, tailR); // Subtract absolute layout 1 away from absolute layout 2
        }
    }
}
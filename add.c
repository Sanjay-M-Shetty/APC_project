#include "apc.h" 
/*
 * Unsigned addition processing core.
 * Employs standard right-to-left list processing, combining columns and carrying fractions.
 */
void addition(node *tail1, node *tail2, node **headR, node **tailR)
{
    int carry = 0, sum = 0; // Initialize variables to track column carrying value and the localized sum result

    while (tail1 != NULL || tail2 != NULL || carry != 0) // Loop continues until all digits are processed and carry is completely cleared
    {
        sum = carry; // Start the column sum computation with the leftover carry value from the previous column
        if (tail1 != NULL) // Check if the first operand list still contains remaining unread higher-order digits
        {
            sum += tail1->data; // Add the numeric value of the current digit from operand 1 into the running column sum
            tail1 = tail1->prev; // Step backwards toward the next higher-order positional digit node in the first list
        }
        if (tail2 != NULL) // Check if the second operand list still contains remaining unread higher-order digits
        {
            sum += tail2->data; // Add the numeric value of the current digit from operand 2 into the running column sum
            tail2 = tail2->prev; // Step backwards toward the next higher-order positional digit node in the second list
        }

        carry = sum / 10; // Extract the next column's carry value using integer division by base 10 (e.g., 15 / 10 = 1)
        sum = sum % 10; // Isolate the single-digit remainder using the modulo operator to save at the current position

        insert_first(headR, tailR, sum); // Prepend the computed digit to the front of the result list as a higher magnitude place
    }
}

/*
 * Handles algebraic sign transformations for incoming addition pipelines.
 */
void handle_sign_addition(node *head1, node *tail1, int sign1, node *head2, node *tail2, int sign2, node **headR, node **tailR, int *res_sign)
{
    int cmp = compare_list(head1, head2); // Evaluate and store the absolute magnitude dominance comparison between both lists

    if (sign1 == 1 && sign2 == 1) // Execution branch triggered when adding two strictly positive arbitrary values (+A + +B)
    {
        *res_sign = 1; // Assign a positive identity flag to the final calculated result sign variable pointer
        addition(tail1, tail2, headR, tailR); // Invoke the core unsigned addition loop using the rightmost tail boundary nodes
    }
    else if (sign1 == -1 && sign2 == -1) // Execution branch triggered when adding two strictly negative arbitrary values (-A + -B)
    {
        *res_sign = -1; // Assign a negative identity flag to the final calculated result sign variable pointer
        addition(tail1, tail2, headR, tailR); // Invoke the core unsigned addition loop since the operations sum absolute values
    }
    else // Execution branch triggered when mixing positive and negative signs, converting addition into a subtraction problem
    {
        if (cmp == SAME) // Branch condition met if the absolute magnitudes of both operand data values are completely identical
        {
            *res_sign = 1; // Set result polarity flag to positive because the matching magnitudes cancel out to net neutral zero
            insert_first(headR, tailR, 0); // Directly instantiate the required literal numeric value zero into the empty result list
        }
        else if (cmp == OPERAND1) // Branch condition met if the absolute value magnitude of Operand 1 dominates Operand 2
        {
            *res_sign = (sign1 == -1) ? -1 : 1; // Inherit the sign of the dominant first operand to dictate the final output polarity
            subtraction(tail1, tail2, headR, tailR); // Subtract absolute Operand 2 away from the dominant Operand 1 list structure
        }
        else // Branch condition met if the absolute value magnitude of Operand 2 dominates Operand 1
        {
            *res_sign = (sign2 == -1) ? -1 : 1; // Inherit the sign of the dominant second operand to dictate the final output polarity
            subtraction(tail2, tail1, headR, tailR); // Subtract absolute Operand 1 away from the dominant Operand 2 list structure
        }
    }
}
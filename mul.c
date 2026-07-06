#include "apc.h" 

/*
 * Unsigned arbitrary precision multiplication using standard long-multiplication mechanics.
 * Walks multiplier digits from right-to-left, computing scalar products and merging rows.
 */
void multiplication(node *tail1, node *tail2, node **headR, node **tailR)
{
    // Establish a baseline total accumulated value representation at absolute 0
    insert_last(headR, tailR, 0); // Initialize the running product total list to zero

    node *curr2 = tail2; // Create a traveler pointer starting at the multiplier's least significant digit
    int shift_count = 0; // Initialize a position tracker to count required column shifts (zeros to append)

    while (curr2 != NULL) // Loop through each digit of the multiplier from right to left
    {
        int d2 = curr2->data; // Extract the current scalar digit from the second operand
        
        // Skip multiplication routines if the current multiplier digit is 0
        if (d2 != 0) // Optimization: bypass long multiplication steps entirely if multiplying by zero
        {
            node *curr1 = tail1; // Create a traveler pointer starting at the multiplicand's least significant digit
            node *head_temp = NULL, *tail_temp = NULL; // Initialize a temporary list to hold the intermediate row product
            int carry = 0; // Reset row multiplication carry status back to zero

            // Compute linear products across current multi-digit columns
            while (curr1 != NULL || carry != 0) // Continue processing until all digits are multiplied and carry is cleared
            {
                int d1 = (curr1 != NULL) ? curr1->data : 0; // Extract multiplicand digit if valid, otherwise treat as zero
                int prod = (d1 * d2) + carry; // Compute total scalar product for the column including running carry

                carry = prod / 10; // Calculate the new positional carry value for the next column
                int digit = prod % 10; // Extract the single units digit to store in the node

                insert_first(&head_temp, &tail_temp, digit); // Prepends the current digit to the row product list

                if (curr1 != NULL) curr1 = curr1->prev; // Advance backward to the next higher-order multiplicand digit
            }

            // Append positional structural zeroes to shift values left
            for (int i = 0; i < shift_count; i++) // Run loop based on current row power-of-ten displacement
            {
                insert_last(&head_temp, &tail_temp, 0); // Append a trailing positional placeholder zero node to the row
            }

            node *head_ans = NULL, *tail_ans = NULL; // Initialize temporary boundary pointers for the combined sum result
            addition(*tailR, tail_temp, &head_ans, &tail_ans); // Add current row product to the existing total accumulated product

            // Reassign context maps to point safely to the newly calculated total sum values
            delete_list(headR, tailR); // Clear memory allocations holding old accumulated intermediate totals
            *headR = head_ans; // Point the caller's result head pointer to the fresh summation
            *tailR = tail_ans; // Point the caller's result tail pointer to the fresh summation

            delete_list(&head_temp, &tail_temp); // Release heap memory occupied by the temporary current row list
        }

        shift_count++; // Increment positional offset tracker to adjust left-shifting for the next iteration
        curr2 = curr2->prev; // Advance backward to the next higher-order multiplier digit
    }
    
    remove_pre_zeros(headR); // Clean up any leftover leading zeros from intermediate additions
}

/*
 * Resolves final polarization statuses for multiplication operations.
 */
void handle_sign_multiplication(node *head1, node *tail1, int sign1, node *head2, node *tail2, int sign2, node **headR, node **tailR, int *res_sign)
{
    multiplication(tail1, tail2, headR, tailR); // Delegate execution to core absolute magnitude multiplication engine
    if (headR == NULL || ((*headR)->next == NULL && (*headR)->data == 0)) // Evaluate if final computed answer list equals mathematical zero
    {
        *res_sign = 1; // Enforce a strictly positive sign context since negative zero is invalid
    }
    else // Handle non-zero signed operations
    {
        *res_sign = (sign1 != sign2) ? -1 : 1; // Assign negative polarity if signs mismatch, otherwise keep positive
    }
}
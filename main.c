#include "apc.h" 

/*
 * Main execution lifecycle framework for the APC engine.
 * Handles formal parsing bounds, sign routing configuration, and automatic heap recovery.
 */
int main(int argc, char *argv[]) 
{
    // 1. Validate parameter count from standard execution input
    if (cla_validation(argc, argv) == FAILURE) // Invokes helper function to ensure exactly 3 arguments are passed to the calculator.
    {
        printf("Usage: ./apc <operand1> <operator> <operand2>\n"); // Prints correct execution usage pattern to stdout upon verification failure.
        return FAILURE; // Aborts execution and returns termination status code (-1) to the operating system.
    }

    // 2. Terminate execution if the operator string violates structural bounds
    if (strlen(argv[2]) != 1) // Verifies that the operator argument consists of exactly one character to prevent malformed strings.
    {
        printf("Error: Invalid operator '%s'\n", argv[2]); // Prints error message detailing the non-conforming operator string value.
        return FAILURE; // Aborts execution and returns termination status code (-1) due to invalid operator length.
    }

    // Initialize list perimeter address spaces to point safely to ground
    node *head1 = NULL, *tail1 = NULL; // Allocates and sets pointer boundaries for the first operand's doubly linked list to NULL.
    node *head2 = NULL, *tail2 = NULL; // Allocates and sets pointer boundaries for the second operand's doubly linked list to NULL.
    node *headR = NULL, *tailR = NULL; // Allocates and sets pointer boundaries for the final calculated result's linked list to NULL.

    // 3. Extract and cache operand algebraic sign configuration pre-parsing
    int sign1 = (argv[1][0] == '-') ? -1 : 1; // Evaluates leading character of first operand to assign numeric sign (-1 for negative, 1 for positive).
    int sign2 = (argv[3][0] == '-') ? -1 : 1; // Evaluates leading character of second operand to assign numeric sign (-1 for negative, 1 for positive).

    // Advance string extraction index pointers past sign notation if explicitly present
    char *op1_str = (argv[1][0] == '-' || argv[1][0] == '+') ? argv[1] + 1 : argv[1]; // Skips leading unary sign character if present on the first operand string.
    char *op2_str = (argv[3][0] == '-' || argv[3][0] == '+') ? argv[3] + 1 : argv[3]; // Skips leading unary sign character if present on the second operand string.

    // 4. Populate lists and validate positional numeric correctness
    if (create_list(op1_str, &head1, &tail1) == FAILURE) // Converts first operand string into digit nodes; checks for invalid non-numeric inputs.
    {
        printf("Error: Invalid character in operand 1\n"); // Prints data integrity error message targeting the first operand string.
        return FAILURE; // Aborts execution immediately without leaking because no other heap structures were allocated yet.
    }

    if (create_list(op2_str, &head2, &tail2) == FAILURE) // Converts second operand string into digit nodes; checks for invalid non-numeric inputs.
    {
        printf("Error: Invalid character in operand 2\n"); // Prints data integrity error message targeting the second operand string.
        delete_list(&head1, &tail1); // Frees the previously allocated memory blocks of the first operand list to avoid memory leaks.
        return FAILURE; // Aborts execution and returns termination status code (-1) due to structural list generation failure.
    }

    // Strip out structurally redundant zeroes before routing
    remove_pre_zeros(&head1); // Trims non-value-adding leading zeros from the first operand's linked list to optimize performance.
    remove_pre_zeros(&head2); // Trims non-value-adding leading zeros from the second operand's linked list to optimize performance.

    char oper = argv[2][0]; // Captures the validated single character operator flag to govern the calculation execution path.
    int res_sign = 1; // Initializes the final calculated result algebraic sign state to default positive (1).

    // 5. Operation switch routing matrix
    switch (oper) // Evaluates operator character to select corresponding sign-aware mathematics wrapper routine.
    {
        case '+': // Intercepts addition operators.
            handle_sign_addition(head1, tail1, sign1, head2, tail2, sign2, &headR, &tailR, &res_sign); // Dispatches lists and signs to the addition orchestration logic.
            break; // Exits switch block cleanly upon execution completion of the addition routine.

        case '-': // Intercepts subtraction operators.
            handle_sign_subtraction(head1, tail1, sign1, head2, tail2, sign2, &headR, &tailR, &res_sign); // Dispatches lists and signs to the subtraction orchestration logic.
            break; // Exits switch block cleanly upon execution completion of the subtraction routine.

        case '*': // Intercepts multiplication operators.
            handle_sign_multiplication(head1, tail1, sign1, head2, tail2, sign2, &headR, &tailR, &res_sign); // Dispatches lists and signs to the multiplication orchestration logic.
            break; // Exits switch block cleanly upon execution completion of the multiplication routine.

        case '/': // Intercepts division operators.
            if (handle_sign_division(head1, tail1, sign1, head2, tail2, sign2, &headR, &tailR, &res_sign) == FAILURE) // Dispatches lists to division workflow; checks for critical errors like division by zero.
            {
                delete_list(&head1, &tail1); // Frees first operand list heap allocations to prevent leaks during division abort.
                delete_list(&head2, &tail2); // Frees second operand list heap allocations to prevent leaks during division abort.
                return FAILURE; // Aborts execution and returns failure status code (-1) following math safety violation.
            }
            break; // Exits switch block cleanly upon successful completion of the division routine.

        default: // Intercepts unsupported characters falling outside defined math operations.
            printf("Error: Invalid operator '%c'\n", oper); // Prints error notice exposing the faulty unsupported operator token.
            delete_list(&head1, &tail1); // Frees first operand list allocations safely before exit sequence.
            delete_list(&head2, &tail2); // Frees second operand list allocations safely before exit sequence.
            return FAILURE; // Aborts processing and returns failure code (-1) due to unroutable command token.
    }

    // 6. Conditional evaluation formatting for negative results
    if (res_sign == -1 && headR != NULL && !(headR->next == NULL && headR->data == 0)) // Ensures a negative sign prints only if total value is non-zero.
    {
        printf("-"); // Outputs negative polarization indicator sign directly ahead of number digits.
    }
    
    print_list(headR); // Iterates over the result linked list nodes from head to tail to display final digit string.

    // 7. System-wide context destruction to release memory resources safely
    delete_list(&head1, &tail1); // Reclaims memory held by the nodes of the first operand list.
    delete_list(&head2, &tail2); // Reclaims memory held by the nodes of the second operand list.
    delete_list(&headR, &tailR); // Reclaims memory held by the nodes of the calculated result list.

    return SUCCESS; // Returns successful process completion code (0) back to host environment.
}
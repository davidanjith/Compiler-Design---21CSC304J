/*
 Hardcoded NFA to DFA Conversion in C 
This is a simple implementaion in C to help understand how the code works..
Feel free to change the state transitions and see the resulant NFA table change!
*/

#include <stdio.h>

int main() {
    /* Hardcoded NFA Transition Table */
    int nfa[4][2] = {{0}}; // Initialize all transitions to 0
    //syntax: nfa[current_state][input_symbol] = state_after_transition (the input symbols used here are 0 and 1, representing a and b)
    nfa[1][0] = 2;  // State 1 transitions on 'a' to State 2
    nfa[1][1] = 13; // State 1 transitions on 'b' to State 13 (composite state)
    nfa[2][0] = 2;  // State 2 transitions on 'a' to itself
    nfa[2][1] = 1;  // State 2 transitions on 'b' to State 1
    nfa[3][0] = 2;  // State 3 transitions on 'a' to State 2
    nfa[3][1] = 3;  // State 3 transitions on 'b' to itself

    /* DFA Table and Discovered States */
    int dfa[10][2] = {{0}}; // DFA transition table initialized to 0 (10 refers to the no.of states and 2 refors to the no.of symbols which is 2 here.)
    int dstate[10] = {0};   // Stores discovered DFA states
    int n = 1, k, j, flag, m, q, r; // State counter, loop variables
    dstate[1] = 1; // Start state of DFA

    /* DFA Construction Process */
    for (k = 1; dstate[k] != 0; k++) {
        m = dstate[k];
        q = (m > 10) ? m / 10 : 0; // Extract first part of composite state
        r = (m > 10) ? m % 10 : m; // Extract second part or keep single state
        
        for (j = 0; j < 2; j++) { // Loop through input symbols 'a' and 'b'
            dfa[k][j] = nfa[r][j]; // Get NFA transition for state 'r'
            
            flag = 0; // Check if new state already exists
            for (int i = 1; i <= n; i++) {
                if (dfa[k][j] == dstate[i]) {
                    flag = 1;
                    break;
                }
            }
            //flag variable is used to check whether a new DFA state has already been added to the dstate array
            if (!flag && dfa[k][j] != 0) { // If new state is found
                int nextState = dfa[k][j];
                int duplicate = 0;
                for (int l = 1; l <= n; l++) {
                    if (dstate[l] == nextState) {
                        duplicate = 1;
                        break;
                    }
                }
                if (!duplicate) {
                    dstate[++n] = nextState; // Add new DFA state
                }
            }
        }
    }
    /*This is the formatting section.. Feel free to format this table however you want!*/
    /* Print DFA Transition Table */
    printf("\nDFA Transition Table:\n");
    printf("------------------------\n");
    printf("| State |   a   |   b   |\n");
    printf("------------------------\n");
    
    for (int i = 1; i < k; i++) {
        if (dstate[i] == 13) {
            printf("|  %2d   |  %2d   |  %2d   |\n", 13, dfa[i][0], dfa[i][1]);
        } else {
            printf("|  %2d   |  %2d   |  %2d   |\n", dstate[i], dfa[i][0], dfa[i][1]);
        }
    }
    printf("------------------------\n");
    
    return 0;
}

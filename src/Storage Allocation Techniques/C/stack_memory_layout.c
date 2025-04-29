/* Have no idea what a Stack Frame, Frame pointer etc. are?
Ok so..
1) A stack frame is the chunk of the stack memory used for a single function call — like func()
   It stores important stuff like the frame pointer, variable offsets, etc.

2) The Frame Pointer points to the beginning of the stack memory location and it does not change
   as a new variable is defined, an offset to the frame pointer is stored tp eficiently keep track of the loc of the current variable!

3) And the Stack Pointer points to the top of the stack at any time.

tldr; we have a FP cuz the SP keeps changing due to system calls like push, call, alloca
      but the FP remains constant and fixed throught the functions lifetime.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_VARS 100

typedef struct {
    char type[10];
    char name[20];
    int size;
    int offset;
} Variable;

int getSize(char* type) {
    if (strcmp(type, "int") == 0) return 4;
    if (strcmp(type, "float") == 0) return 4;
    if (strcmp(type, "char") == 0) return 1;
    if (strcmp(type, "double") == 0) return 8;
    return 0; //if the datatype is unknown
}

void printStackFrame(Variable vars[], int count) {
    printf("\n[Stack Frame So Far]\n");
    printf("FP (Frame Pointer): offset 0\n");
    for (int i = 0; i < count; i++) {
        printf("%s (%s): Offset %d | Size: %d bytes\n", 
               vars[i].name, vars[i].type, vars[i].offset, vars[i].size);
    }
    printf("\n");
}

int main() {
    int lines;
    char input[256];
    Variable vars[MAX_VARS];
    int varCount = 0;
    int offset = 0;

    printf("How many lines of input? ");
    scanf("%d", &lines);
    getchar(); // clear newline after scanf

    for (int l = 0; l < lines; l++) {
        printf("Line %d: ", l + 1);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // remove newline

        // split type and rest
        char* declaration = strtok(input, ";");
        while (declaration != NULL) {
            char type[10], rest[200];
            sscanf(declaration, "%s %[^\n]", type, rest);

            char* token = strtok(rest, ",");
            while (token != NULL) {
                char varname[20];
                sscanf(token, "%*[^a-zA-Z]%s", varname);

                Variable v;
                strcpy(v.type, type);
                strcpy(v.name, varname);
                v.size = getSize(type);
                /*Fun fact?
                the stack actually "grows down" in memory on most modern architectures
                In most systems:
                Memory addresses increase upward (from 0x00000000 to higher)
                and the stack grows downward toward lower addresses
                offset -= v.size;  //stack grows down
                */
                //but uhh lets keep it simple and make the offsets grow up from the FP's value
                
                // Increment offset for each variable
                offset += v.size;
                v.offset = offset;

                vars[varCount++] = v;
                token = strtok(NULL, ",");
            }

            declaration = strtok(NULL, ";");
        }

        // Print stack frame after this line
        printStackFrame(vars, varCount);
    }

    return 0;
}
   
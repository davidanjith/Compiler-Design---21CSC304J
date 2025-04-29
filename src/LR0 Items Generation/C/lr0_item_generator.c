#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_RULES 10
#define MAX_ITEMS 50

typedef struct {
    char lhs[10];
    char rhs[20];
    int dot;
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int size;
} ItemSet;

char grammar[MAX_RULES][20];
int num_rules = 0;
ItemSet itemsets[MAX_ITEMS];
int num_itemsets = 0;

void read_grammar() {
    printf("Enter number of grammar rules: ");
    scanf("%d", &num_rules);
    getchar();
    
    printf("Enter grammar rules (e.g., E->E+T):\n");
    for (int i = 0; i < num_rules; i++) {
        fgets(grammar[i], 20, stdin);
        grammar[i][strcspn(grammar[i], "\n")] = 0; //remove newlines that may be there from da input
    }
}

void add_initial_rule() {
    //add the augmented S'->S rule
    sprintf(grammar[num_rules], "S'->%c", grammar[0][0]);
    num_rules++;
}

int find_item(ItemSet *set, Item *item) {
    for (int i = 0; i < set->size; i++) {
        if (strcmp(set->items[i].lhs, item->lhs) == 0 &&
            strcmp(set->items[i].rhs, item->rhs) == 0 &&
            set->items[i].dot == item->dot) {
            return 1;
        }
    }
    return 0;
}

void closure(ItemSet *set) {
    int changed;
    do {
        changed = 0;
        int current_size = set->size;
        
        for (int i = 0; i < current_size; i++) {
            Item *item = &set->items[i];
            if (item->dot < strlen(item->rhs)) {
                char next = item->rhs[item->dot];
                if (isupper(next)) { // Non-terminal
                    for (int j = 0; j < num_rules; j++) {
                        if (grammar[j][0] == next) {
                            Item new_item;
                            strncpy(new_item.lhs, grammar[j], strchr(grammar[j], '>') - grammar[j] - 1);
                            new_item.lhs[strchr(grammar[j], '>') - grammar[j] - 1] = '\0';
                            strcpy(new_item.rhs, strchr(grammar[j], '>') + 1);
                            new_item.dot = 0;
                            
                            if (!find_item(set, &new_item)) {
                                set->items[set->size++] = new_item;
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
}

void create_initial_itemset() {
    ItemSet *I0 = &itemsets[num_itemsets++];
    I0->size = 0;
    
    //add all rules with dot at start
    for (int i = 0; i < num_rules; i++) {
        Item new_item;
        strncpy(new_item.lhs, grammar[i], strchr(grammar[i], '>') - grammar[i] - 1);
        new_item.lhs[strchr(grammar[i], '>') - grammar[i] - 1] = '\0';
        strcpy(new_item.rhs, strchr(grammar[i], '>') + 1);
        new_item.dot = 0;
        
        I0->items[I0->size++] = new_item;
    }
    
    closure(I0);
}

int find_itemset(ItemSet *set) {
    for (int i = 0; i < num_itemsets; i++) {
        if (itemsets[i].size == set->size) {
            int match = 1;
            for (int j = 0; j < set->size; j++) {
                if (!find_item(&itemsets[i], &set->items[j])) {
                    match = 0;
                    break;
                }
            }
            if (match) return i;
        }
    }
    return -1;
}

void goto_itemset(ItemSet *from, char symbol, ItemSet *to) {
    to->size = 0;
    
    for (int i = 0; i < from->size; i++) {
        Item *item = &from->items[i];
        if (item->dot < strlen(item->rhs) && item->rhs[item->dot] == symbol) {
            Item new_item;
            strcpy(new_item.lhs, item->lhs);
            strcpy(new_item.rhs, item->rhs);
            new_item.dot = item->dot + 1;
            
            to->items[to->size++] = new_item;
        }
    }
    
    closure(to);
}

void generate_itemsets() {
    create_initial_itemset();
    
    int changed;
    do {
        changed = 0;
        int current_itemsets = num_itemsets;
        
        for (int i = 0; i < current_itemsets; i++) {
            //find all symbols after dots
            char symbols[20] = {0};
            int num_symbols = 0;
            
            for (int j = 0; j < itemsets[i].size; j++) {
                Item *item = &itemsets[i].items[j];
                if (item->dot < strlen(item->rhs)) {
                    char sym = item->rhs[item->dot];
                    if (!strchr(symbols, sym)) {
                        symbols[num_symbols++] = sym;
                    }
                }
            }
            
            //create new itemsets for each symbol
            for (int k = 0; k < num_symbols; k++) {
                ItemSet new_set;
                goto_itemset(&itemsets[i], symbols[k], &new_set);
                
                if (new_set.size > 0) {
                    int found = find_itemset(&new_set);
                    if (found == -1) {
                        itemsets[num_itemsets++] = new_set;
                        changed = 1;
                    }
                }
            }
        }
    } while (changed);
}

void print_item(Item *item) {
    printf("%s->", item->lhs);
    for (int i = 0; i <= strlen(item->rhs); i++) {
        if (i == item->dot) printf(".");
        if (i < strlen(item->rhs)) printf("%c", item->rhs[i]);
    }
    printf("\n");
}

void print_itemsets() {
    for (int i = 0; i < num_itemsets; i++) {
        printf("I%d:\n", i);
        for (int j = 0; j < itemsets[i].size; j++) {
            print_item(&itemsets[i].items[j]);
        }
        printf("\n");
    }
}

int main() {
    read_grammar();
    add_initial_rule();
    generate_itemsets();
    print_itemsets();
    return 0;
}

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

// WARNING: while loops, for loops, do loops and goto statements are
//          strictly forbidden.
struct node *append(struct node *l1, struct node *l2) {
    if (l1 == NULL) {
        return l2;
    } else {
        l1 -> next = append(l1 -> next, l2);
        return l1;
    }  
}


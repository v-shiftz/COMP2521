
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct node *flas(struct node *l) {
    struct node *flas = NULL;
    struct node *curr = l;
    int flas_len = 0;
    while (curr != NULL) {
        struct node *start = curr;
        int len = 1;

        while(curr -> next != NULL && curr -> next -> value > curr -> value) {
            len++;
            curr = curr -> next;
        }

        if(len > flas_len && len >= 2) {
            flas = start;
            flas_len = len;
        }
        curr = curr -> next;
    }
    return flas;
}


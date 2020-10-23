#include <stdio.h>
#include <stdlib.h>


#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102
 
typedef int T;

typedef struct Node_tag {
    T value;
    struct Node_tag *next;
} Node_t;

void push_1(Node_t **head, T value) {
    Node_t *tmp = malloc(sizeof(Node_t));
    if (tmp == NULL) {
        printf("Ошибка! Переполнение стека");
        exit(STACK_OVERFLOW);
    }
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
    //free(tmp);
}

// 
Node_t* pop1(Node_t **head) {
    int *p = &((*head)->value);
    printf("память %p\n", p);
    Node_t *out;
    if ((*head) == NULL) {
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    return out;
}


T pop2(Node_t **head) {
    int *p = &((*head)->value);
    printf("память %p\n", p);
    Node_t *out;
    T value;
    if (*head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}

T peek_list(const Node_t* head) {
    if (head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    return head->value;
}

void printStack_list(const Node_t* head) {
    printf("stack >");
    while (head) {
        printf("%d ", head->value);
        head = head->next;
    }
}

size_t getSize_list(const Node_t *head) {
    size_t size = 0;
    while (head) {
        size++;
        head = head->next;
    }
    return size;
}


int main(void) {
    long i;
    Node_t *head = NULL;
    for (i = 0; i < 300; i++) {
        push_1(&head, i);
    }
    printStack_list(head);
    printf("size = %d\n", getSize_list(head));
    while (head) {
        printf("размер нашей памяти %lu\n", sizeof(head)*getSize_list(head));
        printf("%d ", peek_list(head));
        printf("%d ", pop2(&head));
    }
    //free(head);
    return EXIT_SUCCESS;
}
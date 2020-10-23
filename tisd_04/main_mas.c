#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define STACK_MAX_SIZE 80
typedef int T;

typedef struct Stack_tag {
    T data[STACK_MAX_SIZE];
    size_t size;
} Stack_t;

// push кладёт элемент на стек
int push(Stack_t *stack, const T value) {
    if (stack->size >= STACK_MAX_SIZE) {
        printf("Ошибка! Переполнение стека!\n");
        return EXIT_FAILURE;
        exit(STACK_OVERFLOW);
    }
    stack->data[stack->size] = value;
    stack->size++;
    return EXIT_SUCCESS;
}

// pop - возвращает элмент с вершины и переходит к следующему
T pop(Stack_t *stack) {
    int *p = &(stack->data[stack->size]);
    if (stack->size == 0) {
        printf("Ошибка! В стеке не осталось элементов");
        exit(STACK_UNDERFLOW);
    }
    stack->size--;
    printf("память %p\n", p);
    return stack->data[stack->size];
}

// peek - возвращет текущий элемент с вершины
T peek(const Stack_t *stack) {
    if (stack->size <= 0) {
        exit(STACK_UNDERFLOW);
    }
    return stack->data[stack->size - 1];
}

void printStackValue(const T value) {
    printf("%d", value);
}

void printStack(const Stack_t *stack, void (*printStackValue)(const T)) {
    int i;
    int len = stack->size - 1;
    printf("stack %d > ", stack->size);
    for (i = 0; i < len; i++) {
        printStackValue(stack->data[i]);
        printf(" | ");
    }
    if (stack->size != 0) {
        printStackValue(stack->data[i]);
    }
    printf("\n");
}

int main(void)
{
    Stack_t stack;
    stack.size = 0;
    push(&stack, 3);
    printStack(&stack, printStackValue);
    push(&stack, 5);
    printStack(&stack, printStackValue);
    push(&stack, 7);
    printStack(&stack, printStackValue);
    printf("%d\n", pop(&stack));
    printStack(&stack, printStackValue);
    printf("%d\n", pop(&stack));
    printStack(&stack, printStackValue);
    printf("%d\n", pop(&stack));
    printStack(&stack, printStackValue);
    push(&stack, 10);
    push(&stack, 12);
//    printf("%d\n", pop(&stack));
//    printf("%d\n", pop(&stack));
    printf("Сам стек\n");
    printStack(&stack, printStackValue);
    printf("\nРазмер до добавления: %lu\n", sizeof(stack));
    push(&stack, 6);
    printf("Размер после добавления: %lu)", sizeof(stack));    
    return EXIT_SUCCESS;
    //_getch();
}

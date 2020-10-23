#include <stdio.h>
#include <stdlib.h>
#define STACK_MAX_SIZE 20

#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102

typedef int T;

typedef struct Stack_tag {
    T data[STACK_MAX_SIZE];
    size_t size;
} Stack_t;

typedef int T;
typedef struct Node_tag {
    T value;
    struct Node_tag *next;
} Node_t;


// push кладёт элемент на стек
int push(Stack_t *stack, const T value) {
    if (stack->size >= STACK_MAX_SIZE) {
        printf("Ошибка! Переполнение стека!\n");
        return EXIT_FAILURE;
        exit(STACK_OVERFLOW);
    }
    stack->data[stack->size] = value;
    stack->size++;
    return 0;
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




int push_1(Node_t **head, T value) {
    Node_t *tmp = malloc(sizeof(Node_t));
    if (tmp == NULL) {
        printf("Ошибка! Переполнение стека");
        return EXIT_FAILURE;
        exit(STACK_OVERFLOW);
    }
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
    return 0;
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


int menu_select(void)
{
    int c;

    printf("1. МАССИВ: Ввести элементы стека\n");
    printf("2. МАССИВ: Добавить элемент в стек\n");
    printf("3. МАССИВ: Удалить элемент из стека\n");
    printf("4. МАССИВ: Отсортировать стек с использованием другого(массив)\n");
    printf("5. МАССИВ: Вывести текущий стек\n\n");
    printf("6. СПИСОК: Ввести элементы стека\n");
    printf("7. СПИСОК: Добавить элемент в стек\n");
    printf("8. СПИСОК: Удалить элемент из стека\n");
    printf("9. СПИСОК: Вывести массив освободившихся адресов\n");
    printf("10. СПИСОК: Отсортировать стек с использованием другого(список)\n");
    printf("11. СПИСОК: Вывести элменты стека\n");
    printf("\n0. Выход\n");

    do {
        printf("\nВаш выбор: ");
        if (scanf("%d", &c) != 1)
        {
            printf("\nНеверные входные данные\n");
            scanf("%*s");
            c = -1;
        }
    } while (c < 0 || c > 11);

    printf("\n");

    return c;
}


int main(void)
{
    //setlocale (LC_CTYPE, "Russian");
    //setbuf(stdout, NULL);
    int i;
    int x;
    char choice;
    int list_kol;
    int trig = 0;
    int trig_1 = 0;
    init_list();
    init_key_list();
    int mas_kol;
    for(;;)
    {
        choice = menu_select();
        switch(choice)
        {
        case 1:
            if (trig == 1)
            {
                printf("Стек уже существует! Выход из программы\n");
                return EXIT_FAILURE;
            }
            printf("Введите количество элементов стека: ");
            if (scanf("%d", &mas_kol))
            {
                printf("Вы ввели неверное значение\n");
                return EXIT_FAILURE;
            }
            printf("\n");
            if (mas_kol > STACK_MAX_SIZE)
            {
                printf("Вы ввели слишком большое количество элементов\n");
                return EXIT_FAILURE;
            }
            trig = 1;
            Stack_t stack;
            printf("Введите элементы стека, каждый с новой строки\n");
            for (i = 0; i < mas_kol; i++)
            {
                if (scanf("%d", &x) != 1)
                {
                    printf("Вы ввели неверное значение\n");
                    return EXIT_FAILURE;
                }
                push(&stack, x);
            }
            break;
        case 2:
            printf("Введите элмент, который хотите добавить: ");
            if (scanf("%d", &x) != 1)
            {
                printf("Вы ввели неверное значение\n");
                return EXIT_FAILURE;
            }
            if(push(&stack, x))
                return EXIT_FAILURE;
            printf("Элемент успешно добавлен\n");
        	//print_list(&stack, x);
            break;
        case 3: 
        	if (pop(&stack))
                return EXIT_FAILURE;
            printf("Элемент успешно удален\n");
            break;
        case 4:
            init_list();
            printf("1\n");
            fscan_list();
            printf("2\n");
            init_key_list();
            printf("3\n");
           	scan_key_list();
           	printf("4\n");
            break;
        case 5:
            printf("Размер и сами элементы стека\n");
            printStack(&stack, printStackValue);
            break;
        case 6: 
            if (trig_1 == 1)
            {
                printf("Стек уже существует! Выход из программы\n");
                return EXIT_FAILURE;
            }
            printf("Введите количество элементов стека: ");
            if (scanf("%d", &list_kol))
            {
                printf("Вы ввели неверное значение\n");
                return EXIT_FAILURE;
            }
            printf("\n");
            trig_1 = 1;
            Node_t *head = NULL;
            printf("Введите элементы стека, каждый с новой строки\n");
            for (i = 0; i < list_kol; i++)
            {
                if(scanf("%d", &x) != 1)
                {
                    printf("Вы ввели неверное значение\n");
                    return EXIT_FAILURE;
                }
                push_1(&head, x);
            }
            break;
        case 7: 
            printf("Введите элмент, который хотите добавить: ");
            if (scanf("%d", &x) != 1)
            {
                printf("Вы ввели неверное значение\n");
                return EXIT_FAILURE;
            }
            if(push_1(&head, x))
                return EXIT_FAILURE;
            printf("Элемент успешно добавлен\n");
            break;
        case 8: 
        	if (pop2(&stack))
                return EXIT_FAILURE;
            printf("Элемент успешно удален\n");
            break;
        case 9: 
        	check_ef();
            break;
        case 10: 
        	check_sort();
            break;
        case 11:
            printf("Размер и сами элементы стека\n");
            printStack_list(head);
            break;
        case 0: 
        	return 0;
        }
    }
}
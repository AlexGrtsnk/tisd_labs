//copy from macos

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STACK_MAX_SIZE 202

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

typedef struct
{
    size_t *arr;
    int capacity;
    int ind;
} arr_t;

typedef struct arr_svob {
    size_t *data;
    size_t size_svob;
} arr_svob;

int kol_svob = 0;

uint64_t tick(void)
{
    uint32_t high, low;
    __asm__ __volatile__(
        "rdtsc\n"
        "movl %%edx, %0\n"
        "movl %%eax, %1\n"
        : "=r"(high), "=r"(low)::"%rax", "%rbx", "%rcx", "%rdx");

    uint64_t ticks = ((uint64_t)high << 32) | low;

    return ticks;
}


// push кладёт элемент на стек
int push(Stack_t *stack, const T value) {
    if (stack->size >= STACK_MAX_SIZE)
    {
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
    if (stack->size == 0) {
        printf("Ошибка! В стеке не осталось элементов\n");
        exit(STACK_UNDERFLOW);
    }
    stack->size--;
    //printf("%d", stack->data[stack->size]);
    return stack->data[stack->size];
}

void pop_back(Node_t **head)
{
     if (!head)
        return;
    if ((*head) == NULL)
        return;
    Node_t *temp;
    //void *p = (*head)->data;
    temp = (*head)->next;
    free(*head);
    (*head) = temp;
    //return p;
}

void pop_back_1(Node_t **head, arr_svob *mas_free)
{
    kol_svob++;
    mas_free->data[kol_svob] = &((*head)->value);
     if (!head)
        return;
    if ((*head) == NULL)
        return;
    Node_t *temp;
    //void *p = (*head)->data;
    temp = (*head)->next;
    free(*head);
    (*head) = temp;
    //return p;
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
    //*(head + 1) = NULL;
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


T pop2_nomas(Node_t **head) {
    //int *p = &((*head)->value);
    //printf("память %p\n", p);
    Node_t *out;
    T value;
    if (*head == NULL) {
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    value = out->value;
    //free(out);
    return value;
}

T pop2(Node_t **head, arr_svob *mas_free) {
    kol_svob++;
    mas_free->data[kol_svob] = &((*head)->value);
    //printf("память %p\n", p);
    Node_t *out;
    T value;
    if (*head == NULL) {
        printf("Ошибка! В стеке не осталось элементов");
        exit(STACK_UNDERFLOW);
    }
    out = *head;
    *head = (*head)->next;
    value = out->value;
    //free(out);
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
    printf("\n");
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
    printf("4. МАССИВ: Вывести убывающие подпоследовательности в обратном порядке(массив)\n");
    printf("5. МАССИВ: Вывести текущий стек\n\n");
    printf("6. СПИСОК: Ввести элементы стека\n");
    printf("7. СПИСОК: Добавить элемент в стек\n");
    printf("8. СПИСОК: Удалить элемент из стека\n");
    printf("9. СПИСОК: Вывести массив освободившихся адресов\n");
    printf("10. СПИСОК: Вывести убывающие подпоследовательности в обратном порядке(список)\n");
    printf("11. СПИСОК: Вывести элементы стека\n");
    printf("\n0. Выход\n");

    do {
        printf("\nВаш выбор: ");
        if (scanf("%d", &c) != 1)
        {
            printf("\nНеверные входные данные\n");
            scanf("%*s");
            c = -1;
        }
    } while (c < 0 || c > 12);

    printf("\n");

    return c;
}



int decsubseq_arr(Stack_t *stack_copy)
{
    int i;
    int el = pop(stack_copy);
    int count = 0;

    while (stack_copy->size > 0)
    {
        int arr[stack_copy->size];
        int new_el = pop(stack_copy);

        arr[0] = el;
        int i = 1;

        while (new_el > el)
        {
            arr[i] = new_el;
            i++;
            el = new_el;
            if (stack_copy->size > 0)
            {
                new_el = pop(stack_copy);
            }
        }
        el = new_el;
        if (i != 1)
        {
            count++;

            for (int j = 0; j < i; ++j)
            {
                printf("%d ", arr[j]);
            }
            printf("\n");
        }
    }

    return count;
}

int is_emptyl(Node_t *root)
{
    return !root;
}


int decsubseq_list(Node_t **root)
{
    int el = pop2_nomas(root);
    int count = 0;

    while (!is_emptyl(*root))
    {
        int arr[getSize_list(*root) + 1];
        int new_el = pop2_nomas(root);

        arr[0] = el;
        int i = 1;

        while (new_el > el)
        {
            arr[i] = new_el;
            i++;
            el = new_el;
            if (!is_emptyl(*root))
            {
                new_el = pop2_nomas(root);
            }
        }
        el = new_el;

        if (i != 1)
        {
            count++;

            for (int j = 0; j < i; ++j)
            {
                printf("%d ", arr[j]);
            }
            printf("\n");
        }

        //free(arr);
    }

    return count;
}

void variable_free(Node_t *vrb)
{
    free(vrb);
}

void list_free_all(Node_t *head)
{
    Node_t *next;

    for (; head; head = next)
    {
        next = head->next;
        variable_free(head);
    }
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
    int stack_status;
    int trig_vvod_mas = 0;
    int trig_vvod_list = 0;
    unsigned long long start, end;
    Stack_t stack;
    Stack_t stack_copy;
    Node_t *head = NULL;
    Node_t *head_copy = NULL;
    arr_svob svob_free;
    int mas_kol;
    Stack_t stack1, stack2, stack3;
    Node_t *head1 = NULL;
    Node_t *head2 = NULL;
    Node_t *head3 = NULL;

    stack.size = 0;
    stack_copy.size = 0;
    svob_free.data = malloc(100000);
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
            if (scanf("%d", &mas_kol) != 1)
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
            printf ("Если хотите вести элементы сами, введите 0\nЕсли хотите, чтобы стек был заполнен случайными элементами, введите 1: ");
            if (scanf("%d", &trig_vvod_mas) != 1)
            {
                printf("Вы ввели неверное значение!\n");
                return EXIT_FAILURE;
            }
            if (trig_vvod_mas != 0 && trig_vvod_mas != 1)
            {
                printf("Вы ввели неверное значение\n");
                return EXIT_FAILURE;
            }
            if (trig_vvod_mas == 0)
            {
                printf("Введите элементы стека, каждый с новой строки\n");
                for (i = 0; i < mas_kol; i++)
                {
                    if (scanf("%d", &x) != 1)
                    {
                        printf("Вы ввели неверное значение\n");
                        return EXIT_FAILURE;
                    }
                    push(&stack, x);
                    push(&stack_copy, x);
                }
            }
            else
            {
                srand(time(NULL)); 
                for (i = 0; i < mas_kol; i++)
                {
                    x = rand()%(0 - 100 + 1) + 100;
                    push(&stack, x);
                    push(&stack_copy, x);
                }
            }
            printf("Стек успешно создан\n");
            break;
        case 2:
            printf("Введите элемент, который хотите добавить: ");
            if (scanf("%d", &x) != 1)
            {
                printf("Вы ввели неверное значение\n");
                return EXIT_FAILURE;
            }
            if(push(&stack, x))
                return EXIT_FAILURE;
            push(&stack_copy, x);
            printf("Элемент успешно добавлен\n");
        	//print_list(&stack, x);
            break;
        case 3: 
        	pop(&stack);
                //return EXIT_FAILURE;
            pop(&stack_copy);
            printf("Элемент успешно удален\n");
            break;
        case 4:
            printf("Последовательности: ");
            start = tick();
            stack_status = decsubseq_arr(&stack_copy);
            end = tick();
            //head_copy = NULL;
            for (i = 0; i <stack.size; i++)
            {
                push(&stack_copy, stack.data[i]);
                //stack_copy.data[i] = stack.data[i];
            }
            if (!stack_status)
                printf("Нет таких последовательностей\n");
            printf("Время добавления: %llu\n", end - start);
            break;
        case 5:
            printf("Размер и сами элементы стека\n");
            printStack(&stack, printStackValue);
            printf("Объём занимаемой памяти стеком на основе массива, %lu\n\n", sizeof(Stack_t));
            break;
        case 6: 
            if (trig_1 == 1)
            {
                printf("Стек уже существует! Выход из программы\n");
                return EXIT_FAILURE;
            }
            printf("Введите количество элементов стека: ");
            if (scanf("%d", &list_kol) != 1)
            {
                printf("Вы ввели неверное значение\n");
                return EXIT_FAILURE;
            }
            printf("\n");
            trig_1 = 1;
            printf("Введите элементы стека, каждый с новой строки\n");
            //printStack_list(head);
                        printf ("Если хотите вести элементы сами, введите 0\nЕсли хотите, чтобы стек был заполнен случайными элементами, введите 1: ");
            if (scanf("%d", &trig_vvod_list) != 1)
            {
                printf("Вы ввели неверное значение!\n");
                return EXIT_FAILURE;
            }
            if (trig_vvod_list != 0 && trig_vvod_list != 1)
            {
                printf("Вы ввели неверное значение\n");
                return EXIT_FAILURE;
            }
            if (trig_vvod_list == 0)
            {
                for (i = 0; i < list_kol; i++)
                {
                    if(scanf("%d", &x) != 1)
                    {
                        printf("Вы ввели неверное значение\n");
                        return EXIT_FAILURE;
                    }
                    push_1(&head, x);
                    push_1(&head_copy, x);
                    //printStack_list(head);
                }
            }
            else
            {
                srand(time(NULL)); 
                for (i = 0; i < list_kol; i++)
                {
                    x = rand()%(0 - 100 + 1) + 100;
                    push_1(&head, x);
                    push_1(&head_copy, x);
                }
            }
            
            //printStack_list(head);
            break;
        case 7: 
            printf("Введите элемент, который хотите добавить: ");
            if (scanf("%d", &x) != 1)
            {
                printf("Вы ввели неверное значение\n");
                return EXIT_FAILURE;
            }
            if(push_1(&head, x))
                return EXIT_FAILURE;
            push_1(&head_copy, x);
            printf("Элемент успешно добавлен\n");
            break;
        case 8: 
        	pop2(&head, &svob_free);
            pop2_nomas(&head_copy);
            printf("Элемент успешно удален\n");
            break;
        case 9:
            if (kol_svob == 0)
            {
                printf("Массив пуст!\n\n");
            break;
            }
            printf("Массив освободившихся адресов: \n");
            for (i = 0; i < kol_svob; i++)
                printf("%p\n", &(svob_free.data[i]));
            break;
        case 10:
            printf("Последовательности: ");
            start = tick();
            stack_status = decsubseq_list(&head_copy);
            end = tick();
            head_copy = head;
            if (!stack_status)
                printf("Нет таких последовательностей\n");
            printf("Время добавления: %llu\n", end - start);
            break;
        case 11:
            printf("Элементы стека\n");
            printStack_list(head);
            printf("Объём занимаемой памяти стеком на основе списка, %lu\n\n", sizeof(Node_t)*getSize_list(head));
            break;
        case 12:
            //Stack_t stack1;
            stack1.size = 0;
            stack2.size = 0;
            stack3.size = 0;
            start = tick();
            for (i = 0; i < 10; i++)
                push(&stack1, i);
            end = tick();
            printf("Время добавления 10 массив память : %llu %zu\n", end - start, sizeof(stack1));
            start = tick();
            for (i = 0; i < 100; i++)
                push(&stack2, i);
            end = tick();
            printf("Время добавления 100 массив: %llu %zu\n", end - start, sizeof(int)*200);
            start = tick();
            for (i = 0; i < 1000; i++)
                push(&stack3, i);
            end = tick();
            printf("Время добавления 1000 массив: %llu %zu\n", end - start, sizeof(stack3));
/*
            start = tick();
            for (i = 0; i < 10; i++)
                pop(&stack1);
            end = tick();
            printf("Время удаления 10 массив: %llu\n", end - start);
            start = tick();
            for (i = 0; i < 100; i++)
                pop(&stack2);
            end = tick();
            printf("Время удаления 100 массив: %llu\n", end - start);
            start = tick();
            for (i = 0; i < 1000; i++)
                pop(&stack3);
            end = tick();
            printf("Время удаления 1000 массив: %llu\n", end - start);

*/
            start = tick();
            for (i = 0; i < 51; i++)
                push_1(&head1, i);
            end = tick();
            printf("Время добавления 55 список, память: %llu %zu\n", end - start, sizeof(Node_t)*getSize_list(head1));
            start = tick();
            for (i = 0; i < 30; i++)
                push_1(&head2, i);
            end = tick();
            printf("Время добавления 100 список, память: %llu %zu\n", end - start, sizeof(Node_t)*getSize_list(head2));
            start = tick();
            for (i = 0; i < 1000; i++)
                push_1(&head3, i);
            end = tick();
            printf("Время добавления 1000 список, память: %llu %zu\n", end - start, sizeof(Node_t)*getSize_list(head3));

            start = tick();
            for (i = 0; i < 10; i++)
                pop_back(&head1);
            end = tick();
            printf("Время удаления 10 список: %llu\n", end - start);
            start = tick();
            for (i = 0; i < 100; i++)
                pop_back(&head2);
            end = tick();
            printf("Время удаления 100 список: %llu\n", end - start);
            start = tick();
            for (i = 0; i < 1000; i++)
                pop_back(&head3);
            end = tick();
            printf("Время удаления 1000 список: %llu\n", end - start);


            break;
        case 0: 
        	return 0;
        }
    }
}

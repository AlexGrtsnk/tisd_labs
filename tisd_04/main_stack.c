#include <stdio.h>
#include <stdlib.h>


#define STACK_OVERFLOW  -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY   -102


#define LEN_NAME 26
#define PROD_LEN 15
#define FILE_ERROR 2
#define COST_ERROR 3
#define LEN_ERROR 4
#define ARG_ERROR 5

typedef struct
{
    char name[LEN_NAME];
    int price;
} product_t;


 
typedef int T;

typedef struct Node_tag {
    T value;
    struct Node_tag *next;
} Node_t;

typedef struct node node_t;
struct node 
{
    void *data;
    node_t *next; 
};




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

void push_3(node_t **head, T value) {
    node_t *tmp = malloc(sizeof(node_t));
    if (tmp == NULL) {
        printf("Ошибка! Переполнение стека");
        exit(STACK_OVERFLOW);
    }
    tmp->next = *head;
    tmp->data = &(value);
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



void printStack_list_new(const node_t* head) {
        //int ia = *(const int *)a;
    int ib;
    char ia[256];
    product_t ta;
    printf("stack >");
    while (head) {
        ta = *(const product_t *)head->data;
        strcpy(ia, ta.name);
        ib = ta.price;
        printf("%s, %d\n", ia, ib);
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

Node_t* addelem(Node_t *lst, int number)
{
  Node_t *p = NULL;
  Node_t *temp = malloc(sizeof(Node_t));
  p = lst->next; // сохранение указателя на следующий узел
  lst->next = temp; // предыдущий узел указывает на создаваемый
  temp->value = number; // сохранение поля данных добавляемого узла
  temp->next = p; // созданный узел указывает на следующий элемент
  return(temp);
}

Node_t* list_add_end(Node_t *head, Node_t *pers)
{
    Node_t *cur = head;

    if (!head)
        return pers;

    for ( ; cur->next; cur = cur->next)
        ;

    cur->next = pers;

    return head;
}


void append(Node_t **head_a, Node_t **head_b) // не так. это не конец списка
{
    Node_t *cur = *head_a;
    if (*head_a == NULL && *head_b == NULL)
        return;
    //(*head_a)->next = *head_b;
    for ( ; cur->next; cur = cur->next)
        ;
    cur->next = *head_b;
    *head_b = NULL;

    //return head_a;
}


Node_t* getLastButOne(Node_t* head) {
    if (head == NULL) {
        exit(-2);
    }
    if (head->next == NULL) {
        return NULL;
    }
    while (head->next->next) {
        head = head->next;
    }
    return head;
}

void popBack(Node_t **head) {
    Node_t *lastbn = NULL;
    //Получили NULL
    if (!head) {
        exit(-1);
    }
    //Список пуст
    if (!(*head)) {
        exit(-1);
    }
    lastbn = getLastButOne(*head);
    //Если в списке один элемент
    if (lastbn == NULL) {
        free(*head);
        *head = NULL;
    } else {
        free(lastbn->next);
        lastbn->next = NULL;
    }
}

int pop_back(Node_t **head) // хз как
{
    Node_t *copy_new = NULL;
    Node_t *head_cp = *head;
    Node_t *head_cp_1 = *head;
    int i;
    copy_new = *head;
    while (head_cp->next->next)
    {
        head_cp = (head_cp)->next;
    }
    copy_new = head_cp;
    if (copy_new == NULL) {
        free(*head);
        *head = NULL;
    } else {
        i = copy_new->next->value;
        free(copy_new->next);
        copy_new->next = NULL;
    }
    return i;
}

void* pop_back_gt(node_t **head) // вроде работает
{
    node_t *copy_new = NULL;
    node_t *head_cp = *head;
    node_t *head_cp_1 = *head;
    void *p;
    copy_new = *head;
    if (*head == NULL)
        return NULL;
    if((*head)->data == NULL)
        return NULL;
    while (head_cp->next->next)
    {
        head_cp = (head_cp)->next;
    }
    copy_new = head_cp;
    if (copy_new == NULL) {
        free(*head);
        *head = NULL;
    } else {
        p = copy_new->next->data;
        free(copy_new->next);
        copy_new->next = NULL;
        return p;
    }
}

int pop_front(Node_t **head)
{

    int i = (*head)->value;
    if (*head == NULL)
        return 0;
    if((*head)->value == -9)
        return 0;
    (*head) = (*head)->next;
    return i;
}

node_t* reverse(node_t *head)
{
    if (head == NULL)
        return EXIT_FAILURE;
    node_t * curr, *next, *prev = NULL;
    curr = head;
    while(curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    head = prev;
    return head;
}

int cmp_prod(const void *a, const void *b)
{
    product_t ta = *(const product_t *)a;
    product_t tb = *(const product_t *)b;
    int trig_1 = 0;
    int trig_2 = 0;
    if (strcmp(ta.name, tb.name) == 0)
        trig_1 = 1;
    if (ta.price == tb.price)
        trig_2 = 1;
    if (trig_1 + trig_2 == 2)
        return 1;
    else
        return 0;
}

int main(void) {
    long i;
    Node_t *head = NULL;
    Node_t *head1 = NULL;
    node_t *head3 = malloc(sizeof(node_t)*3);
    head3->next = malloc(sizeof(node_t)*3);
    head3->next->next = malloc(sizeof(node_t)*3);
    Node_t *tmp = head1;
    Node_t *head_cp = head1;
    product_t t1;
    product_t t2;
    product_t t3;
    char s1[128] = "aaasadas";
    char s2[128] = "fdhjsfsh";
    char s3[128] = "dasdaasd";
    strcpy(t1.name, s1);
    t1.price = 100;
    strcpy(t2.name, s2);
    t2.price = 200;
    strcpy(t3.name, s3);
    t3.price = 300;
    //head = malloc(sizeof(node_t)*3);
    int k = 1;
    int l = 2;
    int m = 3;
        for (i = 0; i < 10; i++) {
        push_1(&head1, i);
    }
    for (i = 0; i < 3; i++) {
//        tmp->value = i;
//        tmp->next = NULL;
        //head = list_add_end(head, head1);
        //head1 = head1->next;
        //tmp = NULL;
        
        push_1(&head, i);
    }
    printf("ПЕРЕД\n");
    printStack_list(head);
    printf("\n");
    append(&head, &head1);
    //head = head ->next;
    //head->next = head1;
    printf("lj,fdktybt \n");
    //pop_back(&head);
    //printStack_list_new(head);
    printStack_list(head);
    //printStack_list(head1);
    printf("sdasd\n");
    //printf("%d\n", (pop_front(&head1)));
    //printStack_list(head1);
    printf("sadsad\n");
    //push_3(&head3, 1);
    //push_3(&head3, 2);
    //push_3(&head3, 3);
    head3->data = &t1;
    head3->next->data = &t2;
    head3->next->next->data = &t3;
    void *data1 = &(t2);
    printf("res: %d\n", cmp_prod(head3->next->next->data, data1));
    printStack_list_new(head3);
    printf("\nasdadsad");
    //head3 = reverse(head3);
    void *ik;
    ik = pop_back(&head3);
    printStack_list_new(head3);
    //printf("size = %d\n", getSize_list(head));
    //pop2(&head);
    //printf("ывфывфыв\n");
    //printStack_list(head);
    //free(head);
    return EXIT_SUCCESS;
}
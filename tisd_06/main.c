//copy from macos

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define STR_SIZE 128
#define SPACING 5

#define FOUND 1
#define NOT_FOUND 0
#define OK 0
#define FILE_ERROR 1
#define VAL_ERROR 2



typedef struct tree_node
{
    char *val;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

typedef struct
{
    tree_node **arr;
    int size;
    int mem_size;
} dynarr_t;

typedef struct lst
{
    char *value;
    struct lst *next;
} lst_t;

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


int is_prime(unsigned int num)
{
    if (num <= 1)
        return 0;
    if (num % 2 == 0 && num > 2)
        return 0;
    for (int i = 3; i < floor(sqrt(num)); i += 2)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

unsigned int next_prime(int n)
{
    unsigned int prime = n + 1;
    while (!is_prime(prime))
    {
        prime++;
    }

    return prime;
}

int safe_hash(char *s, int n)
{
    unsigned long hash = 0;
    int c;

    while ((c = *s++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % n;
}

int unsafe_hash(char *s, const int n)
{
    unsigned long hash = 0;

    while (*s)
    {
        hash += *s++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % n;
}

int file_len(FILE *f)
{
    rewind(f);

    int lines = 0;
    char buffer[STR_SIZE];

    while (fgets(buffer, STR_SIZE, f) != NULL)
    {
        lines++;
    }

    rewind(f);

    return lines;
}

int fbytes(FILE *f)
{
    rewind(f);
    fseek(f, 0, SEEK_END);
    unsigned size = ftell(f);
    rewind(f);

    return size;
}

int build_hash_table(lst_t **arr, int size, FILE *f, int (*hash)(char *, int))
{
    char buffer[STR_SIZE];
    int max_collisions = 0;

    while (fgets(buffer, STR_SIZE, f) != NULL)
    {
        int cur_collisions = 0;
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        if (deepcmp(buffer, (*arr)[hash(buffer, size)]) == 0)
        {
            cur_collisions = linsert(&(*arr)[hash(buffer, size)], buffer);
        }

        if (cur_collisions > max_collisions)
        {
            max_collisions = cur_collisions;
        }
    }

    rewind(f);

    return max_collisions;
}

void print_hash_table(lst_t *arr, int size)
{
    printf(
           "\n"
           "  ХЕШ | СООТВЕТСТВУЮЩИЕ ДАННЫЕ\n"
           "------------------------------------------\n" );
    lst_t *entry;

    int f = 0;

    for (int i = 0; i < size; i++)
    {
        f = 0;

        entry = &arr[i];
        if (entry->value != NULL)
        {
            printf("%5d | ", i);
            f = 1;
        }
        while (entry != NULL)
        {
            if (entry->value != NULL)
                printf("\"%s\" ", entry->value);
            entry = entry->next;
        }
        if (f)
            printf("\n------------------------------------------\n");
    }
}


void welcome()
{
    printf("%s",
           
           "Данная программа позволяет построить ДДП, сбалансированное двоичное дерево \n"
           "и хеш-таблицу по указанным данным, вывести на экран дерево в виде дерева, \n"
           "провести реструктуризацию хеш-таблицы, если среднее количество сравнений \n"
           "больше указанного, вывести количественную характеристику моделирования. \n\n" );
}




int linsert(lst_t *arr, char *buffer)
{
    if (arr->next == NULL)
    {
        arr->next = calloc(1, sizeof(lst_t));
        arr->value = malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(arr->value, buffer);

        return 1;
    }

    int depth = 1;
    depth += linsert(arr->next, buffer);

    return depth;
}

int deepcmp(char *s, lst_t l)
{
    lst_t *lp = &l;
    int i = 0;

    while (lp != NULL && lp->value != NULL)
    {
        i++;
        if (strcmp(s, lp->value) == 0)
            return i;
        lp = lp->next;
    }

    return 0;
}

int lst_occupation(lst_t *arr, int size)
{
    int occupation = 0;
    for (int i = 0; i < size; i++)
    {
        if (arr[i].value != NULL)
            occupation++;
    }

    return occupation;
}

static void free_lst(lst_t *node)
{
    if (node == NULL)
        return;

    free_lst(node->next);

    free(node);
}

void free_lst_arr(lst_t *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        free_lst(arr[i].next);
    }

    free(arr);
}


char **fill_words(FILE *f)
{
    rewind(f);

    unsigned long flen = file_len(f);

    int ind = 0;
    char buffer[STR_SIZE];
    char **words = (char **)malloc(sizeof(char *) * flen);
    for (int i = 0; i < flen; ++i)
    {
        words[i] = (char *)malloc(sizeof(char) * STR_SIZE);
    }

    while (fgets(buffer, STR_SIZE, f) != NULL)
    {
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        strcpy(words[ind], buffer);
        ind++;
    }

    rewind(f);

    return words;
}

void free_words(char **words, unsigned long flen)
{
    for (int i = 0; i < flen; ++i)
    {
        free(words[i]);
    }

    free(words);
}

int search_tree(char *to_find, tree_node root)
{
    int i = 0;
    tree_node *p = &root;

    while (p != NULL)
    {
        i++;
        if (strcmp(p->val, to_find) == 0)
            return i;

        if (strcmp(p->val, to_find) > 0)
            p = p->right;
        else
            p = p->left;
    }

    return NOT_FOUND;
}

double search_tree_av(tree_node root, char **words, unsigned long flen)
{
    uint64_t time = tick();
    for (int i = 0; i < flen; ++i)
    {
        search_tree(words[i], root);
    }
    time = tick() - time;

    return (double)time / flen;
}

int search_hashtable(char *to_find, lst_t *hash_lst, int n, int (*hash)(char *, int))
{
    int ec = deepcmp(to_find, hash_lst[hash(to_find, n)]);
    if (ec > 0)
        return ec;

    return NOT_FOUND;
}

double search_hashtable_av(lst_t *hash_lst, int n, int (*hash)(char *, int),
                           char **words, unsigned long flen)
{
    uint64_t time = tick();
    for (int i = 0; i < flen; ++i)
    {
        search_hashtable(words[i], hash_lst, n, hash);
    }
    time = tick() - time;

    return (double)time / flen;
}

int search_file(char *to_find, FILE *f)
{
    rewind(f);

    int i = 0;
    char buffer[STR_SIZE];

    while (fgets(buffer, STR_SIZE, f) != NULL)
    {
        i++;
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';

        if (strcmp(to_find, buffer) == 0)
        {
            return i;
        }
    }

    rewind(f);

    return NOT_FOUND;
}

double search_file_av(FILE *f, char **words, unsigned long flen)
{
    uint64_t time = tick();
    for (int i = 0; i < flen; ++i)
    {
        search_file(words[i], f);
    }
    time = tick() - time;

    return (double)time / flen;
}

tree_node *create_node(char *val)
{
    tree_node *node = malloc(sizeof(tree_node));

    node->val = malloc(sizeof(char *) * (strlen(val) + 1));
    strcpy(node->val, val);

    node->left = NULL;
    node->right = NULL;

    return node;
}

tree_node *insert(tree_node *node, char *val)
{
    if (node == NULL)
    {
        return create_node(val);
    }

    if (strcmp(val, node->val) > 0)
    {
        node->left = insert(node->left, val);
    }
    else
    {
        if (strcmp(val, node->val) < 0)
        {
            node->right = insert(node->right, val);
        }
    }

    return node;
}

void fill_tree(tree_node **root, FILE *f)
{
    char buffer[STR_SIZE];

    fgets(buffer, STR_SIZE, f);
    if (buffer[strlen(buffer) - 1] == '\n')
    {
        buffer[strlen(buffer) - 1] = '\0';
    }

    *root = insert(*root, buffer);

    while (fgets(buffer, STR_SIZE, f) != NULL)
    {
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        insert(*root, buffer);
    }

    rewind(f);
}

void print_tree(tree_node *root, int space)
{
    if (root == NULL)
    {
        return;
    }

    space += SPACING;

    print_tree(root->right, space);

    printf("\n");
    for (int i = SPACING; i < space; i++)
    {
        printf(" ");
    }
    printf("%s\n", root->val);

    print_tree(root->left, space);
}

void tree_depth(tree_node *root, int *vrtxs, int *cmprs, int depth)
{
    if (!root)
    {
        return;
    }
    (*vrtxs)++;
    (*cmprs) += depth;

    tree_depth(root->right, vrtxs, cmprs, depth + 1);
    tree_depth(root->left, vrtxs, cmprs, depth + 1);
}

void push_back(dynarr_t *arr, tree_node *to_add)
{
    arr->size++;
    if (arr->size >= arr->mem_size)
    {
        arr->mem_size *= 2;
        arr->arr = realloc(arr->arr, arr->mem_size * sizeof(tree_node *));
    }

    for (int i = arr->size; i > 0; i--)
    {
        arr->arr[i] = arr->arr[i - 1];
    }

    arr->arr[0] = to_add;
}

void store_nodes(tree_node *root, dynarr_t *arr)
{
    if (root == NULL)
    {
        return;
    }

    store_nodes(root->left, arr);
    push_back(arr, root);
    store_nodes(root->right, arr);
}

tree_node *build_tree(dynarr_t *arr, int start, int end)
{
    if (start > end)
    {
        return NULL;
    }

    int mid = (start + end) / 2;
    tree_node *root = arr->arr[mid];

    root->right = build_tree(arr, start, mid - 1);
    root->left = build_tree(arr, mid + 1, end);

    return root;
}

tree_node *balance_tree(tree_node *root, int *unique)
{
    dynarr_t arr;
    arr.mem_size = 8;
    arr.size = 0;
    arr.arr = malloc(sizeof(tree_node *) * arr.mem_size);
    store_nodes(root, &arr);

    int n = arr.size;
    *unique = n;

    tree_node *r = build_tree(&arr, 0, n - 1);

    free(arr.arr);

    return r;
}

void free_tree(tree_node *node)
{
    if (node == NULL)
    {
        return;
    }

    free_tree(node->left);
    free_tree(node->right);

    free(node);
}


int main(int argc)
{
    welcome();
    char name[256];
    printf("Пожалуйста, введите имя файла с расшерением\n");
    scanf("%s", &name);
    FILE *f = fopen(name, "r");
    if (f == NULL)
    {
        printf(
               "Неверное имя файла! Повторите попытку.\n" );
        return FILE_ERROR;
    }

    if (fgetc(f) == EOF)
    {
        printf(
               "Файл пуст! Проверьте файл.\n" );
        return FILE_ERROR;
    }

    rewind(f);

    char **words = fill_words(f);
    unsigned long flen = file_len(f);
    tree_node *root = NULL;

    uint64_t time = tick();
    fill_tree(&root, f);
    time = tick() - time;

    printf(
           "БИНАРНОЕ ДЕРЕВО НА ОСНОВЕ ДАННЫХ ИЗ ФАЙЛА:\n" );
    print_tree(root, 0);
    
    printf(
           "Дерево построено за = %ld тактов процессора "
           "(в данное время включено время чтения из файла).\n\n" ,
           time);

    int unique;
    time = tick();
    root = balance_tree(root, &unique);
    time = tick() - time;
    printf(
           "БИНАРНОЕ ДЕРЕВО ПОСЛЕ БАЛАНСИРОВКИ:\n" );
    
    print_tree(root, 0);
    
    printf(
           "Дерево сбалансировано за = %ld тактов процессора.\n\n" ,
           time);

    int n = next_prime(file_len(f));
    lst_t *hash_lst = calloc(n, sizeof(lst_t));
    int cmpr, desired_cmpr;

    int (*hash)(char *, int);

    time = tick();
    unsafe_hash("chill", 10);
    time = tick() - time;
    printf(
           "%ld - генерация простой хеш-функции (в тактах процессора).\n" ,
           time);

    time = tick();
    safe_hash("chill", 10);
    time = tick() - time;
    printf(
           "%ld - генерация сложной хеш-функции (в тактах процессора).\n" ,
           time);

    hash = &unsafe_hash;
    time = tick();
    cmpr = build_hash_table(&hash_lst, n, f, hash);
    time = tick() - time;

    printf(
           "\nХЕШ-ТАБЛИЦА НА ОСНОВЕ ПРОСТОЙ ФУНКЦИИ:" );
    print_hash_table(hash_lst, n);
    printf(
           "Максимальное количество коллизий = %d.\n" ,
           cmpr);
    printf(
           "Хеш-таблица построена за = %ld тактов процессора "
           "(в данное время включено время чтения из файла).\n\n" ,
           time);

    printf("\nВведите допустимое количество коллизий: ");
    int ec = scanf("%d", &desired_cmpr);
    if (ec != 1)
    {
        printf( "Введено неверное значение!\n" );

        return VAL_ERROR;
    }

    if (cmpr > desired_cmpr)
    {
        while (cmpr != desired_cmpr)
        {
            free_lst_arr(hash_lst, n);
            n = next_prime(n);
            hash_lst = calloc(n, sizeof(lst_t));

            hash = &safe_hash;
            time = tick();
            cmpr = build_hash_table(&hash_lst, n, f, hash);
            time = tick() - time;
        }
        printf(
               "\nХЕШ-ТАБЛИЦА НА ОСНОВЕ УЛУЧШЕННОЙ ФУНКЦИИ:" );
        print_hash_table(hash_lst, n);
        printf(
               "Количество коллизий = %d.\n" ,
               cmpr);
        printf(
               "Хеш-таблица построена за = %ld тактов процессора "
               "(в данное время включено время чтения из файла).\n\n" ,
               time);
    }
    else
    {
        printf(
               "Результат достижим за введенное количество коллизий.\n"
               "Пересоздание таблицы не требуется.\n" );
    }

    char to_find[STR_SIZE];
    printf("\nВведите слово которое хотите найти: ");
    scanf("%s", to_find);

    int depth = 0;
    int vrtxs = 0;
    int cmprs = 0;
    tree_node *root2 = NULL;
    fill_tree(&root2, f);
    time = tick();
    ec = search_tree(to_find, *root2);
    time = tick() - time;
    tree_depth(root2, &vrtxs, &cmprs, depth);
    if (ec > 0)
    {
        printf( "ПОИСК В БИНАРНОМ ДЕРЕВЕ\n" );
        printf("Слово \"%s\" найдено за %ld тактов процессора.\n"
               "Среднее время поиска в бинарном дереве - %lf тактов процессора.\n"
               "Бинарное дерево занимает - %ld байт.\n"
               "Количество сравнений для достижения результата - %d.\n"
               "Среднее количество сравнений - %f.\n",
               to_find, time, search_tree_av(*root2, words, flen),
               unique * sizeof(tree_node), ec,
               (double)cmprs / vrtxs);
    }
    else
    {
        printf(
               "Слово \"%s\" не найдено.\n" ,
               to_find);

        return VAL_ERROR;
    }
    free_tree(root2);

    depth = 0;
    vrtxs = 0;
    cmprs = 0;
    time = tick();
    ec = search_tree(to_find, *root);
    time = tick() - time;
    tree_depth(root, &vrtxs, &cmprs, depth);
    if (ec > 0)
    {
        printf( "ПОИСК В БИНАРНОМ СБАЛАНСИРОВАННОМ ДЕРЕВЕ\n" );
        printf("Слово \"%s\" найдено за %ld тактов процессора.\n"
               "Среднее время поиска в бинарном сбалансированном дереве - %lf тактов процессора.\n"
               "Бинарное сбалансированное дерево занимает - %ld байт.\n"
               "Количество сравнений для достижения результата - %d.\n"
               "Среднее количество сравнений - %f.\n",
               to_find, time, search_tree_av(*root, words, flen),
               unique * sizeof(tree_node), ec,
               (double)cmprs / vrtxs);
    }
    else
    {
        printf(
               "Слово \"%s\" не найдено.\n" ,
               to_find);

        return VAL_ERROR;
    }
    free_tree(root);

    time = tick();
    ec = search_hashtable(to_find, hash_lst, n, hash);
    time = tick() - time;
    if (ec > 0)
    {
        printf( "ПОИСК В ХЕШ-ТАБЛИЦЕ\n" );
        printf("Слово \"%s\" найдено за %ld тактов процессора.\n"
               "Среднее время поиска в хеш-таблице - %lf тактов процессора.\n"
               "Хеш-таблица занимает - %ld байт.\n"
               "Количество сравнений для достижения результата - %d.\n"
               "Среднее количество сравнений - %f.\n",
               to_find, time / 4, search_hashtable_av(hash_lst, n, hash, words, flen),
               (n + unique - lst_occupation(hash_lst, n)) * sizeof(lst_t) + sizeof(lst_t *), ec,
               (double)(1 + desired_cmpr) / 2);
    }
    else
    {
        printf(
               "Слово \"%s\" не найдено.\n" ,
               to_find);

        return VAL_ERROR;
    }
    free_lst_arr(hash_lst, n);

    time = tick();
    ec = search_file(to_find, f);
    time = tick() - time;
    if (ec > 0)
    {
        printf( "ПОИСК В ФАЙЛЕ\n" );
        printf("Слово \"%s\" найдено за %ld тактов процессора.\n"
               "Среднее время поиска в файле - %lf тактов процессора.\n"
               "Файл занимает - %u байт.\n"
               "Количество сравнений для достижения результата - %d.\n"
               "Среднее количество сравнений - %f.\n",
               to_find, time, search_file_av(f, words, flen),
               fbytes(f), ec, (double)file_len(f) / 2);
    }
    else
    {
        printf(
               "Слово \"%s\" не найдено.\n" ,
               to_find);

        return VAL_ERROR;
    }

    fclose(f);

    return OK;
}
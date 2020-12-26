//copy from macos

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

#define GOT_ARG 1

#define EPS 1e-6


#define EOK 0


#define EINVALIDINTINPUT 141
#define EINVALIDINTER 142


#define EARRAYMEMORY 151


#define EQUEUEOVERFLOW 161
#define EQUEUEEMPTY 162
#define EQUEUEMEMORY 163


typedef struct
{
    double time_out;
    int num;
} task_t;


typedef struct queuenode
{
    task_t task;
    struct queuenode *next;
} queuenode_t;

typedef struct
{
    unsigned capacity, size;
    queuenode_t *front, *rear;
} queuelist_t;


typedef struct
{
    size_t *arr;
    int capacity;
    int ind;
} arr_t;

typedef struct
{
    unsigned capacity, size, rear, front;
    task_t *arr;
} queuearr_t;


#define errcheck(err, len, queuearr, queuelist, fmem, msg) \
    for (int i = 0; i < len; ++i)                          \
    {                                                      \
        if (errno == err[i])                               \
        {                                                  \
            printf("%s", msg);                             \
            freequeuearr(queuearr);                        \
            freequeuelist(queuelist, fmem);                \
            free_array(fmem);                              \
            return EOK;                                    \
        }                                                  \
    }


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

double get_time_my(const int llim, const int rlim)
{
    double rnd = (double)rand() / (double)RAND_MAX;

    return (rlim - llim) * rnd + llim;
    //return 0.5;
}

arr_t *create_array(const int capacity)
{
    arr_t *arr = (arr_t *)malloc(sizeof(arr_t));
    if (!arr)
    {
        return NULL;
    }

    arr->capacity = capacity;
    arr->ind = -1;
    arr->arr = (size_t *)malloc(arr->capacity * sizeof(size_t));

    return arr;
}

void output_array(const arr_t arr)
{
    if (arr.ind == -1)
    {
        printf("%s", "Массив пуст");
    }
    else
    {
        for (int i = 0; i < arr.ind + 1; ++i)
        {
            printf("%zx ", arr.arr[i]);
        }
    }

    printf("\n");
}

void free_array(arr_t *arr)
{
    if (arr)
    {
        if (arr->arr)
        {
            free(arr->arr);
            free(arr);
            errno = EOK;

            return;
        }

        free(arr);
        errno = EOK;

        return;
    }

    errno = EARRAYMEMORY;
}


void welcome()
{
    printf("%s",
           
           "Данная программа позволяет сравнить время, затраченное на обработку \n"
           "очереди, реализованной на основе массива и односвязного линейного списка. \n"
           "Под обработкой подразумевается обработка деятельности обслуживающего аппарата. \n\n" );
    printf("%s",
           
           "Выберите действие из списка, введя соответствуюший номер:\n" );
    printf("%s",
           
           "1. МАССИВ: Ввести данные обслуживающего аппарата и вывести статистику работы.\n"
           "2. МАССИВ: Вывести количественную характеристику выполнения операций над очередью.\n\n"
           "3. СПИСОК: Ввести данные обслуживающего аппарата и вывести статистику работы.\n"
           "4. СПИСОК: Вывести количественную характеристику выполнения операций над очередью.\n\n"
           "0. Выход из программы.\n\n" );
}

void clean_input_stream()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void input_interval(int *const num, const int left, const int right)
{
    if (scanf("%d", num) != GOT_ARG)
    {
        clean_input_stream();
        errno = EINVALIDINTINPUT;
        return;
    }

    if (*num < left || *num > right)
    {
        clean_input_stream();
        errno = EINVALIDINTER;
        return;
    }
}

queuearr_t *create_queuearr(const unsigned capacity)
{
    queuearr_t *queue = (queuearr_t *)malloc(sizeof(queuearr_t));
    if (!queue)
    {
        return NULL;
    }
    queue->capacity = capacity;
    queue->front = 0;
    queue->size = 0;
    queue->rear = capacity - 1;
    queue->arr = (task_t *)malloc(queue->capacity * sizeof(task_t));
    if (!queue->arr)
    {
        free(queue);
        return NULL;
    }

    return queue;
}

int is_fullarr(queuearr_t *const queue)
{
    return queue->size == queue->capacity;
}

int is_emptyarr(queuearr_t *const queue)
{
    return queue->size == 0;
}

void enqueuearr(queuearr_t *const queue, const task_t task)
{
    if (is_fullarr(queue))
    {
        errno = EQUEUEOVERFLOW;
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->arr[queue->rear] = task;
    queue->size++;
}

task_t dequeuearr(queuearr_t *const queue)
{
    task_t task = {.time_out = 0, .num = 0};
    if (is_emptyarr(queue))
    {
        errno = EQUEUEEMPTY;
        return task;
    }
    task = queue->arr[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    return task;
}

void freequeuearr(queuearr_t *queue)
{
    if (queue)
    {
        if (queue->arr)
        {
            free(queue->arr);
            free(queue);
            errno = EOK;

            return;
        }

        free(queue);
        errno = EOK;

        return;
    }
}

queuenode_t *create_queuenode(const task_t task)
{
    queuenode_t *node = (queuenode_t *)malloc(sizeof(queuenode_t));
    if (!node)
    {
        return NULL;
    }
    node->task.num = task.num;
    node->task.time_out = task.time_out;
    node->next = NULL;

    return node;
}

queuelist_t *create_queuelist(const unsigned capacity)
{
    queuelist_t *queue = (queuelist_t *)malloc(sizeof(queuelist_t));
    if (!queue)
    {
        return NULL;
    }
    queue->rear = NULL;
    queue->front = NULL;
    queue->size = 0;
    queue->capacity = capacity;

    return queue;
}

int is_emptylist(queuelist_t *const queue)
{
    return queue->rear == NULL;
}

int is_fulllist(queuelist_t *const queue)
{
    return queue->size == queue->capacity;
}

void enqueuelist(queuelist_t *const queue, const task_t task)
{
    queuenode_t *node = create_queuenode(task);

    if (is_fulllist(queue))
    {
        errno = EQUEUEOVERFLOW;
        return;
    }

    if (queue->rear == NULL)
    {
        queue->front = node;
        queue->rear = node;
        queue->size++;
        return;
    }

    queue->rear->next = node;
    queue->rear = node;
    queue->size++;
}

task_t dequeuelist(queuelist_t *const queue, arr_t *const fmem)
{
    task_t task = {.time_out = 0, .num = 0};
    if (queue->front == NULL)
    {
        errno = EQUEUEEMPTY;
        return task;
    }

    queuenode_t *node = queue->front;
    fmem->arr[++fmem->ind] = (size_t)node;
    task = node->task;

    if (queue->front == queue->rear)
    {
        queue->rear = NULL;
        queue->front = NULL;
    }
    else
    {
        queue->front = queue->front->next;
    }

    free(node);

    queue->size--;

    return task;
}

void freequeuelist(queuelist_t *queue, arr_t *const fmem)
{
    if (queue == NULL)
    {
        return;
    }

    while (!is_emptylist(queue))
    {
        dequeuelist(queue, fmem);
    }

    free(queue);
}

int check_rear(queuelist_t *queue, arr_t *fmem)
{
    size_t top = (size_t)queue->rear;
    int is_found = 0;

    for (int i = 0; i < fmem->ind; ++i)
    {
        if (top == fmem->arr[i])
        {
            is_found = 1;
            for (int j = i; j < fmem->ind - 1; ++j)
            {
                size_t temp = fmem->arr[j];
                fmem->arr[j] = fmem->arr[j + 1];
                fmem->arr[j + 1] = temp;
            }
        }
    }

    if (is_found)
    {
        fmem->ind--;
    }

    return is_found;
}


void list_model(queuelist_t *const queue, arr_t *const fmem,
                const int llim_in, const int rlim_in,
                const int llim_out, const int rlim_out,
                const int repeats)
{
    int in_tasks = 0;
    int out_tasks = 0;
    int failed_tasks = 0;
    int calls = 0;
    double service_time = 0;
    double hold_time = 0;
    size_t max_memory = 0;
    size_t m = 0;
    double time_in = get_time_my(llim_in, rlim_in);
    double time_out = -1;

    task_t task_todo;

    int reusedmem = 0;
    int newmem = 0;
    int which;

    int overall_len = 0;
    //uint64_t vrem_1 = tick();
    while (out_tasks != 1000)
    {
        if (time_out < 0 || time_in < time_out)
        {
            if (time_out < 0)
            {
                hold_time += time_in;
            }

            service_time += time_in;
            time_out -= time_in;
            time_in = get_time_my(llim_in, rlim_in);

            task_t task = {.num = 0,
                           .time_out = get_time_my(llim_out, rlim_out)};

            enqueuelist(queue, task);

            if (errno == EQUEUEOVERFLOW)
            {
                printf("Очередь переполнена! Новый элемент не будет добавлен!\n");
                failed_tasks++;
            }
            else
            {
                which = check_rear(queue, fmem);
                if (which)
                {
                    reusedmem++;
                }
                else
                {
                    newmem++;
                }

                in_tasks++;
            }

            if (time_out < 0 && queue->size)
            {
                task_todo = dequeuelist(queue, fmem);
                time_out = task_todo.time_out;
            }
        }
        else
        {
            time_in -= time_out;
            service_time += time_out;
            time_out = 0;

            task_todo.num++;
            calls++;

            if (task_todo.num < repeats)
            {
                task_todo.time_out = get_time_my(llim_out, rlim_out);
                enqueuelist(queue, task_todo);

                if (errno == EQUEUEOVERFLOW)
                {
                    printf("Очередь переполнена! Новый элемент не будет добавлен!\n");
                    failed_tasks++;
                }
                else
                {
                    which = check_rear(queue, fmem);
                    if (which)
                    {
                        reusedmem++;
                    }
                    else
                    {
                        newmem++;
                    }
                }
            }
            else
            {
                overall_len += queue->size;
                out_tasks++;
                if (out_tasks % 100 == 0)
                {
                    printf(
                           "\n---\n"
                           "Обработано %d заявок.\n"
                           "Длина очереди: %d\n"
                           "Средняя длина очереди: %d\n"
                           "---\n" ,
                           out_tasks, queue->size, overall_len / out_tasks);
                }
            }

            if (is_emptylist(queue))
            {
                time_out = -1;
            }
            else
            {
                task_todo = dequeuelist(queue, fmem);
                time_out = task_todo.time_out;
            }
        }
        //m = queue->size*(sizeof(task_t) + sizeof(unsigned int)*2) + sizeof(unsigned int)*2;
        /*
        m = queue->size;
        if (m > max_memory)
            max_memory = queue->size;
        printf("SIZE SIZE ПАМЯТЬ ПАМЯТЬ!!!!! %lu\n", (sizeof(task_t) + sizeof(unsigned int)*2)*1000 + sizeof(unsigned int)*2);

        //printf("ПАМЯТЬ ПАМЯТЬ!!!!! %zu\n", sizeof(queue)*10);
        */
    }
    //uint64_t vrem_2 = tick();
    //printf("ВРЕМЯ ВЫПОЛНЕНИЯ НА МАССИВЕ %llu\n\n", vrem_2 - vrem_1);
    //printf("DHDHJAHJDJHAH ПАМЯТЬ ПАМЯТЬ ПАМЯТЬ МАКСИМАЛЬНАЯ %lu\n\n", max_memory);
    double expected_time;
    if ((llim_in + rlim_in) / 2 * 1000 > (llim_out + rlim_out) / 2 * repeats * 1000)
    {
        expected_time = (llim_in + rlim_in) / 2 * 1000;
    }
    else
    {
        expected_time = calls * (llim_out + rlim_out) / 2;
    }
    double delta = fabs(service_time - expected_time) / expected_time * 100;

    printf(
           "Рабочее время автомата: %lf (ожидаемое рабочее время: %lf, погрешность: %lf%%)\n"
           "Число вошедших заявок: %d\n"
           "Число вышедших заявок: %d\n"
           "Число необработанных заявок: %d\n"
           "Число срабатываний автомата: %d\n"
           "Время простоя автомата: %lf\n"
           "Количество адресов, взятых из использованной памяти: %d\n"
           "Количество адресов, взятых из новой памяти: %d\n" ,
           service_time, expected_time, delta, in_tasks, out_tasks, failed_tasks,
           calls, hold_time, reusedmem, newmem);
}

void array_model(queuearr_t *const queue,
                 const int llim_in, const int rlim_in,
                 const int llim_out, const int rlim_out,
                 const int repeats)
{
    int in_tasks = 0;
    int out_tasks = 0;
    int failed_tasks = 0;
    int calls = 0;
    double service_time = 0;
    double hold_time = 0;
    //size_t max_memory = 0;
    //size_t m = 0;

    double time_in = get_time_my(llim_in, rlim_in);
    double time_out = -1;

    task_t task_todo;

    int overall_len = 0;
    //uint64_t vrem_1 = tick();
    while (out_tasks != 1000)
    {
        if (time_out < 0 || time_in < time_out)
        {
            if (time_out < 0)
            {
                hold_time += time_in;
            }

            service_time += time_in;
            time_out -= time_in;
            time_in = get_time_my(llim_in, rlim_in);

            task_t task = {.num = 0,
                           .time_out = get_time_my(llim_out, rlim_out)};

            enqueuearr(queue, task);

            if (errno == EQUEUEOVERFLOW)
            {
                printf("Очередь переполнена! Новый элемент не будет добавлен!\n");
                failed_tasks++;
            }
            else
            {
                in_tasks++;
            }

            if (time_out < 0 && queue->size)
            {
                task_todo = dequeuearr(queue);
                time_out = task_todo.time_out;
            }
        }
        else
        {
            time_in -= time_out;
            service_time += time_out;
            time_out = 0;

            task_todo.num++;
            calls++;

            if (task_todo.num < repeats)
            {
                task_todo.time_out = get_time_my(llim_out, rlim_out);
                enqueuearr(queue, task_todo);

                if (errno == EQUEUEOVERFLOW)
                {
                    printf("Очередь переполнена! Новый элемент не будет добавлен!\n");
                    failed_tasks++;
                }
            }
            else
            {
                overall_len += queue->size;
                out_tasks++;
                if (out_tasks % 100 == 0)
                {
                    printf(
                           "\n---\n"
                           "Обработано %d заявок.\n"
                           "Длина очереди: %d\n"
                           "Средняя длина очереди: %d\n"
                           "---\n" ,
                           out_tasks, queue->size, overall_len / out_tasks);
                }
            }

            if (is_emptyarr(queue))
            {
                time_out = -1;
            }
            else
            {
                task_todo = dequeuearr(queue);
                time_out = task_todo.time_out;
            }
        }
        //m = queue->size*sizeof(task_t) + sizeof(unsigned int)*4;
        //if (m > max_memory)
        //    max_memory = m;
        //printf("ПАМЯТЬ ПАМЯТЬ!!!!! %lu\n", sizeof(task_t)*1000 + sizeof(unsigned int)*4);
    }
    //uint64_t vrem_2 = tick();
    //printf("ВРЕМЯ ВЫПОЛНЕНИЯ НА МАССИВЕ %llu\n\n", vrem_2 - vrem_1);
    //max_memory = sizeof(queue);
    //printf("DHDHJAHJDJHAH ПАМЯТЬ ПАМЯТЬ ПАМЯТЬ МАКСИМАЛЬНАЯ %lu", max_memory);
    double expected_time;
    if ((llim_in + rlim_in) / 2 * 1000 > (llim_out + rlim_out) / 2 * repeats * 1000)
    {
        expected_time = (llim_in + rlim_in) / 2 * 1000;
    }
    else
    {
        expected_time = calls * (llim_out + rlim_out) / 2;
    }
    double delta = fabs(service_time - expected_time) / expected_time * 100;

    printf(
           "Рабочее время автомата: %lf (ожидаемое рабочее время: %lf, погрешность: %lf%%)\n"
           "Число вошедших заявок: %d\n"
           "Число вышедших заявок: %d\n"
           "Число необработанных заявок: %d\n"
           "Число срабатываний автомата: %d\n"
           "Время простоя автомата: %lf\n" ,
           service_time, expected_time, delta, in_tasks, out_tasks,
           failed_tasks, calls, hold_time);
}


int main()
{
    int capacity;
    int ltime_in, rtime_in;
    int ltime_out, rtime_out;
    int repeats;
    arr_t *fmem = NULL;
    queuelist_t *queuelist = NULL;
    queuearr_t *queuearr = NULL;

    int inputerr[] = {EINVALIDINTINPUT, EINVALIDINTER};

    int cmd = -1;

    welcome();

    while (cmd)
    {
        input_interval(&cmd, 0, 4);
        errcheck(inputerr, 2,
                 queuearr, queuelist, fmem,
                  "Введено недопустимое значение! Повторите попытку." );

        if (cmd == 1)
        {
            if (queuearr)
            {
                printf( "%s\n" ,
                       "Очередь уже существует! Выход из программы..");

                freequeuearr(queuearr);
                freequeuelist(queuelist, fmem);
                free_array(fmem);

                return EOK;
            }
            else
            {
                printf("%s\n", "Введите максимальное число элементов в очереди:");
                input_interval(&capacity, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );

                printf("%s\n", "Введите интервал прибытия заявки в очередь (2 целых числа через пробел):");
                input_interval(&ltime_in, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );
                input_interval(&rtime_in, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );

                if (rtime_in < ltime_in)
                {
                    printf( "%s\n" ,
                           "Правая граница должна быть больше левой! Повторите попытку.");

                    return EOK;
                }

                printf("%s\n", "Введите интервал обработки заявки в очереди (2 целых числа через пробел):");
                input_interval(&ltime_out, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );
                input_interval(&rtime_out, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );

                if (rtime_out < ltime_out)
                {
                    printf( "%s\n" ,
                           "Правая граница должна быть больше левой! Повторите попытку.");

                    return EOK;
                }

                printf("%s\n", "Введите количество обслуживаний одной заявки:");
                input_interval(&repeats, 1, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );

                queuearr = create_queuearr(capacity);
                uint64_t start, end;
                start = tick();
                array_model(queuearr, ltime_in, rtime_in, ltime_out, rtime_out, repeats);
                end = tick();
                printf( "%s %ju\n" ,
                       "Время выполнения моделирования на основе массива:", end - start);
            }

            welcome();
        }

        if (cmd == 2)
        {
            queuearr_t *test = create_queuearr(1000);
            uint64_t start, end;

            start = tick();
            for (int i = 0; i < 1000; ++i)
            {
                task_t task = {.num = i, .time_out = (double)i};
                enqueuearr(test, task);
            }
            end = tick();

            printf( "%s %ju\n" ,
                   "Добавление элементов в очередь на основе массива:",
                   (end - start) / 1000);

            start = tick();
            for (int i = 0; i < 1000; ++i)
            {
                dequeuearr(test);
            }
            end = tick();

            printf( "%s %ju\n" ,
                   "Удаление элементов из очереди на основе массива:",
                   (end - start) / 1000);

            welcome();
        }

        if (cmd == 3)
        {
            if (queuelist)
            {
                printf( "%s\n" ,
                       "Очередь уже существует! Выход из программы..");

                freequeuearr(queuearr);
                freequeuelist(queuelist, fmem);
                free_array(fmem);

                return EOK;
            }
            else
            {
                printf("%s\n", "Введите максимальное число элементов в очереди:");
                input_interval(&capacity, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );

                printf("%s\n", "Введите интервал прибытия заявки в очередь (2 целых числа через пробел):");
                input_interval(&ltime_in, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );
                input_interval(&rtime_in, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );

                if (rtime_in < ltime_in)
                {
                    printf( "%s\n" ,
                           "Правая граница должна быть больше левой! Повторите попытку.");

                    return EOK;
                }

                printf("%s\n", "Введите интервал обработки заявки в очереди (2 целых числа через пробел):");
                input_interval(&ltime_out, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );
                input_interval(&rtime_out, 0, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );

                if (rtime_out < ltime_out)
                {
                    printf( "%s\n" ,
                           "Правая граница должна быть больше левой! Повторите попытку.");

                    return EOK;
                }

                printf("%s\n", "Введите количество обслуживаний одной заявки:");
                input_interval(&repeats, 1, 99999);
                errcheck(inputerr, 2,
                         queuearr, queuelist, fmem,
                          "Введено недопустимое значение! Повторите попытку." );

                queuelist = create_queuelist(capacity);
                fmem = create_array(capacity);
                uint64_t start, end;
                start = tick();
                list_model(queuelist, fmem, ltime_in, rtime_in, ltime_out, rtime_out, repeats);
                end = tick();
                printf( "%s %ju\n" ,
                       "Время выполнения моделирования на основе списка:", end - start);
            }

            welcome();
        }

        // if (cmd == 4)
        // {
        //     printf( "%s\n" ,
        //            "Массив свободных областей:");
        //     if (fmem != NULL)
        //     {
        //         output_array(*fmem);
        //     }

        //     welcome();
        // }

        if (cmd == 4)
        {
            queuelist_t *test = create_queuelist(1000);
            arr_t *fmem_test = create_array(1000);
            uint64_t start, end;

            start = tick();
            for (int i = 0; i < 1000; ++i)
            {
                task_t task = {.num = i, .time_out = (double)i};
                enqueuelist(test, task);
            }
            end = tick();

            printf( "%s %ju\n" ,
                   "Добавление элементов в очередь на основе списка:",
                   (end - start) / 1000);

            start = tick();
            for (int i = 0; i < 1000; ++i)
            {
                dequeuelist(test, fmem_test);
            }
            end = tick();

            printf( "%s %ju\n" ,
                   "Удаление элементов из очереди на основе списка:",
                   (end - start) / 1000);

            welcome();
        }
    }

    freequeuearr(queuearr);
    freequeuelist(queuelist, fmem);
    free_array(fmem);

    return EOK;
}
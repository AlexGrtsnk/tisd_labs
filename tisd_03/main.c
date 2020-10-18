#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>


/*
Return codes and macroreplacements.
*/

/*
Global return codes.
*/
#define OK 0

/*
ds_matrix module's return codes.
*/
#define MEMORY_ALLOC_ERROR 1
#define MATRIX_INPUT_ERROR 2

/*
func_io module's return codes.
*/
#define INVALID_INTERVAL_ERROR 11
#define INVALID_INT_INPUT_ERROR 12

/*
Macroreplacements.
*/
#define GOT_ARG 1
#define FALSE 0
#define TRUE 1
#define A_LESS 1
#define B_LESS 2
#define EQUAL 3
#define UB 4

/*
Colored printf ouptut.
*/


#define type_spec "%d"
#define out_spec "%5d "

typedef struct
{
    int **matrix;
    int rows;
    int columns;
} matrix_t;

typedef struct
{
    int *elems;
    int *row_entry;
    int *col_entry;
    int elems_amount;
    int cols_amount;
} sparse_t;




/*
Processor's tick counter.

Output data:
* ticks - processor's ticks until return statement.
*/
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

/*
Classic matrix addition.

Input data:
* const matrix_t matrix_a, const matrix_t matrix_b - matrices to be added.
* matrix_t *const matrix_res - result matrix.
*/
void classic_sum(const matrix_t matrix_a, const matrix_t matrix_b,
                 matrix_t *const matrix_res, uint64_t *const ticks)
{
    uint64_t start = tick();
    for (int row = 0; row < matrix_a.rows; ++row)
    {
        for (int col = 0; col < matrix_a.columns; ++col)
        {
            *(*(matrix_res->matrix + row) + col) =
                *(*(matrix_a.matrix + row) + col) + *(*(matrix_b.matrix + row) + col);
        }
    }
    uint64_t end = tick();

    *ticks = end - start;
}

/*
Count non-zero dots after sparse addition.

Input data:
* const sparse_t sparse_a, const sparse_t sparse_b - sparse matrices to be 
checked.
* int *const dots - overall dots.
*/
int sdots(const sparse_t sparse_a, const sparse_t sparse_b, int *const dots)
{
    for (int col = 0; col < sparse_a.cols_amount; ++col)
    {
        int *a_arr, *b_arr;
        int a_ind = 0, b_ind = 0;
        int cur_a, cur_b;

        if (col + 1 != sparse_a.cols_amount)
        {
            cur_a = *(sparse_a.col_entry + col + 1);
            cur_b = *(sparse_b.col_entry + col + 1);
        }
        else
        {
            cur_a = sparse_a.elems_amount;
            cur_b = sparse_b.elems_amount;
        }

        int a_column = cur_a - *(sparse_a.col_entry + col);
        int b_column = cur_b - *(sparse_b.col_entry + col);
        a_arr = (int *)calloc(a_column, sizeof(int));
        b_arr = (int *)calloc(b_column, sizeof(int));

        for (int a_col = *(sparse_a.col_entry + col); a_col < cur_a; ++a_col)
        {
            if (a_arr)
            {
                *(a_arr + a_ind) = *(sparse_a.row_entry + a_col);
            }
            a_ind++;
        }

        for (int b_col = *(sparse_b.col_entry + col); b_col < cur_b; ++b_col)
        {
            if (b_arr)
            {
                *(b_arr + b_ind) = *(sparse_b.row_entry + b_col);
            }
            b_ind++;
        }

        int matches = 0;

        for (int i = 0; i < a_column; ++i)
        {
            for (int j = i; j < b_column; ++j)
            {
                if (*(a_arr + i) == *(b_arr + j))
                {
                    matches++;
                }
            }
        }

        *dots += a_column + b_column - matches;

        free(a_arr);
        free(b_arr);
    }

    return OK;
}

/*
Find minimum of two arrays.

Input data:
* int *a_arr, const int a_len, int *b_arr, const int b_len - arrays and 
their properties.

Output data:
* Return sign: A_LESS (if a_arr less than b_arr), B_LESS (if b_arr less than 
a_arr) or EQUAL (otherwise).
*/
int arrays_min(int *a_arr, const int a_len, int *b_arr, const int b_len)
{
    if (a_len == 0 && b_len != 0)
    {
        return B_LESS;
    }

    if (b_len == 0 && a_len != 0)
    {
        return A_LESS;
    }

    int a_min = *a_arr, b_min = *b_arr;
    int a_pos = 0, b_pos = 0;

    for (int a = 0; a < a_len; ++a)
    {
        if (*(a_arr + a) < a_min)
        {
            a_min = *(a_arr + a);
            a_pos = a;
        }
    }

    for (int b = 0; b < b_len; ++b)
    {
        if (*(b_arr + b) < b_min)
        {
            b_min = *(b_arr + b);
            b_pos = b;
        }
    }

    if (a_min < b_min)
    {
        *(a_arr + a_pos) = 1000;

        return A_LESS;
    }

    if (b_min < a_min)
    {
        *(b_arr + b_pos) = 1000;

        return B_LESS;
    }

    if (a_min == b_min && a_min == 1000 && b_min == 1000)
    {
        return UB;
    }

    *(a_arr + a_pos) = 1000;
    *(b_arr + b_pos) = 1000;

    return EQUAL;
}

/*
Sparse matrix addition.

Input data:
* const sparse_t sparse_a, const sparse_t sparse_b - matrices to be added.
* sparse_t *const sparse_res - result matrix.
*/
void sparse_sum(const sparse_t sparse_a, const sparse_t sparse_b,
                sparse_t *const sparse_res, uint64_t *const ticks)
{
    int cur_el = 0;
    int a_glob = 0;
    int b_glob = 0;

    for (int col = 0; col < sparse_a.cols_amount; ++col)
    {
        int *a_arr, *b_arr;
        int a_ind = 0, b_ind = 0;
        int cur_a, cur_b;

        if (col + 1 != sparse_a.cols_amount)
        {
            cur_a = *(sparse_a.col_entry + col + 1);
            cur_b = *(sparse_b.col_entry + col + 1);
        }
        else
        {
            cur_a = sparse_a.elems_amount;
            cur_b = sparse_b.elems_amount;
        }

        int a_column = cur_a - *(sparse_a.col_entry + col);
        int b_column = cur_b - *(sparse_b.col_entry + col);
        a_arr = (int *)calloc(a_column, sizeof(int));
        b_arr = (int *)calloc(b_column, sizeof(int));

        for (int a_col = *(sparse_a.col_entry + col); a_col < cur_a; ++a_col)
        {
            if (a_arr)
            {
                *(a_arr + a_ind) = *(sparse_a.row_entry + a_col);
            }
            a_ind++;
        }

        for (int b_col = *(sparse_b.col_entry + col); b_col < cur_b; ++b_col)
        {
            if (b_arr)
            {
                *(b_arr + b_ind) = *(sparse_b.row_entry + b_col);
            }
            b_ind++;
        }

        int matches = 0;

        for (int i = 0; i < a_column; ++i)
        {
            for (int j = i; j < b_column; ++j)
            {
                if (*(a_arr + i) == *(b_arr + j))
                {
                    matches++;
                }
            }
        }

        int range = a_column + b_column - matches;

        uint64_t start = tick();
        for (int i = 0; i < range; ++i)
        {
            int where = arrays_min(a_arr, a_column, b_arr, b_column);

            if (where == A_LESS)
            {
                *(sparse_res->elems + cur_el) = *(sparse_a.elems + a_glob);
                *(sparse_res->row_entry + cur_el) = *(sparse_a.row_entry + a_glob);
                cur_el++;
                a_glob++;
            }

            if (where == B_LESS)
            {
                *(sparse_res->elems + cur_el) = *(sparse_b.elems + b_glob);
                *(sparse_res->row_entry + cur_el) = *(sparse_b.row_entry + b_glob);
                cur_el++;
                b_glob++;
            }

            if (where == EQUAL)
            {
                *(sparse_res->elems + cur_el) = *(sparse_a.elems + a_glob) + *(sparse_b.elems + b_glob);
                *(sparse_res->row_entry + cur_el) = *(sparse_a.row_entry + a_glob);
                cur_el++;
                a_glob++;
                b_glob++;
            }
        }

        uint64_t end = tick();
        *ticks += end - start;

        free(a_arr);
        free(b_arr);
    }
}

int fill_col_entry(sparse_t *const sparse, const matrix_t matrix)
{
    int elems = 0;
    for (int col = 0; col < matrix.columns; ++col)
    {
        int col_elems = 0;
        for (int row = 0; row < matrix.rows; ++row)
        {
            if (*(*(matrix.matrix + row) + col) != 0)
            {
                col_elems++;
                elems++;
            }
        }

        *(sparse->col_entry + col) = elems - col_elems;
    }

    return OK;
}

int randommy(const int offset)
{
    int doom_seed[] = {
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30,
        69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94,
        252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 8, 237, 149, 56, 87, 174, 20, 125, 136, 171,
        168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122,
        60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
        1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159,
        86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147,
        118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183,
        170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129,
        22, 39, 253, 9, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
        251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
        49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

    time_t now = time(0) + offset + doom_seed[offset % 256];
    int seed = now % 256;

    return abs(doom_seed[seed] + doom_seed[(seed + 1) % 256] - doom_seed[(seed + 2) % 256]);
}

/*
Get amount of empty tiles in matrix.

Input data:
* const matrix_t matrix - matrix to be checeked.

Output data:
* Amount of empty tiles.
*/
int get_empty_tiles(const matrix_t matrix)
{
    int empty_tiles = 0;

    for (int row = 0; row < matrix.rows; ++row)
    {
        for (int col = 0; col < matrix.columns; ++col)
        {
            if (*(*(matrix.matrix + row) + col) == 0)
            {
                empty_tiles++;
            }
        }
    }

    return empty_tiles;
}

/*
Fill matrix matrix with 4 (10% chance) or 2 (90% chance).

Input data:
* matrix_t *const matrix - matrix to be filled.
*/
void gorandom(matrix_t *const matrix, int percent)
{
    int empty_tiles = get_empty_tiles(*matrix);

    int rand = randommy(empty_tiles) % empty_tiles;
    int tile = randommy(empty_tiles);

    for (int row = 0; row < matrix->rows; ++row)
    {
        for (int col = 0; col < matrix->columns; ++col)
        {
            if (!(*(*(matrix->matrix + row) + col)) && rand)
            {
                --rand;
            }
            else
            {
                if (!(*(*(matrix->matrix + row) + col)) && !rand)
                {
                    *(*(matrix->matrix + row) + col) = tile;

                    return;
                }
            }
        }
    }
}

/*
IO module.
*/


/*
Welcomming print with available features.
*/
void welcome()
{
    printf("Данная программа позволяет сравнить время, затраченное на сложение \n"
           "двух разреженных матриц, хранимых в стандартном виде и в разреженном \n"
           "столбцовом видах.\n\n" );
    printf("Выберите действие из списка, введя соответствуюший номер:\n" );
    printf("1. Ввести матрицы вручную.\n"
           "2. Сгенерировать матрицы случайно (по проценту заполненности).\n"
           "3. Сложить матрицы, используя стандартный способ хранения.\n"
           "4. Сложить матрицы, используя разреженный столбцовый вид хранения.\n"
           "5. Вывести исходные и результирующую матрицы в стандартном виде.\n"
           "6. Вывести исходные и результирующую матрицы в разреженном столбцовом виде.\n"
           "0. Выход из программы.\n\n" );
}

/*
Clean input stream from trash.
*/
void clean_input_stream()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

/*
Input number in between.

Input data:
* int *const num - number of action to be done.
* const int left - left border of between.
* const int right - right border of between.

Output data:
* Return code - OK, INVALID_INT_INPUT_ERROR or IN_BETWEEN_ERROR.
*/
int input_interval(int *const num, const int left, const int right)
{
    if (scanf(type_spec, num) != GOT_ARG)
    {
        clean_input_stream();
        return INVALID_INT_INPUT_ERROR;
    }

    if (*num < left || *num > right)
    {
        clean_input_stream();
        return INVALID_INTERVAL_ERROR;
    }

    return OK;
}


/*
CDIO utilities for sparse matrix data structure.
*/


/*
Fill col_entry array with -1 value.

Input data:
* sparse_t *const sparse - sparse matrix to be changed.
*/
void fill_sparse(sparse_t *const sparse)
{
    for (int ent = 0; ent < sparse->elems_amount; ++ent)
    {
        *(sparse->elems + ent) = 0;
    }
    for (int ent = 0; ent < sparse->elems_amount; ++ent)
    {
        *(sparse->row_entry + ent) = 0;
    }
    for (int ent = 0; ent < sparse->cols_amount; ++ent)
    {
        *(sparse->col_entry + ent) = -1;
    }
}

/*
Sparse matrix creation.

Input data:
* sparse_t *const sparse - sparse matrix to be created.
* const int dots - amount non-zero values in matrix.
* const int cols - amount of base matrix columns.

Output data:
* Return code - OK or MEMORY_ALLOC_ERROR.
*/
int screate(sparse_t *const sparse, const int dots, const int cols)
{
    sparse->elems = (int *)malloc(dots * sizeof(int));
    sparse->row_entry = (int *)malloc(dots * sizeof(int));
    sparse->col_entry = (int *)malloc(cols * sizeof(int));
    sparse->elems_amount = dots;
    sparse->cols_amount = cols;

    if (!sparse->elems || !sparse->row_entry || !sparse->col_entry)
    {
        return MEMORY_ALLOC_ERROR;
    }

    fill_sparse(sparse);

    return OK;
}

/*
Sparse matrix deletion.

Input data:
* sparse_t *const sparse - matrix to be deleted.

Output data:
* Return code - OK or MEMORY_ALLOC_ERROR.
*/
int sdelete(sparse_t *const sparse)
{
    if (!sparse->elems || !sparse->row_entry || !sparse->col_entry)
    {
        return MEMORY_ALLOC_ERROR;
    }

    free(sparse->elems);
    free(sparse->row_entry);
    free(sparse->col_entry);

    return OK;
}

/*
Sparse matrix input.

Input data:
* sparse_t *const sparse - sparse matrix to be inputted.
* matrix_t *const matrix - base matrix.

Output data:
* Return code - OK, MEMORY_ALLOC_ERROR or MATRIX_INPUT_ERROR.
*/
int sinput(sparse_t *const sparse, const matrix_t matrix)
{
    if (!matrix.matrix || !sparse->elems || !sparse->row_entry || !sparse->col_entry)
    {
        return MEMORY_ALLOC_ERROR;
    }

    int ind = -1;
    int cur = 0;

    for (int col = 0; col < matrix.columns; ++col)
    {
        for (int row = 0; row < matrix.rows; ++row)
        {
            if (*(*(matrix.matrix + row) + col) != 0)
            {
                ind++;
                if (*(sparse->col_entry + col) == -1)
                {
                    *(sparse->col_entry + col) = ind;
                }
            }
        }
    }

    for (int col = 0; col < matrix.columns; ++col)
    {
        for (int row = 0; row < matrix.rows; ++row)
        {
            if (*(*(matrix.matrix + row) + col) != 0)
            {
                *(sparse->elems + cur) = *(*(matrix.matrix + row) + col);
                *(sparse->row_entry + cur) = row;
                cur++;
            }
        }
    }

    for (int ent = 0; ent < sparse->cols_amount; ++ent)
    {
        if (*(sparse->col_entry + ent) == -1)
        {
            for (int search = ent; search < sparse->cols_amount; ++search)
            {
                if (*(sparse->col_entry + search) != -1)
                {
                    *(sparse->col_entry + ent) = *(sparse->col_entry + search);
                    break;
                }

                *(sparse->col_entry + search) = sparse->elems_amount;
            }
        }
    }

    return OK;
}

/*
Sparse matrix output.

Input data:
* const sparse_t sparse - sparse matrix to be outputed.

Output data:
* Return code - OK or MEMORY_ALLOC_ERROR.
*/
int soutput(const sparse_t sparse)
{
    if (!sparse.elems || !sparse.row_entry || !sparse.col_entry)
    {
        return MEMORY_ALLOC_ERROR;
    }

    for (int el = 0; el < sparse.elems_amount; ++el)
    {
        printf(out_spec, *(sparse.elems + el));
    }
    printf( "\n");

    for (int el = 0; el < sparse.elems_amount; ++el)
    {
        printf("%5d ", *(sparse.row_entry + el));
    }
    printf( "\n");

    for (int el = 0; el < sparse.cols_amount; ++el)
    {
        printf("%5d ", *(sparse.col_entry + el));
    }
    printf( "\n");

    return OK;
}

/*
CDIO utilities for matrix data structure.
*/


void fill_matrix(matrix_t *const matrix)
{
    for (int row = 0; row < matrix->rows; ++row)
    {
        for (int col = 0; col < matrix->columns; ++col)
        {
            *(*(matrix->matrix + row) + col) = 0;
        }
    }
}

/*
Dynamic matrix creation.

Input data:
* matrix_t *const matrix - matrix to be created.

Output data:
* Return code - OK or MEMORY_ALLOC_ERROR.
*/
int create(matrix_t *const matrix)
{
    matrix->matrix = (int **)malloc(matrix->rows * sizeof(int *));

    if (!matrix->matrix)
    {
        return MEMORY_ALLOC_ERROR;
    }

    for (int row = 0; row < matrix->rows; ++row)
    {
        *(matrix->matrix + row) = (int *)malloc(matrix->columns * sizeof(int));

        if (!*(matrix->matrix + row))
        {
            for (int clean = 0; clean < row; ++clean)
            {
                free(*(matrix->matrix + clean));
            }

            free(matrix->matrix);

            return MEMORY_ALLOC_ERROR;
        }
    }

    fill_matrix(matrix);

    return OK;
}

/*
Dynamic matrix deletion.

Input data:
* matrix_t *const matrix - mattrix to be deleted.

Output data:
* Return code - OK or MEMORY_ALLOC_ERROR.
*/
int deletee(matrix_t *const matrix)
{
    if (!matrix->matrix)
    {
        return MEMORY_ALLOC_ERROR;
    }

    for (int clean = 0; clean < matrix->rows; ++clean)
    {
        if (*(matrix->matrix + clean))
        {
            free(*(matrix->matrix + clean));
        }
    }

    free(matrix->matrix);

    return OK;
}

/*
Dynamic matrix input.

Input data:
* matrix_t *const matrix - matrix to be inputed.
* const int dots - amount of dots to be entered.

Output data:
* Return code - OK, MEMORY_ALLOC_ERROR or MATRIX_INPUT_ERROR.
*/
int input(matrix_t *const matrix, const int dots)
{
    if (!matrix->matrix)
    {
        return MEMORY_ALLOC_ERROR;
    }

    for (int dot = 0; dot < dots; ++dot)
    {
        int row, col, cur;

        if (input_interval(&row, 0, matrix->rows - 1))
        {
            return INVALID_INT_INPUT_ERROR;
        }

        if (input_interval(&col, 0, matrix->columns - 1))
        {
            return INVALID_INT_INPUT_ERROR;
        }

        if (input_interval(&cur, -1000, 99999))
        {
            return INVALID_INT_INPUT_ERROR;
        }

        *(*(matrix->matrix + row) + col) = cur;
    }

    return OK;
}

/*
Dynamic matrix output.

Input data:
* const matrix_t *const matrix - matrix to be outputed.

Output data:
* Return code - OK or MEMORY_ALLOC_ERROR.
*/
int output(const matrix_t *const matrix)
{
    if (!matrix->matrix)
    {
        return MEMORY_ALLOC_ERROR;
    }

    for (int row = 0; row < matrix->rows; ++row)
    {
        for (int col = 0; col < matrix->columns; ++col)
        {
            printf(out_spec, *(*(matrix->matrix + row) + col));
        }

        printf( "\n");
    }

    return OK;
}

int main()
{
    //system("clear");
    welcome();

    matrix_t matrix_a, matrix_b, matrix_res;
    sparse_t sparse_a, sparse_b, sparse_res;
    int dots_a, dots_b, dots_sum = 0;
    int percent_a, percent_b;

    int cur_command = -1;

    while (cur_command != 0)
    {
        if (input_interval(&cur_command, 0, 6))
        {
            printf(
                   "%s\n" ,
                   "Введена недопустимая команда! Повторите попытку.");

            welcome();
        }

        else
        {
            if (cur_command == 1)
            {
                printf("%s\n", "Введите количество строк матрицы А (от 1 до 1000):");
                if (input_interval(&matrix_a.rows, 1, 1000))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    return OK;
                }

                printf("%s\n", "Введите количество столбцов матрицы А (от 1 до 1000):");
                if (input_interval(&matrix_a.columns, 1, 1000))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    return OK;
                }

                printf("%s\n", "Введите количество ненулевых элементов матрицы А (от 1 до 1000):");
                if (input_interval(&dots_a, 0, matrix_a.rows * matrix_a.columns))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    welcome();
                }

                printf("%s\n", "Введите элементы матрицы А в формате 'строка столбец значение':");
                create(&matrix_a);
                if (input(&matrix_a, dots_a))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    return OK;
                }

                matrix_b.rows = matrix_a.rows;
                matrix_b.columns = matrix_a.columns;

                printf("%s\n", "Введите количество ненулевых элементов матрицы В (от 1 до 1000):");
                if (input_interval(&dots_b, 0, matrix_b.rows * matrix_b.columns))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    welcome();
                }

                printf("%s\n", "Введите элементы матрицы В в формате 'строка столбец значение':");
                create(&matrix_b);
                if (input(&matrix_b, dots_b))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    return OK;
                }

                matrix_res.rows = matrix_a.rows;
                matrix_res.columns = matrix_a.columns;

                create(&matrix_res);

                screate(&sparse_a, dots_a, matrix_a.columns);
                sinput(&sparse_a, matrix_a);
                fill_col_entry(&sparse_a, matrix_a);

                screate(&sparse_b, dots_b, matrix_a.columns);
                sinput(&sparse_b, matrix_b);
                fill_col_entry(&sparse_b, matrix_b);

                sdots(sparse_a, sparse_b, &dots_sum);
                screate(&sparse_res, dots_sum, matrix_a.columns);

                welcome();
            }

            if (cur_command == 2)
            {
                printf("%s\n", "Введите количество строк матрицы А (от 1 до 1000):");
                if (input_interval(&matrix_a.rows, 1, 1000))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    return OK;
                }

                printf("%s\n", "Введите количество столбцов матрицы А (от 1 до 1000):");
                if (input_interval(&matrix_a.columns, 1, 1000))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    return OK;
                }

                printf("%s\n", "Введите процентное значение ненулевых элементов матрицы А (от 0 до 100):");
                if (input_interval(&percent_a, 0, 100))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    return OK;
                }

                printf("%s\n", "Введите процентное значение ненулевых элементов матрицы В (от 0 до 100):");
                if (input_interval(&percent_b, 0, 100))
                {
                    printf(
                           "%s\n" ,
                           "Введено недопустимое значение! Повторите попытку.");

                    return OK;
                }

                matrix_b.rows = matrix_a.rows;
                matrix_b.columns = matrix_a.columns;

                create(&matrix_a);
                create(&matrix_b);
                int to_fill_a = floor(matrix_a.rows * matrix_a.columns * percent_a / 100.0);
                int a_create = to_fill_a;
                while (to_fill_a)
                {
                    gorandom(&matrix_a, percent_a);
                    to_fill_a--;
                }
                int to_fill_b = floor(matrix_a.rows * matrix_a.columns * percent_b / 100.0);
                int b_create = to_fill_b;
                while (to_fill_b)
                {
                    gorandom(&matrix_b, percent_b);
                    to_fill_b--;
                }

                matrix_res.rows = matrix_a.rows;
                matrix_res.columns = matrix_a.columns;

                create(&matrix_res);

                screate(&sparse_a, a_create, matrix_a.columns);
                sinput(&sparse_a, matrix_a);
                fill_col_entry(&sparse_a, matrix_a);

                screate(&sparse_b, b_create, matrix_a.columns);
                sinput(&sparse_b, matrix_b);
                fill_col_entry(&sparse_b, matrix_b);

                sdots(sparse_a, sparse_b, &dots_sum);
                screate(&sparse_res, dots_sum, matrix_a.columns);

                welcome();
            }

            if (cur_command == 3)
            {
                uint64_t ticks = 0;
                classic_sum(matrix_a, matrix_b, &matrix_res, &ticks);

                printf(
                       "%s%ju\n" ,
                       "Время сложения матриц, хранимых стандартным методом = ",
                       ticks);

                welcome();
            }

            if (cur_command == 4)
            {
                uint64_t ticks = 0;
                classic_sum(matrix_a, matrix_b, &matrix_res, &ticks);
                ticks = 0;
                fill_col_entry(&sparse_a, matrix_a);
                fill_col_entry(&sparse_b, matrix_b);
                sparse_sum(sparse_a, sparse_b, &sparse_res, &ticks);
                fill_col_entry(&sparse_res, matrix_res);

                printf(
                       "%s%ju\n" ,
                       "Время сложения матриц, хранимых разреженным "
                       "столбцовым методом = ",
                       ticks);

                int mat_size = sizeof(int **) + 
                               sizeof(int *) * matrix_res.rows + 
                               sizeof(int) * matrix_res.rows * matrix_res.columns + 
                               2 * sizeof(int);
                int sparse_size = sizeof(int *) * 3 +
                                  sizeof(int) * sparse_res.elems_amount * 2 + 
                                  sizeof(int) * sparse_res.cols_amount +
                                  2 * sizeof(int);

                printf("~%d~%d\n", sparse_size, mat_size);

                welcome();
            }

            if (cur_command == 5)
            {
                printf(
                       "%s\n" ,
                       "Матрица А:");
                output(&matrix_a);

                printf(
                       "%s\n" ,
                       "Матрица В:");
                output(&matrix_b);

                printf(
                       "%s\n" ,
                       "Матрица RES:");
                output(&matrix_res);

                welcome();
            }

            if (cur_command == 6)
            {
                printf(
                       "%s\n" ,
                       "Матрица А:");
                soutput(sparse_a);

                printf(
                       "%s\n" ,
                       "Матрица В:");
                soutput(sparse_b);

                printf(
                       "%s\n" ,
                       "Матрица RES:");
                soutput(sparse_res);

                welcome();
            }
        }
    }

    deletee(&matrix_a);
    sdelete(&sparse_a);
    deletee(&matrix_b);
    sdelete(&sparse_b);
    deletee(&matrix_res);
    sdelete(&sparse_res);

    return OK;
}
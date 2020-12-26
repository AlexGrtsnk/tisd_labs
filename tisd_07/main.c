#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

typedef struct graph
{
    int size;
    int **matrix;
    char **paths;
} graph_t;

#define OK 0
#define FILE_ERROR 2
#define GRAPH_ERROR 4
#define N 10000
#define N1 100
#define READ_OK 1
#define OK 0
#define INVALID_INPUT 1



 

void create_string_path(graph_t graph, char *graph_visual)
{
    char temp[N1];
    sprintf(graph_visual, "%s", "digraph g{\n");

    for (int i = 0; i < graph.size; i++)
    {
        for (int j = 0; j < graph.size; j++)
        {
            if (graph.matrix[i][j] != 0)
            {
                sprintf(temp, "\t%d -> %d [label=%d]\n", i + 1, j + 1, graph.matrix[i][j]);
                strcat(graph_visual, temp);
            }
        }
    }

    strcat(graph_visual, "}");
}

static void shift_string(char *path)
{
    char buffer[N1 * N1];
    char entry_string[N1] = "[color=\"red\"]\n";

    strcpy(buffer, path + 1);

    for (int i = 0; entry_string[i] != '\0'; i++)
    {
        *(path++) = entry_string[i];
    }

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        *(path++) = buffer[i];
    }

    path = '\0';
}

static void add_red_color(char *path)
{
    if (path != NULL)
    {
        while (*path++ != '\n');
        shift_string(--path);
    }
}

static void print_path(char *graph_visual, char *path)
{
    while (*path)
    {
        path++;
        int space_count = 0, i = 0;
        char *buffer = malloc(N1 * sizeof(char));

        while (space_count < 3 && *path != '\0')
        {
            buffer[i++] = *(path++);
            if (*(path - 1) == ' ')
            {
                space_count++;
            }
        }

        buffer[i] = '\0';
        if (buffer[0] != '>')
        {
            add_red_color(strstr(graph_visual, buffer));
        }

        space_count = 0;
        while (space_count < 2 && *path != '\0')
        {
            if (*path-- == ' ')
            {
                space_count++;
            }
        }

        path++;
        free(buffer);
    }

}

static void print_solution(char *solution_path)
{
    FILE *f = fopen("graph.dot", "w");
    fprintf(f, "%s", solution_path);
    fclose(f);
    system("dot -Tpng -O graph.dot");
    //FILE *f1 = fopen("graph.dot.png", "r");
    system("open graph.dot.png");
    //fclose("")
    sleep(4);
    system("killall Preview");
    //fclose(f1);
}

void paint_graphs(const graph_t graph, char *graph_visual, char *copy_graph_visual, const int start)
{
    for (int j = 0; j < graph.size; j++)
    {
        strcpy(copy_graph_visual, graph_visual);
        print_path(copy_graph_visual, graph.paths[j]);

        if (graph.paths[j][0] != '\0')
        {
            printf("Путь: %s\n", graph.paths[j]);
            graph.paths[j][0] = '\0';
        }

        if (strcmp(copy_graph_visual, graph_visual))
        {
            print_solution(copy_graph_visual);
            memset(copy_graph_visual, 0, N1 * N1);
        }
    }
}


static int min_distance(int *distance, char *is_visited, const int size)
{
    int min = INT_MAX;
    int min_index;

    for (int i = 0; i < size; i++)
    {
        if (is_visited[i] == 0 && distance[i] <= min)
        {
            min = distance[i];
            min_index = i;
        }
    }

    return min_index;
}

static void reverse(char *string)
{
    int len = strlen(string);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = string[i];
        string[i] = string[len - i - 1];
        string[len - i - 1] = temp;
    }
}

static void save_path(graph_t graph, const int start_vertex, char *path)
{
    for (int i = 0; i < graph.size; i++)
    {
        if (i != start_vertex)
        {
            char buffer[10];
            sprintf(buffer, "%d ", i + 1);
            int j = i;

            do
            {
                strcat(graph.paths[i], buffer);
                j = path[j];
                sprintf(buffer, ">- %d ", j + 1);
            } while(j != start_vertex);

            strcat(graph.paths[i], buffer);
            reverse(graph.paths[i]);
        }
    }

}

int deijkstra(graph_t graph, int start_vertex)
{
    int *distance = malloc(graph.size * sizeof(int));
    char *is_visited = malloc(graph.size * sizeof(char));
    char *path = malloc(graph.size * sizeof(int));

    for (int i = 0; i < graph.size; i++)
    {
        *(distance + i) = INT_MAX;
        *(is_visited + i) = 0;
        *(path + i) = 0;
    }

    *(distance + start_vertex) = 0;

    for (int i = 0; i < graph.size - 1; i++)
    {
        int index_to_min = min_distance(distance, is_visited, graph.size);
        *(is_visited + index_to_min) = 1;

        for (int j = 0; j < graph.size; j++)
        {
            if (!is_visited[j] && graph.matrix[index_to_min][j] && distance[index_to_min] != INT_MAX
                && distance[index_to_min] + graph.matrix[index_to_min][j] < distance[j])
            {
                distance[j] = distance[index_to_min] + graph.matrix[index_to_min][j];
                path[j] = index_to_min;
            }
        }
    }

    save_path(graph, start_vertex, path);
    for (int i = 0; i < graph.size; i++)
    {
        if (distance[i] == INT_MAX)
        {
            fprintf(stderr, "Ошибка, граф не является связанным.\n");
            return GRAPH_ERROR;
        }
    }

    free(distance);
    free(is_visited);
    free(path);

    return OK;
}

graph_t create_graph(int size)
{
    graph_t graph;
    graph.size = size;

    graph.matrix = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
    {
        *(graph.matrix + i) = calloc(size, sizeof(int));
    }

    graph.paths = malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++)
    {
        *(graph.paths + i) = malloc(N1 * sizeof(char));
    }

    return graph;
}


int read_graph_size(int *size)
{
    fprintf(stdout, "%s", "Введите количество городов: ");

    if (fscanf(stdin, "%d", size) != READ_OK)
    {
        return INVALID_INPUT;
    }

    if (*size <= 0)
        return INVALID_INPUT;

    return OK;
}

static int keyboard_read(const graph_t graph, FILE *f)
{
    fprintf(stdout, "%s\n", "Введите пары связанных городов (в виде цифр) и путь между ними.\n"
                            "Введите -1 -1 -1, как признак окончания ввода.");

    int i = 1, j = 1, scale = 0;

    while (i != -1 && j != -1 && scale != -1)
    {
        graph.matrix[i - 1][j - 1] = scale;
        if (graph.matrix[j - 1][i - 1] == 0)
        {
            graph.matrix[j - 1][i - 1] = scale;
        }

        if (fscanf(f, "%d %d %d", &i, &j, &scale) != 3)
        {
            return INVALID_INPUT;
        }

        if (i <= 0 || j <= 0 || scale <= 0)
        {
            return INVALID_INPUT;
        }

        /*
        if (i > graph.size || i < 1 || j > graph.size || j < 1 || scale < 1)
        {
            fprintf(stderr, "%s\n", "Некорректный ввод.");
            return INVALID_INPUT;
        }
        */
    }

    return OK;
}

static int file_read(const graph_t graph, FILE *f)
{
    for (int i = 0; i < graph.size; i++)
    {
        for (int j = 0; j < graph.size; j++)
        {
            if (fscanf(f, "%d", &graph.matrix[i][j]) != READ_OK)
            {
                fprintf(stderr, "%s", "Ошибка чтения из файла\n");
                return INVALID_INPUT;
            }
        }
    }

    return OK;
}

int read_graph(const graph_t graph, FILE *f)
{
    int code_error;

    if (f == stdin)
    {
        if ((code_error = keyboard_read(graph, f)))
        {
            return code_error;
        }
    }
    else
    {
        if ((code_error = file_read(graph, f)))
        {
            return code_error;
        }
    }

    return OK;
}




int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    //FILE *f = stdin;
    char name[256];
    int size, code_error;
    printf("Здравствуйте! Это программа выполняет поиск кратчайших путей для каждой пары городов. Ввод происходит из файла с матрицей смежности, следуйте инструкции\n\n");
    printf("Пожалуйста, введите размер матрицы, содержащейся в файле:\n");
    scanf("%d", &size);
    printf("Пожалуйста, введите имя файла с расшерением:\n");
    scanf("%s", &name);
    if (name[strlen(name) - 1] == '\n')
        name[strlen(name) - 1] = '\0';
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

    /*
    if (argc == 1)
    {
        if ((code_error = read_graph_size(&size)))
        {
            return code_error;
        }
    }
    else
    {
        if ((f = fopen(argv[1], "r")) == NULL)
        {
            fprintf(stderr, "%s\n", "Ошибка при чтении файла.");
            return FILE_ERROR;
        }

        size = atoi(argv[2]);
    }
    */
    graph_t graph = create_graph(size);

    if ((code_error = read_graph(graph, f)))
    {
        return code_error;
    }

    char copy_graph_visual[N];
    char graph_visual[N];
    create_string_path(graph, graph_visual);

    /*
    for (int i = 0; i < graph.size; i++)
    {
        for (int j = 0; j < graph.size; j++)
        {
            printf("%d ", graph.matrix[i][j]);
        }
        putchar('\n');
    }
    */

    for (int i = 0; i < graph.size; i++)
    {
        if (deijkstra(graph, i))
        {
            return GRAPH_ERROR;
        }

        paint_graphs(graph, graph_visual, copy_graph_visual, i);
    }

    return OK;
}

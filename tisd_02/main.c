#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
#define SEP ",.\n"
#define N 100

// Объявление 
struct data
{
	int d;
	int m;
	int y;
};

struct office
{
	char position[20];
	char organization[20];
};

typedef struct
{
	char surname[30];
	char name[20];
	int phone;
	char adres[40];

	enum 
	{
		personal,
		work
	}st;

	union
	{
		struct data dofb;
		struct office ofc;
	} type;

} listp;

typedef struct
{
    int index_src;
    char surname[30];
} key_listp;

listp list[MAX], list_q[MAX], list_m[MAX];
key_listp list_surname[MAX], list_surname_q[MAX], list_surname_m[MAX];



// Функции инизиализации и поиска 1го не свободного элемента, удаления и свободного элемента
void init_list(void)
{
    for(int i = 0; i < MAX; i++)
        list[i].surname[0] = '\0';
}

void init_key_list(void)
{
    for (int i = 0; i < MAX; i++)
        list_surname[i].index_src = -1;
}

int find_not_free_1(void)
{
    register int t;

    for (t = 0; list[t].surname[0] == '\0' && t < MAX; t++);

    if (t == MAX)
        return -1;

    return t;
}

int find_not_free_2(void)
{
    register int t;

    for (t = 0; list_surname[t].index_src == -1 && t < MAX; t++);

    if (t == MAX)
        return -1;

    return t;
}

int find_free(void)
{
    register int t;

    for (t = 0; list[t].surname[0] && t < MAX; t++);

    if (t == MAX)
        return -1; /* no slots free */

    return t;
}

void del(void)
{
    int slot;
    int i = 0, j = 0;

    printf("Введите номер записи для удаления: ");
    if (scanf("%d", &slot) != 1)
    {
        printf("\nНеверные входные данные\n");
        scanf("%*s");
        slot = -1;
    }

    if (slot >= 1 && slot <= MAX)
    {
        for (i = 0; i < MAX; i++)
        {
            if (list[i].surname[0])
                j++;
            if (j == slot)
                break;
        }
        list[i].surname[0] = '\0';
    }
    else
        printf("Такой записи не существует\n");
    
    printf("\n");
}

// Разбиение строки на слова
int count_fields(const char *str)
{
    int len = strlen(str);
    int n = 0;
    
    for (int i = 0; i < len; i++)
       if (!strchr(SEP, str[i]))
           if (i == 0 || strchr(SEP, str[i - 1]))
               n++;
    
    return n;
}
int split_fields(char *str, char **fields)
{
    int len = strlen(str);
    int j = 0;
    
    for (int i = 0; i < len; i++)
    {
        if (strchr(SEP, str[i]))
            str[i] = 0;
        else if (i == 0 || str[i - 1] == 0)
        {
            fields[j] = str + i;
            j++;
        }
    }
    
    return j;
}

// Ввод/вывод файловый
void fscan_list(void)
{
    char name[30];

    printf("Введите название исходного файла, с расширением: ");
    scanf("%s", name);

    FILE *f_inp;

    f_inp = fopen(name, "r");

    if (f_inp == NULL)
    {
        printf("Не удалось открыть файл\n\n");
        return;
    }

    char str[100];
    char *estr;
    int slot = 0;
    int flag = 0;

    while (1)
    {
        estr = fgets(str, sizeof(str), f_inp);
        if (estr == NULL)
        {
            if (feof(f_inp) != 0)
            {
                flag = 1;
                break;
            }
            else
            {
                printf("Ошибка при чтении файла\n");
                break;
            }
        }

        if (slot == MAX)
            break;

        int cn = count_fields(str);

        if (cn)
        {
            char fields[2*MAX];
            char *p = fields;

            if (1)
            {
                split_fields(str, &p);

	            for (int i = 0; i < cn; i++)
	            {
	                if (i == 0)
                    {
	                    strcpy(list[slot].surname, &fields[i]);
                        printf("%s\n",list[slot].surname);
                    }
	                else if (i == 1)
                    {
	                    strcpy(list[slot].name, &fields[i]);
                        printf("%s\n",list[slot].name);
                    }
	                else if (i == 2)
                    {
	                    list[slot].phone = atoi(&fields[i]);
                        printf("%d\n",list[slot].phone);
                    }
	                else if (i == 3)
                    {
	                    strcpy(list[slot].adres, &fields[i]);
                        printf("%s\n",list[slot].adres);
                    }
	                else if (i == 4)
	                {
	                    if (strcmp("personal", &fields[i]) == 0)
                        {
	                        list[slot].st = personal;
                            printf("%d\n",list[slot].st);
                        }
	                    else if (strcmp("work", &fields[i]) == 0)
                        {
	                        list[slot].st = work;
                            printf("%d\n",list[slot].st);
                        }
	                }
	                else if (i == 5)
	                {
	                	printf("st = %d\n", list[slot].st);
	                    switch (list[slot].st)
	                    {
	                    case 0:

	                        list[slot].type.dofb.d = atoi(&fields[i]);
                            printf("%d\n",list[slot].type.dofb.d);

	                        break;

	                    case 1:
	                    	strcpy(list[slot].type.ofc.position, &fields[i]);
                            printf("%s\n",list[slot].type.ofc.position);

	                        break;
	                    }
	                }
	                else if (i == 6)
	                {
	                	printf("st = %d\n", list[slot].st);
	                    switch (list[slot].st)
	                    {
	                    case 0:
	                    	list[slot].type.dofb.m = atoi(&fields[i]);
                            printf("%d\n",list[slot].type.dofb.m);

	                        break;

	                    case 1:
	                        strcpy(list[slot].type.ofc.organization, &fields[i]);
                            printf("%s\n",list[slot].type.ofc.organization);

	                        break;
	                    }
	                }
	                else if (i == 7)
                    {
	                    list[slot].type.dofb.y = atoi(&fields[i]);
                        printf("%d\n",list[slot].type.dofb.y);
                    }
	            }
            }
        }
        slot++;
        printf("slot = %d\n", slot);
    }
    printf("slot1 = %d\n", slot);
    if (slot == MAX && flag == 0)
        printf("\nВ таблице больше нет места\n");
    else
        printf("OK\n");

    printf ("FOK");
    printf("\n");

    fclose(f_inp);
}

void fprint_list(void)
{
    char name[30];

    printf("Введите название результирующего файла, с расширением: ");
    scanf("%s", name);

    FILE *f_out;

    f_out = fopen(name, "w");

    if (f_out == NULL)
    {
        printf("Не удалось открыть файл\n");
        return;
    }

    for (int i = 0; i < MAX; i++)
    {
        if (list[i].surname[0] != '\0')
        {
            fprintf(f_out, "%s,", list[i].surname);
            fprintf(f_out, "%s,", list[i].name);
            fprintf(f_out, "%d,", list[i].phone);
            fprintf(f_out, "%s,", list[i].adres);

            switch (list[i].st)
            {
            case 0:
            	fprintf(f_out, "personal,");
                fprintf(f_out, "%d.%d.%d\n", list[i].type.dofb.d, list[i].type.dofb.m, list[i].type.dofb.y);

                break;

            case 1:
            	fprintf(f_out, "work,");
                fprintf(f_out, "%s,", list[i].type.ofc.position);
                fprintf(f_out, "%s\n", list[i].type.ofc.organization);

                break;

            }
        }
    }

    printf("OK\n\n");

    fclose(f_out);
}

// Ввод вывод для таблицы ключей
void scan_key_list(void)
{
    for (int i = 0; i < MAX; i++)
    {
        if (list[i].surname[0] != '\0')
        {
            list_surname[i].index_src = i + 1;
            strcpy(list_surname[i].surname, list[i].surname);
        }
    }
}

void print_key_list(void)
{
    int flag = find_not_free_2();

    if (flag != -1)
        for (int i = 0; i < MAX; i++)
        {
            if (list_surname[i].index_src != -1)
            {
                printf("Исходный индекс: %d\n", list_surname[i].index_src);
                printf("Фамилия: %s\n", list_surname[i].surname);

                printf("\n");
            }
        }
    else
        printf("Таблица ключей пуста\n\n");
}

// Выбор типа 
int type(void)
{
    int c;

    printf("1. Личная\n");
    printf("2. Служебная\n");

    do {
        printf("\nВаш выбор: ");
        if (scanf("%d", &c) != 1)
        {
            printf("\nНеверные входные данные\n");
            scanf("%*s");
            c = -1;
        }
    } while (c < 0 || c > 2);

    printf("\n");

    return c;
}

// Ввод/вывод на экран
void scan_list(void)
{
    int slot;

    slot = find_free();

    if (slot == -1)
    {
        printf("\nВ таблице больше нет места\n\n");

        return;
    }

    printf("Введите фамилию: ");
    if (scanf("%s", list[slot].surname) != 1)
    {
        printf("\nНеверные входные данные\n\n");
        list[slot].surname[0] = '\0';

        return;
    }

    printf("Введите имя: ");
    if (scanf("%s", list[slot].name) != 1)
    {
        printf("\nНеверные входные данные\n\n");
        list[slot].surname[0] = '\0';
        return;
    }

    printf("Введите номер телефона: ");
    if (scanf("%d", &list[slot].phone) != 1)
    {
        printf("\nНеверные входные данные\n\n");
        scanf("%*s");
        list[slot].surname[0] = '\0';
        return;
    }


    printf("Введите адрес: ");
    if (scanf("%s", list[slot].adres) != 1)
    {
        printf("\nНеверные входные данные\n\n");
        list[slot].surname[0] = '\0';
        return;
    }

    printf("\nВыберете тип информации:\n");
    int choice = type();
    switch(choice)
    {
        case 1: list[slot].st = personal;

            printf("Введите дату рождения (дд мм гггг): ");
            if (scanf("%d %d %d", &list[slot].type.dofb.d, &list[slot].type.dofb.m, &list[slot].type.dofb.y) != 3)
            {
                printf("\nНеверные входные данные\n\n");
                list[slot].surname[0] = '\0';
                return;
            }
            else if ((list[slot].type.dofb.d < 0) || (list[slot].type.dofb.d > 31) || (list[slot].type.dofb.m < 0) || (list[slot].type.dofb.m > 12) || (list[slot].type.dofb.y < 0))
            {
                printf("\nНеверные входные данные\n\n");
                list[slot].surname[0] = '\0';
                return;
            }

            break;

        case 2: list[slot].st = work;


		    printf("Введите должность: ");
		    if (scanf("%s", list[slot].type.ofc.position) != 1)
		    {
		        printf("\nНеверные входные данные\n\n");
		        list[slot].surname[0] = '\0';
		        return;
		    }

		    printf("Введите организацию: ");
		    if (scanf("%s", list[slot].type.ofc.organization) != 1)
		    {
		        printf("\nНеверные входные данные\n\n");
		        list[slot].surname[0] = '\0';
		        return;
		    }

            printf("\n");

            break;
    }
}

void print_list(void)
{
    int flag = find_not_free_1();

    if (flag != -1)
        for (int slot = 0; slot < MAX; ++slot)
        {
            if (list[slot].surname[0])
            {
                int i = 0, j = 0;
                for (i = 0; i < MAX; i++)
                {
                    if (list[i].surname[0])
                        j++;
                    if (i == slot)
                        break;
                }
                printf("Index: %d\n", j);
                printf("Фамилия: %s\n", list[slot].surname);
                printf("Имя: %s\n", list[slot].name);
                printf("Телефон: %d\n", list[slot].phone);
                printf("Адрес: %s\n", list[slot].adres);

                switch (list[slot].st)
                {
                case 0:
                    printf("Дата рождения: %d.%d.%d\n", list[slot].type.dofb.d, list[slot].type.dofb.m, list[slot].type.dofb.y);

                    break;

                case 1:

                    printf("Должность: %s\n", list[slot].type.ofc.position);
                    printf("Организация: %s\n", list[slot].type.ofc.organization);

                    break;


                }

                printf("\n");
            }
        }
    else
        printf("Таблица абонентов пуста\n\n");

}

// Сортировка
int compare_list(const void *p, const void *q)
{
    const listp* a = (const listp*)p;
    const listp* b = (const listp*)q;

    return strcmp(a->surname, b->surname);
}

void sort_list(void)
{
    int f = find_not_free_1();
    if (f != -1)
    {
        qsort(list, MAX, sizeof(listp), compare_list);
        printf("OK\n");
    }
    else
        printf("List is empty\n");

    printf("\n");
}

int compare_key_list(const void *p, const void *q)
{
    const key_listp* a = (const key_listp*)p;
    const key_listp* b = (const key_listp*)q;

    return strcmp(a->surname, b->surname);
}

void sort_key_list(void)
{
    int f = find_not_free_1();
    if (f != -1)
    {
        qsort(list_surname, MAX, sizeof(key_listp), compare_key_list);
        printf("OK\n");
    }
    else
        printf("Key list is empty\n");

    printf("\n");
}

void swap(void *x, void *y, int size)
{
    char *a = x;
    char *b = y;
    char temp;

    for (int i = 0; i < size; i++)
    {
        temp = *a;
        *a = *b;
        *b = temp;

        a++;
        b++;
    }
}



void mysort(void *a, int n, int sizem, int (*compare)(const void *, const void *))
{

    char *pb = a;
    char *pe = pb + n * sizem;
    for (char *pi = pe - sizem; pi >= pb + sizem; pi = pi - sizem) 
    {
        char *max_i = pi;
        for (char *pj = pi - sizem; pb <= pj; pj = pj - sizem) 
        {
            if (compare(pj, max_i) > 0) 
            {
                max_i = pj;
            }
        }
        swap(pi, max_i, sizem);
    }
}

// Функция для ДР
void spec(void)
{
    int flag = 0;
    int day[7];
    int month[7];

    printf("Введите сегодняшнюю дату (дд мм)");
    scanf("%d %d", &day[0], &month[0]);
    for (int i = 1; i < 8; i++)
    {
	    day[i] = day[i-1] + 1;
	    month[i] = month[i-1];
	    if ((day[i]  > 31) && ((month[i] == 1) || (month[i] == 3) || (month[i] == 5) || (month[i] == 7) || (month[i] == 8) || (month[i] == 10)))
	    {
	    	month[i] = month[i] + 1;
	    	day[i] = day[i] - 31;
	    }
	    
	    else if ((day[i] > 31) && (month[i] == 12))
	    {
	    	month[i] = 1;
	    	day[i] = day[i] - 31;
	    }
	    
	    else if ((day[i]  > 30) && ((month[i] == 4) || (month[i] == 6) || (month[i] == 9) || (month[i] == 11) ))
	    {
	    	month[i] = month[i] + 1;
	    	day[i] = day[i] - 30;
	    }
	    
	    else if ((day[i] > 28) && (month[i] == 2))
	    {
	    	month[i] = month[i] + 1;
	    	day[i] = day[i] - 28;
	    }
	}


    for (int i = 0; i < MAX; i++)
    {
    	for (int j = 0; j < 8; j++)
        if (list[i].surname[0] && list[i].st == personal && list[i].type.dofb.d == day[j] && list[i].type.dofb.m == month[j])
        {
            printf("%s\n", list[i].surname);
            flag = 1;
        }

    }

    if (flag == 0)
    {
        printf("В ближайшую неделю нет дней рождения\n");
    }

    printf("\n");
}


unsigned long long tick(void)
{
    unsigned long long d;

    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

void check_ef(void)
{
    int f_1 = find_not_free_1();
    int f_2 = find_not_free_2();

    if (f_1 != -1 && f_2 != -1)
    {
        unsigned long long tb, te, t_1, t_2;

        t_1 = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < MAX; j++)
                list_q[j] = list[j];
            tb = tick();
            qsort(list_q, MAX, sizeof(listp), compare_list);
            te = tick();
            t_1 += (te - tb);
        }

        t_2 = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < MAX; j++)
                list_surname_q[j] = list_surname[j];
            tb = tick();
            qsort(list_surname_q, MAX, sizeof(key_listp), compare_key_list);
            te = tick();
            t_2 += (te - tb);
        }

        printf("t_1 = %llu\n", t_1);
        printf("t_2 = %llu\n", t_2);

        printf("Сортировать таблицу ключей на %llu%% быстрее, чем исходную\n", (100 - 100 * t_2 / t_1));

        unsigned long long size_1, size_2;

        size_1 = sizeof(list) * MAX;
        size_2 = (sizeof(list) + sizeof(list_surname)) * MAX;

        printf("size_1 = %llu\n", size_1);
        printf("size_2 = %llu\n", size_2);

        printf("Таблица ключей занимает дополнительно %llu%% памяти\n", (100 - 100 * size_1 / size_2));

        printf("\n");
    }
    else
        printf("Таблицы пусты\n\n");
}

void check_sort(void)
{
    int f_2 = find_not_free_2();

    if (f_2 != -1)
    {
        unsigned long long tb, te, t_1, t_2;

        t_1 = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < MAX; j++)
                list_surname_m[j] = list_surname[j];
            tb = tick();
            mysort(list_surname_m, MAX, sizeof(key_listp), compare_key_list);
            te = tick();
            t_1 += (te - tb);
        }

        t_2 = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < MAX; j++)
                list_surname_q[j] = list_surname[j];
            tb = tick();
            qsort(list_surname_m, MAX, sizeof(key_listp), compare_key_list);
            te = tick();
            t_2 += (te - tb);
        }

        printf("t_1 = %llu\n", t_1);
        printf("t_2 = %llu\n", t_2);

        printf("Быстрая сортировка на %llu%% быстрее, чем !!!шейкерная\n", (100 - 100 * t_2 / t_1));

        printf("\n");
    }
    else
        printf("Таблицы пусты\n\n");
}


// Меню
int menu_select(void)
{
    int c;

    printf("1. Добавить абонента в таблицу\n");
    printf("2. Вывести таблицу абонентов\n");
    printf("3. Удалить абонента из таблицы\n");
    printf("4. Считать таблицу абонентов из файла\n");
    printf("5. Записать таблицу абонентов в файл\n");
    printf("6. Отсортировать таблицу абонентов по фамилии\n");
    printf("7. Вывести таблицу ключей\n");
    printf("8. Отсортировать таблицу ключей по фамилии\n");
    printf("9. Результат сравнения эффективности обработки данных\n");
    printf("10. Результат сравнения сортировок\n");
    printf("11. Вывести список абонентов, у которых ДР в ближайшую неделю\n");
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

// мейн
int main(void)
{
    setbuf(stdout, NULL);
    
    char choice;

    init_list();
    init_key_list();

    for(;;)
    {
        choice = menu_select();
        switch(choice)
        {
        case 1:
            scan_list();
            init_key_list();
            scan_key_list();
            break;
        case 2: 
        	print_list();
            break;
        case 3: 
        	del();
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
        	fprint_list();
            break;
        case 6: 
        	sort_list();
            break;
        case 7: 
        	print_key_list();
            break;
        case 8: 
        	sort_key_list();
            break;
        case 9: 
        	check_ef();
            break;
        case 10: 
        	check_sort();
            break;
        case 11: 
        	spec();
            break;
        case 0: 
        	return 0;
        }
    }
}
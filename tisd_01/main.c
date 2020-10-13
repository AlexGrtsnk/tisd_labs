#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
typedef long BigInt[100];
//BigInt A, B, C;
//long Osn = 1000000000;
//long lenOsn=9;


void Init(BigInt A, int k)
{
    int i;
    for (i = 0; i < 100; i++)
        A[i] = k;
}

void BigIntInput(BigInt A, char inStr[200])
{
    long Osn = 1000000000;
    long lenOsn=9;

    //char inStr[200];
    char buf[10];
    int i, j, startPos, endPos;
    Init(A, 0);
    //scanf("%s", inStr);
    A[0]=strlen(inStr)/lenOsn;
    if (strlen(inStr)%lenOsn != 0) A[0]++;
    startPos=strlen(inStr)-lenOsn;
    endPos=strlen(inStr);
    for (i=1; i<=A[0]; i++)
    {
        if (startPos<0) startPos=0;
        for (j=0; j<(endPos-startPos); j++)
            buf[j]=inStr[startPos+j];
            buf[endPos-startPos]='\0';
            A[i]=atol(buf);
            endPos=startPos;
            startPos=endPos-lenOsn;
    }
    return;
}

void BigIntOutput(BigInt A)
{
    long Osn = 1000000000;
    long lenOsn=9;

    int len, i, j;
    char tmp[10];
    len=A[0];
    printf("%d", A[len]);
    for (i=len-1; i>0; i--)
    {
        sprintf(tmp, "%d", A[i]);
        for (j=0; j<lenOsn-strlen(tmp); j++)
            printf("0");
        printf("%d",A[i]);
    }
    return;
}

int Compare(BigInt A, BigInt B)
{
    long Osn = 1000000000;
    long lenOsn=9;

    int i;
    if (A[0]<B[0])
        return -1;
    else if(A[0]>B[0]) return 1;
    for (i=A[0]; i>0; i--){
        if (A[i]<B[i]) return -1;
        else if(A[i]>B[i]) return 1;
    }
    return 0;
}
void Shift1(BigInt a, int s){
    int i, j;
    int y = 0;
    for(i=a[0];i>0;i--)
        a[i+s] = a[i];
    for(j=1;j<=s;j++) a[j]=0;
    a[0]+=s;
    int kol;
    kol++;
}

void Shift(BigInt a, int s){
    int i, j;
    int y = 0;
    for(i=a[0];i>0;i--)
        a[i+s] = a[i];
    for(j=1;j<=s;j++) a[j]=0;
    a[0]= a[0]+s;
    int kol;
    kol++;
}


void Shift_2(BigInt array, int size) {
     int i, temp = array[0];
     for (i = 0, size--; array[i] > 0; i++) array[i] = array[i+1];
     array[i] = temp;
  }

void Shift_3(BigInt A,int x)
{
    int i = 1, a = 0;
    int l = A[1];
    while (A[i+1] > 0)
    {
        A[i] = A[i+1];
        i++;
    }
    A[i] = l;
}


void Shift_4(BigInt array, int size) {
    int temp = array[--size];
    while ( size > 0 ) array[size--] = array[size-1];
    array[0] = temp;
  }




void Sub(BigInt A, BigInt B, BigInt C)
{
    long Osn = 1000000000;
    long lenOsn=9;

    long i, carry=0;
    C[0]=max(A[0], B[0]);
    for (i=1; i<=C[0]; i++)
    {
        C[i]=A[i]-B[i]-carry;
        if (C[i]<0)
        {
            C[i]=C[i] + Osn;
            carry=1;
        }
        else
            carry=0;
        } //for
        while (C[C[0]]==0) C[0]--;
        if (C[0]<1) C[0]=1;
        return; } //sub
  //      if (compare(A, B)==-1) Sub(B, A, C); else Sub(A, B, C);


void DivonShort (BigInt A, long divisor, BigInt B)
    {
            long Osn = 1000000000;
            long lenOsn=9;

        __int64 tmp; int i; __int64 ost=0;
        B[0]=A[0];
        for (i=A[0]; i>0; i--)
            { tmp = ost*Osn+A[i];
            B[i] = tmp / divisor;
            ost = tmp % divisor;
            }
        while (B[B[0]]==0) B[0]--;
        if (B[0]<1) B[0]=1;
        return;
        }



void MultonShort(BigInt A, long mnoj, BigInt B)
{
    long Osn = 1000000000;
    long lenOsn=9;

    __int64 tmp;
    int i;
    long carry=0;
    B[0]=A[0];
    for (i=1; i<=A[0]; i++)
    {
        tmp=(__int64)A[i]*mnoj + carry;
        carry = tmp / Osn;
        B[i] = tmp % Osn;
    }
    if (carry>0)
    {
        B[0]++;
        B[B[0]]=carry;
    }
    return;
}

void Copy(BigInt A, BigInt B)
{
    int i;
    for (i = 0; i < 100; i++)
        A[i] = B[i];
}


 void Mult(BigInt A, BigInt B, BigInt C)
 {
    long Osn = 1000000000;
    long lenOsn=9;
    int i1, i2, i3;
    long tmp;
    int maxlen = A[0]+B[0]+1;
    Init(C, 0);
    for (i1=1; i1<=A[0]; i1++)
        for (i2=1; i2<=B[0]; i2++)
        {
            i3=i1+i2 -1;
            tmp=(__int64)A[i1]*B[i2];
            while (tmp>0)
            {
                tmp = tmp + C[i3];
                C[i3] = tmp % Osn;
                tmp = tmp / Osn;
                i3++;
            }
        }
        for (i1=maxlen-1; i1>0;i1--)
        {
            if (C[i1]!=0)
            {
                C[0]=i1;
                return;
            }
        }
        C[0]=1;
    return;
}

void Add(BigInt A, BigInt B, BigInt C)
{
    long Osn = 1000000000;
    long lenOsn=9;

    long i, carry=0;
    C[0]=max(A[0], B[0]);
    for (i=1; i<=C[0]; i++)
    {
        C[i]=A[i]+B[i]+carry;
        carry=C[i]/Osn;
        C[i]=C[i]%Osn;
    } //for
    if (carry>0)
    {
        C[0]++;
        C[C[0]]=carry;
    }
    return;
}


void Diff(BigInt A, BigInt B, BigInt C)
{
    BigInt C1, A1;
    BigInt N;
    BigInt up, down,digit, last_success,tmp2,tmp;
    Init(N, 0);
    N[0] = 1;
    N[1] = 2;
    Init(C1,0);
    Init(A1,0);
    Init(up,0);
    Init(down,0);
    Init(digit,0);
    Init(tmp,0);
    Init(last_success,0);
    Add(C,N,C1);
    Copy(C, C1);
    Mult(B,C,A1);
    //while (Compare(A,A1) == 1)
    //{
        //Init(A1,0);
        //Init(C1,0);
        //Add(C,N,C1);
        //Copy(C,C1);
        //Mult(B,C,A1);
        Copy(up,A);
 while (A)
        {
            Add(up,down,tmp);
            DivonShort(tmp,2,digit);
            //digit=(up+down)/2;

            Init(tmp2, 0);
            Mult(B, digit, tmp2);
            if (Compare(A, tmp2)==-1)
                Sub(digit,N,up);//up=digit-1;
            else
            {
                Add(digit,N,down);//down=digit+1;
                Copy(last_success,digit);
            }
         if (Compare(up,down)==-1) break;
        } //end of binary search




    //}
    Copy(C,last_success);
    return;
}



int main(void)
{
    setlocale(LC_CTYPE,"Russian");
    char s[64];
    char cel[31];
    char m[31];
    char n[34];
    char k_1[7];
    int tr_1 = 0,tr_2 = 0,tr_3 = 0, tr_4 = 0, tr_5 = 0, tr_6 = 0, tr_dot = 0, tr_E = 0;
    int umn = 0, delen = 0;
    int num_e;
    int i = 1, j = 0, k = 0, l = -1, f, d = 0;
    int znak_del;
    long long a_1;
    double a_2;
    char *start='\0',*end='\0';
    int tr_1_no = 0, tr_2_no = 0;
    printf("Действительное число вводится так: +-m.nЕ+-K\n");
    printf("Пожалуйста, вводите целое число со знаком, например +1 или -1\n");
    printf("Пожалуйста, введите действительное число\n");
    fgets(s,65,stdin);
    if (s[0] == '-')
        {tr_1 = -1; tr_3 = -1;}
    if (s[0] == '+')
        {tr_2 = 1; tr_3  = 1;}
    if ((tr_1 == 0) && (tr_2 == 0))
    {
        printf("Вы не ввели знак числа, пожалуйста введите его\n");
        return EXIT_FAILURE;
    }
    while (s[i]!='.')
    {
        tr_dot = 1;
        m[j] = s[i];
        i++;
        j++;
        if (i == strlen(s) - 1)
        {
            printf("Вы неверно ввели число, хочется Вам напомнить что верный формат +-m.nЕ+-K\n");
            return 1;
        }
    }
    m[j] = '\0';
    if (tr_dot == 1)
    {
        i++;
        while(s[i]!='E')
        {
            tr_E = 1;
            n[k] = s[i];
            i++;
            k++;
        if (i == strlen(s) - 1)
        {
            printf("Вы неверно ввели число, хочется Вам напомнить что верный формат +-m.nЕ+-K\n");
            return 1;
        }
        }
        if (tr_E == 1)
        {
            n[k] = '\0';
            if (strlen(n) + strlen(m) > 30)
            {
                printf("Вы ввели слишком большую мантису\n");
                return EXIT_FAILURE;
            }
            i++;
            if (s[i] == '+')
            {
                tr_4 = 1;
                tr_6 = 1;
            }
            if (s[i]!='-')
            {
                tr_5 = -1;
                tr_6 = -1;
            }
            if ((tr_4 == 0) && (tr_5 == 0))
            {
                printf("Вы не ввели знак порядка, пожалуйста введите его\n");
                return EXIT_FAILURE;
            }
            k_1[0] = s[i];
            i++;
            for (f = i;f<strlen(s);f++)
            {
                d++;
                k_1[d]=s[f];
            }
            k_1[d] = '\0';
            long k_3 = atol(k_1);
            if (k_3 > 99999 || k_3 < -99999)
            {
                printf("Вы ввели слишком большой порядок\n");
                return EXIT_FAILURE;
            }
        }
        else
        {
            printf("Вы неверно ввели число, хочется Вам напомнить что верный формат +-m.nЕ+-K\n");
            return EXIT_FAILURE;
        }

    }
    else
    {
        printf("Вы неверно ввели число, хочется Вам напомнить что верный формат +-m.nЕ+-K\n");
        return EXIT_FAILURE;
    }
    printf("пожалуйста, введите целое число\n");
    fgets(cel,32,stdin);
    if (strlen(cel) > 30)
    {
        printf("Вы ввели слишком большое целое число\n");
        return EXIT_FAILURE;
    }
    cel[strlen(cel) - 1] = '\0';
    tr_4 = 0;
    tr_5 = 0;
    tr_6 = 0;
    if (cel[0]=='-')
        {tr_4 = -1; tr_6 = -1;}
    if (cel[0]=='+')
        {tr_5 = 1; tr_6  = 1;}
    if ((tr_4 == 0) && (tr_5 == 0))
    {
        printf("Вы не ввели знак целого числа, пожалуйста введите его\n");
        return EXIT_FAILURE;
    }
    znak_del = tr_3 * tr_6;
    long k_2 = atol(k_1);
    char cel_new[31];
    for (i = 1; i < strlen(cel); i++)
        cel_new[i-1] = cel[i];
    cel_new[strlen(cel) - 1] = '\0';

    int len_m = strlen(m);
    int k_obsh_m = k_2 + len_m;
    int len_cel = strlen(cel) - 1;
    int k_vse = k_obsh_m - len_cel;
    long asd = atol(cel_new);
    long qwe = atol(n);
    char vse_mn[123];
    strcpy(vse_mn,m);
    strcat(vse_mn,n);
    BigInt A,B,C;
    BigIntInput(A, vse_mn);
    BigIntInput(B, cel_new);
    int lop = 0;
    BigInt A11, L6;
    Init(L6,0);
    L6[0] = 1;
    Init(A11,0);
    Init(C,0);
    printf("asdasd");
    if (Compare(A,L6) == 0){
        printf("Результат\n%d.0E+0",0); return 0;}
    if (Compare(B,L6) == 0){
        printf("На ноль делить нельзя, пожалуйста, введите целое число заново"); return 1;}
    while (Compare(A,B) == -1)
    {
        MultonShort(A,10,A11);
        Copy(A,A11);
        k_vse--;
    }
    Diff(A, B, C);
    BigInt G2;
    Init(G2,0);
    L6[1] = 1;
    printf("sdad");
    if (strlen(vse_mn) == 30)
    {
        char new_1[100];
        for (i = 0; i <= strlen(cel_new); i++){
        strcpy(new_1,vse_mn[strlen(vse_mn) - 1 - i]);
        strcpy(new_1,vse_mn[strlen(vse_mn) - 2 - i]);}
        long jkl = atol(new_1);
        long lkj = atol(cel);
        if (jkl % lkj >= 5)
        {
            Add(C,L6,G2); 
            Copy(C,G2);
        }    
    }
    if (k_vse > 99999){
        printf("Произошло переполнение"); return 1;}
    //BigIntOutput(C);
    printf("Результат\n");
    if (znak_del < 0)
        printf("-0.");
    else
        printf("0.");
    BigIntOutput(C);
    printf("E");
    if (k_vse < 0)
        printf("%d",k_vse+1);
    else
        printf("+%d",k_vse+1);
//    getch();
    return 0;
}

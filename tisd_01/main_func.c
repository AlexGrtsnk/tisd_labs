#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

void BigIntInput(BigInt A)
{
    long Osn = 1000000000;
    long lenOsn=9;

    char inStr[200];
    char buf[10];
    int i, j, startPos, endPos;
    Init(A, 0);
    scanf("%s", inStr);
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

int maxmy(long a, long b)
{
    if (a > b)
        return a;
    else
    {
        return b;
    }
    
}


void Sub(BigInt A, BigInt B, BigInt C)
{
    long Osn = 1000000000;
    long lenOsn=9;

    long i, carry=0;
    C[0]=maxmy(A[0], B[0]);
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

        long long tmp; int i; long long ost=0; 
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

    long long tmp;
    int i;
    long carry=0;
    B[0]=A[0];
    for (i=1; i<=A[0]; i++)
    {
        tmp=(long long)A[i]*mnoj + carry;
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
            tmp=(long long)A[i1]*B[i2]; 
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
    C[0]=maxmy(A[0], B[0]);
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
    //printf("asdasd111111\n");
    BigInt A, B, C;
    BigIntInput(A);
    BigIntInput(B);
    Init(C,0);
    Diff(A, B, C);
    BigIntOutput(C);
    printf("\n");
    return 0;
}

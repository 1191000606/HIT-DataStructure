#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>

#define MaxLen 20

typedef union data
{
   double count;
   double rate;
}data;

typedef struct charNode
{
    char Character;
    data occur;
    struct charNode *next;
}charNode;
typedef charNode *rateList;

typedef struct HuffmanNode
{   
    double rate;
    int lchild;
    int rchild;
    int parent;
}HuffmanNode;
typedef HuffmanNode *HuffmanTree;

typedef struct HuffmanCode
{
    char Character;
    char Code[MaxLen];
}HuffmanCode;
typedef HuffmanCode *codeList;

int kindnum=0;

int CreateRL(rateList *pRL,FILE *fp);

void InitTree(HuffmanTree T,rateList RL);
void CreateTree(HuffmanTree T);
void SelectMin(HuffmanTree T,int *min);
int Cmp(HuffmanNode T[],int i,int j);

void InitCL(codeList CL,rateList RL);
void CreateCL(codeList CL,HuffmanTree T);
void Encoding(FILE *raw,codeList CL,int total,int kindnum);
void Compress();

void Free(rateList *pRL,HuffmanTree T,codeList CL);


int main()
{   
    FILE *fp=fopen("hamlet.txt","r");

    rateList RL=NULL;

    int total=CreateRL(&RL,fp);
    fclose(fp);
    
    HuffmanTree T=(HuffmanNode*)malloc((2*kindnum-1)*sizeof(HuffmanNode));

    InitTree(T,RL);    

    CreateTree(T);

    codeList CL=(HuffmanCode*)malloc(kindnum*sizeof(HuffmanCode));

    InitCL(CL,RL);

    CreateCL(CL,T);

    fp=fopen("hamlet.txt","r");
    Encoding(fp,CL,total,kindnum);
    fclose(fp);

    Compress();
    
    Free(&RL,T,CL);

    return 0;
}

int CreateRL(rateList *pRL,FILE *fp)
{
    (*pRL)=(charNode*)malloc(sizeof(charNode));
    (*pRL)->next=NULL;

    int total=0;

    while(1)
    {   
        char a;

        if(fscanf(fp,"%c",&a)==EOF)
        {
            break;
        }
        
        total++;

        charNode *p=(*pRL)->next;

        while(p!=NULL)
        {
            if(p->Character==a)
            {
                break;
            }
            p=p->next;
        }

        if(p!=NULL)
        {
            p->occur.count++;
        }
        else
        {   
            kindnum++;
            p=(charNode*)malloc(sizeof(charNode));
            p->Character=a;
            p->occur.count=1;
            p->next=(*pRL)->next;
            (*pRL)->next=p;
        }
    }

    charNode *p=(*pRL)->next;

    while(p!=NULL)
    {
        p->occur.rate=p->occur.count/total;
        p=p->next;
    }

    return total;
}

void InitTree(HuffmanTree T,rateList RL)
{
    charNode *p=RL->next;

    for(int i=0;i<kindnum;i++)
    {
        T[i].rate=p->occur.rate;
        T[i].parent=-1;
        T[i].lchild=-1;
        T[i].rchild=-1;
        p=p->next;
    }

    for(int i=kindnum;i<2*kindnum-1;i++)
    {
        T[i].rate=2.0;
        T[i].parent=-1;
        T[i].lchild=-1;
        T[i].rchild=-1;
    }
}

void CreateTree(HuffmanTree T)
{
    int min;

    for(int i=0;i<kindnum-1;i++)
    {
        SelectMin(T,&min);
        T[kindnum+i].lchild=min;
        T[min].parent=kindnum+i;
        double temp=T[min].rate;
        
        SelectMin(T,&min);
        T[kindnum+i].rchild=min;
        T[min].parent=kindnum+i;
        T[kindnum+i].rate=temp+T[min].rate;
    }
}

void SelectMin(HuffmanTree T,int *min)
{   
    *min=0;
    for(int i=1;i<2*kindnum-1;i++)
    {
        if(Cmp(T,*min,i)==1)
        {
            *min=i;
        }
    }
}

int Cmp(HuffmanNode T[],int i,int j)
{
    if(T[j].parent==-1&&(T[i].parent!=-1||T[j].rate<T[i].rate))
    {
        return 1;
    }
    return 0;
}

void InitCL(codeList CL,rateList RL)
{
    charNode *p=RL->next;

    for(int i=0;i<kindnum;i++)
    {
        CL[i].Character=p->Character;
        CL[i].Code[0]='\0';
        p=p->next;
    }
}

void CreateCL(codeList CL,HuffmanTree T)
{
    for(int i=0;i<kindnum;i++)
    {
        int p=T[i].parent;
        int c=i;
        char Code[MaxLen];
        int num=MaxLen-1;

        while(p!=-1)
        {
            if(T[p].lchild==c)
            {
                Code[--num]='1';
            }
            else
            {
                Code[--num]='0';
            }
            c=p;
            p=T[p].parent;
        }

        strcpy(&CL[i].Code[0],&Code[num]);
    }
}

void Encoding(FILE *raw,codeList CL,int total,int kindnum)
{   
    char folderName[15]="Compression";
    _mkdir(folderName);

    FILE *key=fopen("Compression\\key.txt","w");

    fprintf(key,"%d %d\n",total,kindnum);

    for(int i=0;i<kindnum;i++)
    {
        fprintf(key,"'%c' %s\n",CL[i].Character,CL[i].Code);
    }
    
    FILE *processed=fopen("Compression\\processed.txt","w");

    char character;

    while(fscanf(raw,"%c",&character)!=EOF)
    {   
        int i=-1;
        while(CL[++i].Character!=character);
        fprintf(processed,"%s",CL[i].Code);
    }

    fclose(key);
    fclose(processed);
}

void Compress()
{   
    FILE *processed=fopen("Compression\\processed.txt","r");
    FILE *Compressed=fopen("Compression\\Compressed.txt","wb");

    int flag=1;
    unsigned int a;

    while(flag)
    {   
        a=0;
        char b;
        for(int i=0;i<8;i++)
        {
            if(fscanf(processed,"%c",&b)!=EOF)
            {   
                a=a<<1;
                if(b=='1')
                {
                    a=a|1;
                }
            }
            else
            {   
                a=a<<(7-i);
                flag=0;
                break;
            }
        }
        fprintf(Compressed,"%c",(unsigned char)a);
    }

    fclose(processed);
    fclose(Compressed);
}

void Free(rateList *pRL,HuffmanTree T,codeList CL)
{
    charNode *p=(*pRL);
    charNode *q=(*pRL)->next;

    while(1)
    {
        free(p);
        p=q;
        if(q==NULL)
        {
            break;
        }
        q=q->next;
    }

    free(T);

    free(CL);
}

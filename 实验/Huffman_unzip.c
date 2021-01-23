#include <stdio.h>
#include <stdlib.h>


typedef struct HuffmanNode
{   
    char character;
    int lchild;
    int rchild;
}HuffmanNode;
typedef HuffmanNode *HuffmanTree;

void Decompress();

int main()
{   
    Decompress();

    return 0;
}

void Decompress()
{
    FILE *key=fopen("Compression\\key.txt","r");
    FILE *Compressed=fopen("Compression\\Compressed.txt","rb");
    
    int total,kindnum;

    fscanf(key,"%d %d\n",&total,&kindnum);

    HuffmanNode T[2*kindnum-1];
    for(int i=0;i<2*kindnum-1;i++)
    {
        T[i].lchild=-1;
        T[i].rchild=-1;
        T[i].character='#';
    }

    char character,code[20];
    int index=1;

    for(int i=0;i<kindnum;i++)
    {   
        fscanf(key,"'%c' %s\n",&character,code);
        int j=0;
        int p=0;

        while(1)
        {
            if(code[j]=='0')
            {   
                if(T[p].rchild==-1)
                {
                    T[p].rchild=index++;
                }
                p=T[p].rchild;
            }
            else if(code[j]=='1')
            {   
                if(T[p].lchild==-1)
                {
                    T[p].lchild=index++;
                }
                p=T[p].lchild;
            }
            else if(code[j]=='\0')
            {
                T[p].character=character;
                break;
            }
            j++;
        }
    }

    FILE* Decompressed=fopen("Compression\\Decompressed.txt","w");
    unsigned char c;

    int a=0;
    index=0;
    int num=0;

    while(1)
    {   
        fscanf(Compressed,"%c",&c);
        int C[8];
        for(int i=0;i<8;i++)
        {   
            char temp=c;
            if((C[i]=(temp>>(7-i))&1)!=EOF)
            {
                C[i]=(temp>>(7-i))&1;
            }
            else
            {   
                fclose(Compressed);
                return;
            }
        
            if(C[i]==1)
            {
                index=T[index].lchild;
            }
            else if(C[i]==0)
            {
                index=T[index].rchild;
            }

            if(T[index].character!='#')
            {
                fprintf(Decompressed,"%c",T[index].character);
                index=0;
                num++;
                if(num>=total)
                {
                    fclose(Compressed);
                    return;
                }
            }
        }
    }

    fclose(Compressed);
}
#include <stdio.h>
#include <stdlib.h>
#define infinity 2147483647
//实验三.txt中第一行是每年购买机器的价格
//实验三.txt中第二行是机器使用一定年限时的维修价格，默认维修价格只与使用年限有关
//合理修改实验三.txt中的数据也可以得到适当的结果
//输入年限请不要输错

typedef struct Graph
{
    int *Vertex;
    int **Edge;
    int n;
}*Graph;

void CreateGraph(Graph G);
void Dijkstra(Graph G);
int Getmin(int D[],int S[],int n);

int main()
{
    int n;

    printf("您需要几年的设备更新计划：(输入阿拉伯数字)");
    scanf("%d",&n);

    Graph G=(Graph)malloc(sizeof(struct Graph));
    G->n=n+1;
    G->Vertex=(int *)malloc(sizeof(int)*(G->n));
    G->Edge=(int **)malloc(sizeof(int*)*(G->n));
    for(int i=0;i<G->n;i++)
    {
        G->Edge[i]=(int *)malloc(sizeof(int)*(G->n));
    }
    

    CreateGraph(G);

    Dijkstra(G);

    for(int i=0;i<G->n;i++)
    {
        free(G->Edge[i]);
    }
    free(G->Edge);
    free(G->Vertex);
    free(G);
    
    return 0;
}

void CreateGraph(Graph G)
{   
    FILE *fp=fopen("实验三.txt","r");

    for(int i=0;i<G->n-1;i++)
    {
        fscanf(fp,"%d ",&G->Vertex[i]);
    }
    G->Vertex[G->n-1]=0;

    int Cost[G->n-1],cost;
    fscanf(fp,"%d",&Cost[0]);
    for(int i=1;i<G->n-1;i++)
    {   
        fscanf(fp," %d",&cost);
        Cost[i]=Cost[i-1]+cost;
    }

    for(int i=0;i<G->n;i++)
    {
        for(int j=0;j<G->n;j++)
        {
            if(j<=i)
            {
                G->Edge[i][j]=infinity;
            }
            else if(j>i)
            {
                G->Edge[i][j]=G->Vertex[i]+Cost[j-i-1];
            }
        }
    }

    fclose(fp);
}

void Dijkstra(Graph G)
{
    int S[G->n];
    int CLOSEST[G->n];
    int D[G->n];

    for(int i=0;i<G->n;i++)
    {
        S[i]=0;
        CLOSEST[i]=0;
        D[i]=G->Edge[0][i];
    }
    S[0]=1;
    CLOSEST[0]=-1;

    for(int i=1;i<G->n;i++)
    {
        int min=Getmin(D,S,G->n);
        S[min]=1;
        for(int j=min+1;j<G->n;j++)
        {
            if(S[j]==0&&D[min]+G->Edge[min][j]<D[j])
            {
                CLOSEST[j]=min;
                D[j]=D[min]+G->Edge[min][j];
            }
        }
    }

    int way[G->n];
    int index=0;
    way[index]=G->n-1;
    while(way[index]!=0)
    {
        way[index+1]=CLOSEST[way[index]];
        index++;
    }

    int sum=0;
    for(int i=index;i>0;i--)
    {
        printf("第%d年初买设备,使用至第%d年年末,耗资%d+%d=%d\n",way[i]+1,way[i-1],G->Vertex[way[i]],G->Edge[way[i]][way[i-1]]-G->Vertex[way[i]],G->Edge[way[i]][way[i-1]]);
        sum+=G->Edge[way[i]][way[i-1]];
    }
    printf("总金额为%d\n",sum);
}

int Getmin(int D[],int S[],int n)
{
    int min=0;

    for(int i=0;i<n;i++)
    {
        if(S[i]==0&&D[i]<D[min])
        {
            min=i;
        }
    }

    return min;
}


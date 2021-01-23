#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char name[10];
    char brand[10];
    double price;
    int num;
    int index;
    struct node *next;
}product;

typedef struct list
{
    product *head;
    product *last;
    int sum;
}List;

List New();
List Open();
List Buy(List List);
List Sell(List List);
void Find(List List);
void Update(List list);
void close(List List);

List New()
{
    printf("新店开张，祝生意兴隆！\n");
    List List;
    List.head=List.last=(product *)malloc(sizeof(product));
    List.sum=0;
    List.last->next=NULL;

    return List;
}

List Open()
{
    FILE *p1=fopen("data.txt","r");

    List List;

    List.head=List.last=(product *)malloc(sizeof(product));
    List.last->next=NULL;
    List.sum=0;

    if(p1==NULL)
    {
        printf("请新建文件！\n");
        return List;
    }

    while(1)
    {   
        List.last->next=(product *)malloc(sizeof(product));
        if(fscanf(p1,"%d.%s %s %lf %d",&List.last->next->index,List.last->next->name,List.last->next->brand,&List.last->next->price,&List.last->next->num)!=EOF)
        {   
            List.last=List.last->next;
            List.sum++;
        }
        else
        {
            free(List.last->next);
            List.last->next=NULL;
            break;
        }
    }

    fclose(p1);

    return List;
}

List Buy(List List)
{
    product *A=(product *)malloc(sizeof(product));
    product *B=List.head;
    product *C=List.head->next;

    printf("请输入商品名称，品牌，价格，数量：");
    scanf("%s %s %lf %d",A->name,A->brand,&A->price,&A->num);
    A->next=NULL;

    char ch;
    while((ch=getchar())!=EOF&&ch!='\n');

    if(C==NULL)
    {
        A->index=++List.sum; 
        List.last->next=A;
        List.last=A;
        return List;
    }
    
    if(A->price>=List.last->price)
    {
        if(strcmp(List.last->name,A->name)==0&&strcmp(List.last->brand,A->brand)==0)
        {
            List.last->num+=A->num;
            free(A);
        }
        else
        {   
            A->index=++List.sum; 
            List.last->next=A;
            List.last=A;
        }
    }
    else
    {
        while(1)
        {
            if(C->price>A->price)
            {   
                A->index=++List.sum; 
                A->next=C;
                B->next=A;
                break;
            }
            else if(C->price==A->price)
            {
                if(strcmp(C->name,A->name)==0&&strcmp(C->brand,A->brand)==0)
                {
                    C->num+=A->num;
                    free(A);
                }
                else
                {   
                    A->index=++List.sum;
                    A->next=C;
                    B->next=A;
                }
                break;
            }
            else if(C->price<=A->price)
            {
                B=C;
                C=C->next;
            }
        }
    }

    return List;
}

List Sell(List List)
{
    product *B=List.head->next;

    if(B==NULL)
    {
        printf("库存已空!\n");
        return List;
    }

    printf("是否开始查找:(输入1或0):");

    int flag=0;
    scanf("%d",&flag);

    if(flag==0)
    {
        printf("可供选择商品信息如下:\n");
        while(B!=NULL)
        {
            printf("%d.%s %s %.2lf %d\n",B->index,B->name,B->brand,B->price,B->num);
            B=B->next;
        }
    }
    else
    {
        Find(List);
        while(flag!=0)
        {        
            printf("是否再次查找:(输入1或0)");
            scanf("%d",&flag);
            if(flag==1)
            {
                Find(List);
            }
        }
    }

    printf("请输入购买商品的序号及数量:");
    int index,num;
    scanf("%d %d",&index,&num);
    char ch;
    while((ch=getchar())!=EOF&&ch!='\n');

    B=List.head->next;

    while(B->index!=index&&B!=NULL)
    {
        B=B->next;
    }

    if(B==NULL)
    {
        printf("输入错误！\n");
    }

    if(B->num<=num)
    {   
        if(B->num<num)
        {
            printf("存货不足，仅出售%d件\n",B->num);
        }
        B->num=0;
    }
    else
    {
        B->num-=num;
    }

    return List;
}

void Find(List List)
{
    product *B=List.head->next;

    printf("输入1按种类检索，输入2按品牌检索，输入3按价格检索，输入4按库存检索，输入5查看全部:");

    int flag=-1;

    scanf("%d",&flag);

    char ch;
    while((ch=getchar())!=EOF&&ch!='\n');

    switch (flag)
    {
        case 1:
            printf("输入种类:");
            char name[10];
            scanf("%s",&name);
            while(B)
            {   
                if(strcmp(B->name,name)==0)
                    printf("%d.%s %s %.2lf %d\n",B->index,B->name,B->brand,B->price,B->num);
                B=B->next;
            }
            break;
        
        case 2:
            printf("输入品牌:");
            char brand[10];
            scanf("%s",&brand);
            while(B)
            {   
                if(strcmp(B->brand,brand)==0)
                    printf("%d.%s %s %.2lf %d\n",B->index,B->name,B->brand,B->price,B->num);
                B=B->next;
            }
            break;

        case 3:
            printf("输入价格下限,价格上限(空格分隔):");
            double min,max;
            scanf("%lf %lf",&min,&max);
            while(B)
            {   
                if(B->price>=min&&B->price<=max)
                    printf("%d.%s %s %.2lf %d\n",B->index,B->name,B->brand,B->price,B->num);
                B=B->next;
                if(B->price>max)
                {
                    break;
                }
            }
            break;
        
        case 4:
            printf("存货小于20的有:\n");
            while(B)
            {   
                if(B->num<20)
                    printf("%d.%s %s %.2lf %d\n",B->index,B->name,B->brand,B->price,B->num);
                B=B->next;
            }
            break;

        case 5:
            printf("全部商品信息如下:\n");
            while(B)
            {
                printf("%d.%s %s %.2lf %d\n",B->index,B->name,B->brand,B->price,B->num);
                B=B->next;
            }
            break;
    }

    return;
}

void Update(List List)
{
    FILE *p1;

    p1=fopen("data.txt","w");

    product *B;

    B=List.head->next;

    while(B)
    {
        fprintf(p1,"%d.%s %s %.2lf %d\n",B->index,B->name,B->brand,B->price,B->num);
        B=B->next;
    }

    fclose(p1);

    printf("更新完成！\n");

    return;
}

void Close(List List)
{
    FILE *p1;

    p1=fopen("data.txt","w");

    product *B,*A;

    A=B=List.head->next;

    while(B)
    {
        fprintf(p1,"%d.%s %s %.2lf %d\n",B->index,B->name,B->brand,B->price,B->num);
        B=B->next;
        free(A);
        A=B;
    }

    fclose(p1);

    free(List.head);

    printf("今日事今日毕！\n");
    return;
}

int main()
{   
    List List;

    printf("共有七种操作：New,Open,Buy,Sell,Update,Find,Close。使用大写的首字母来表示您要进行的操作。\n");

    char Operations[7]="NOBSUFC";
    char A;

    while(1)
    {
        printf("输入您要进行的操作:");
        scanf("%c",&A);

        char ch;
        while ((ch=getchar())!=EOF&&ch!='\n');

        int i=0;
        for(;i<7;i++)
        {
            if(A==Operations[i])
            {
                break;
            }
        }

        switch (i)
        {
        case 0:
            List=New();
            break;
        
        case 1:
            List=Open();
            break;

        case 2:
            List=Buy(List);
            break;

        case 3:
            List=Sell(List);
            break;

        case 4:
            Update(List);
            break;

        case 5:
            Find(List);
            break;

        case 6:
            Close(List);
            return 0;
        }
    }

    return 0;
}
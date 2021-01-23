#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#define WINDOWS_NUM 10
#define OpenHour 11
#define OpenMin 0
#define OpenSecond 0

typedef struct event
{
    int type;
    int occurTime;
    struct event *next;
}Event;

typedef Event *EventList;

typedef struct student
{
    int arrivalTime;
    int duration;
    int id;
    struct student * next;
}Student;

typedef struct window
{
    Student *front;
    Student *rear;
}StudentQueue;

//采用全局变量
Event event;
EventList eventList;//事件要做到按时间顺序而非先来后到，要实现插入，因此采用链表。
Student student;
StudentQueue studentQueue[WINDOWS_NUM+1];//排队使用队列。
int closeTime;
int studentNum=0;
int totalTime=0;
int hour=OpenHour;
int minute=OpenMin;
int second=OpenSecond;

//食堂模拟函数，以及食堂开门关门函数
void CanteenSimulation();
void CanteenOpen();
void CanteenClose();

//事件只有两种，到（上一个学生的到），离开
void StudentArrival();
void StudentDepature();

//链表操作
void Insert(EventList *pL,Event ev);
void DelEvent(EventList *pL,Event* ev);

//队列操作，分别为学生入队和出队
void EnQueue(StudentQueue *pQ,Student stu);
void DelQueue(StudentQueue *pQ,Student* stu);
int Lenth(StudentQueue Q);
int GetMin(StudentQueue Q[]);

//时间操作
void Transform(int* hour,int *minute,int *second,int time);

int main()
{
    printf("请输入营业时长(以秒计算)：\n>>>");
    scanf("%d",&closeTime);

    srand(time(NULL));

    CanteenSimulation();

    return 0;
}


//----------------------------------食堂模拟------------------------------
void CanteenSimulation()
{
    CanteenOpen();

    while(eventList->next!=NULL)
    {
        DelEvent(&eventList,&event);//取出eventList的第一个保存在event里然后删除
        if(event.type==0)
        {
            StudentArrival();
        }
        else 
        {
            StudentDepature();
        }
    }

    CanteenClose();

    return;
}

void CanteenOpen()
{   
    //初始化带头结点的链表eventList
    eventList=(EventList)malloc(sizeof(Event));
    eventList->next=NULL;

    //食堂开门的同时，生成一个事件，即第一位同学的到来
    event.type=0;
    event.occurTime=0;

    //事件插入链表eventList中
    Insert(&eventList,event);

    //初始化队列，此时四个队列，队列带不带头结点
    for(int i=1;i<=WINDOWS_NUM;i++)
    {
        studentQueue[i].front=(Student*)malloc(sizeof(student));
        studentQueue[i].front->next=NULL;
        studentQueue[i].rear=studentQueue[i].front;
    }
}

void CanteenClose()
{
    printf("---------------------\n共有%d位同学打饭，排队买饭总用时%d秒，平均排队买饭时间%d秒\n", studentNum, totalTime, totalTime/studentNum);
}


//----------------------------------学生事件模拟------------------------------
void StudentArrival()
{   
    //event.type==0,表示会有下一个学生到来，随机生成下一个学生在几秒钟后来，买饭时间
    int interval,duration;

    if(event.occurTime>=3600&&event.occurTime<=5400)
    {   
        interval=rand()%5;
        duration=rand()%15+15;
    }
    else
    {
        interval=rand()%30;
        duration=rand()%15+15;
    }

    if(event.occurTime+interval>=closeTime)//如果学生来的时候的时候已经关门了
    {   
        Transform(&hour,&minute,&second,event.occurTime+interval);
        printf("%d：%d：%d\t第%d位学生进入食堂，但食堂已经关门了\n",hour,minute,second,studentNum+1,event.occurTime+interval);
    }
    else
    {   
        //没有超时则生成学生
        studentNum++;
        student.arrivalTime=event.occurTime+interval;
        student.duration=duration;
        student.id=studentNum;
        
        //学生排入最短队中
        int min=GetMin(studentQueue);
        EnQueue(&studentQueue[min],student);

        Transform(&hour,&minute,&second,student.arrivalTime);
        printf("%d：%d：%d\t第%d位学生到达,排入%d号窗口前,打饭需要%d秒\n",hour,minute,second,student.id,min,student.duration);
        
        //如果这个时候队里面只有一个人，生成他的离开事件，插入eventList中，此时他的发生时间就是到达时间加上买饭时间
        if(Lenth(studentQueue[min])==1)
        {
            event.occurTime=student.arrivalTime+duration;
            event.type=min;
            Insert(&eventList,event);
        }
        //如果队里面不止一个人，那么下一个人的离开时间为上一个人的离开时间加上这个人办理业务的事件，具体在下面。
        //如果不是来了就买饭，而是排队，那么他的离开事件是当前一个人买饭结束时才生成并插入于eventList中

        //一个人到了之后，开始下一个同学到来的事件，但并不是说下一个同学马上要来，而是开始下一个同学到来时间，买饭时间的判定
        event.occurTime=student.arrivalTime;
        event.type=0;
        if(event.occurTime < closeTime)
        {
            Insert(&eventList,event);
        }
    }
}

void StudentDepature()
{   
    //如果是学生离开事件，那么由event.type得到离开的学生在哪一个队列中，将这一位同学的信息保存在student中，然后从队列中删除
    DelQueue(&studentQueue[event.type],&student);

    if(event.occurTime>closeTime)
    {
        printf("%d号窗口，排到第%d位同学，但食堂已经关门了\n",event.type,student.id);
        //要把这个队列里后面的人数减去。
        studentNum-=(Lenth(studentQueue[event.type])+1);
    }
    else
    {
        //总时间等于离开的时间减去这位同学来的时间
        totalTime+=event.occurTime-student.arrivalTime;
        Transform(&hour,&minute,&second,event.occurTime);
        printf("%d：%d：%d\t%d号窗口前，第%d位同学打饭离开，总用时%d秒\n",hour,minute,second,event.type,student.id,event.occurTime-student.arrivalTime);

        //如果这位同学后面有人，那么插入下一位同学的离开事件，那个事件发生时间为上一个同学的离开时间加上这位同学的打饭时间，将这位同学所在的队列保存在event.type里方便下一次从队列里面找到这位同学
        if(Lenth(studentQueue[event.type])>=1)
        {   
            event.occurTime=event.occurTime+studentQueue[event.type].front->next->duration;
            Insert(&eventList,event);
        }
    }
}


//----------------------------------链表操作------------------------------
//按时间大小将event插入eventList中
void Insert(EventList *pL,Event ev)
{
    Event *p=*pL;
    Event *q=p->next;

    Event* r=(Event *)malloc(sizeof(event));
    (*r)=ev;

    for(;q&&q->occurTime<event.occurTime;p=q,q=q->next);
    
    p->next=r;
    r->next=q;
}

//取第一个事件（按时间大小，而不是插入的先后），保存到event里，然后从eventList上删除
void DelEvent(EventList *pL,Event* ev)
{   
    Event* p=(*pL)->next;

    *ev=*p;

    (*pL)->next=(*pL)->next->next;

    free(p);
}


//----------------------------------队列操作------------------------------
void EnQueue(StudentQueue *pQ,Student stu)
{
    Student *p=(Student *)malloc(sizeof(Student));

    (*p)=stu;
    p->next=NULL;

    (*pQ).rear->next=p;
	(*pQ).rear=p;
}

void DelQueue(StudentQueue *pQ,Student* stu)
{   
    if((*pQ).front==(*pQ).rear)
    {
        return ;
    }

    (*stu)=*((*pQ).front->next);

    Student *p=(*pQ).front->next;

    (*pQ).front->next=(*pQ).front->next->next;

    if ( (*pQ).rear == p ) //删除一个后，队列变空了
    (*pQ).rear = (*pQ).front;

    free(p);
}

//得到队列长度，队列没有头节点
int Lenth(StudentQueue Q)
{
    Student *p=Q.front->next;

    int num=0;

    for(;p!=NULL;p=p->next,num++);

    return num;
}

//得到最短队列的编号，注意函数不要命名为short,short是类型，是个关键字，不能随便用
int GetMin(StudentQueue Q[])
{
    int min=1;

    for(int i=2;i<=WINDOWS_NUM;i++)
    {
        if(Lenth(Q[i])<Lenth(Q[min]))
        {
            min=i;
        }
    }

    return min;
}

//----------------------------------时间操作------------------------------
void Transform(int* hour,int *minute,int *second,int time)
{
    (*hour)=OpenHour+time/3600;
    (*minute)=OpenMin+(time%3600)/60;
    (*second)=OpenSecond+time%60;
}
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

//����ȫ�ֱ���
Event event;
EventList eventList;//�¼�Ҫ������ʱ��˳����������󵽣�Ҫʵ�ֲ��룬��˲�������
Student student;
StudentQueue studentQueue[WINDOWS_NUM+1];//�Ŷ�ʹ�ö��С�
int closeTime;
int studentNum=0;
int totalTime=0;
int hour=OpenHour;
int minute=OpenMin;
int second=OpenSecond;

//ʳ��ģ�⺯�����Լ�ʳ�ÿ��Ź��ź���
void CanteenSimulation();
void CanteenOpen();
void CanteenClose();

//�¼�ֻ�����֣�������һ��ѧ���ĵ������뿪
void StudentArrival();
void StudentDepature();

//�������
void Insert(EventList *pL,Event ev);
void DelEvent(EventList *pL,Event* ev);

//���в������ֱ�Ϊѧ����Ӻͳ���
void EnQueue(StudentQueue *pQ,Student stu);
void DelQueue(StudentQueue *pQ,Student* stu);
int Lenth(StudentQueue Q);
int GetMin(StudentQueue Q[]);

//ʱ�����
void Transform(int* hour,int *minute,int *second,int time);

int main()
{
    printf("������Ӫҵʱ��(�������)��\n>>>");
    scanf("%d",&closeTime);

    srand(time(NULL));

    CanteenSimulation();

    return 0;
}


//----------------------------------ʳ��ģ��------------------------------
void CanteenSimulation()
{
    CanteenOpen();

    while(eventList->next!=NULL)
    {
        DelEvent(&eventList,&event);//ȡ��eventList�ĵ�һ��������event��Ȼ��ɾ��
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
    //��ʼ����ͷ��������eventList
    eventList=(EventList)malloc(sizeof(Event));
    eventList->next=NULL;

    //ʳ�ÿ��ŵ�ͬʱ������һ���¼�������һλͬѧ�ĵ���
    event.type=0;
    event.occurTime=0;

    //�¼���������eventList��
    Insert(&eventList,event);

    //��ʼ�����У���ʱ�ĸ����У����д�����ͷ���
    for(int i=1;i<=WINDOWS_NUM;i++)
    {
        studentQueue[i].front=(Student*)malloc(sizeof(student));
        studentQueue[i].front->next=NULL;
        studentQueue[i].rear=studentQueue[i].front;
    }
}

void CanteenClose()
{
    printf("---------------------\n����%dλͬѧ�򷹣��Ŷ�������ʱ%d�룬ƽ���Ŷ���ʱ��%d��\n", studentNum, totalTime, totalTime/studentNum);
}


//----------------------------------ѧ���¼�ģ��------------------------------
void StudentArrival()
{   
    //event.type==0,��ʾ������һ��ѧ�����������������һ��ѧ���ڼ����Ӻ�������ʱ��
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

    if(event.occurTime+interval>=closeTime)//���ѧ������ʱ���ʱ���Ѿ�������
    {   
        Transform(&hour,&minute,&second,event.occurTime+interval);
        printf("%d��%d��%d\t��%dλѧ������ʳ�ã���ʳ���Ѿ�������\n",hour,minute,second,studentNum+1,event.occurTime+interval);
    }
    else
    {   
        //û�г�ʱ������ѧ��
        studentNum++;
        student.arrivalTime=event.occurTime+interval;
        student.duration=duration;
        student.id=studentNum;
        
        //ѧ��������̶���
        int min=GetMin(studentQueue);
        EnQueue(&studentQueue[min],student);

        Transform(&hour,&minute,&second,student.arrivalTime);
        printf("%d��%d��%d\t��%dλѧ������,����%d�Ŵ���ǰ,����Ҫ%d��\n",hour,minute,second,student.id,min,student.duration);
        
        //������ʱ�������ֻ��һ���ˣ����������뿪�¼�������eventList�У���ʱ���ķ���ʱ����ǵ���ʱ�������ʱ��
        if(Lenth(studentQueue[min])==1)
        {
            event.occurTime=student.arrivalTime+duration;
            event.type=min;
            Insert(&eventList,event);
        }
        //��������治ֹһ���ˣ���ô��һ���˵��뿪ʱ��Ϊ��һ���˵��뿪ʱ���������˰���ҵ����¼������������档
        //����������˾��򷹣������Ŷӣ���ô�����뿪�¼��ǵ�ǰһ�����򷹽���ʱ�����ɲ�������eventList��

        //һ���˵���֮�󣬿�ʼ��һ��ͬѧ�������¼�����������˵��һ��ͬѧ����Ҫ�������ǿ�ʼ��һ��ͬѧ����ʱ�䣬��ʱ����ж�
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
    //�����ѧ���뿪�¼�����ô��event.type�õ��뿪��ѧ������һ�������У�����һλͬѧ����Ϣ������student�У�Ȼ��Ӷ�����ɾ��
    DelQueue(&studentQueue[event.type],&student);

    if(event.occurTime>closeTime)
    {
        printf("%d�Ŵ��ڣ��ŵ���%dλͬѧ����ʳ���Ѿ�������\n",event.type,student.id);
        //Ҫ���������������������ȥ��
        studentNum-=(Lenth(studentQueue[event.type])+1);
    }
    else
    {
        //��ʱ������뿪��ʱ���ȥ��λͬѧ����ʱ��
        totalTime+=event.occurTime-student.arrivalTime;
        Transform(&hour,&minute,&second,event.occurTime);
        printf("%d��%d��%d\t%d�Ŵ���ǰ����%dλͬѧ���뿪������ʱ%d��\n",hour,minute,second,event.type,student.id,event.occurTime-student.arrivalTime);

        //�����λͬѧ�������ˣ���ô������һλͬѧ���뿪�¼����Ǹ��¼�����ʱ��Ϊ��һ��ͬѧ���뿪ʱ�������λͬѧ�Ĵ�ʱ�䣬����λͬѧ���ڵĶ��б�����event.type�﷽����һ�δӶ��������ҵ���λͬѧ
        if(Lenth(studentQueue[event.type])>=1)
        {   
            event.occurTime=event.occurTime+studentQueue[event.type].front->next->duration;
            Insert(&eventList,event);
        }
    }
}


//----------------------------------�������------------------------------
//��ʱ���С��event����eventList��
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

//ȡ��һ���¼�����ʱ���С�������ǲ�����Ⱥ󣩣����浽event�Ȼ���eventList��ɾ��
void DelEvent(EventList *pL,Event* ev)
{   
    Event* p=(*pL)->next;

    *ev=*p;

    (*pL)->next=(*pL)->next->next;

    free(p);
}


//----------------------------------���в���------------------------------
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

    if ( (*pQ).rear == p ) //ɾ��һ���󣬶��б����
    (*pQ).rear = (*pQ).front;

    free(p);
}

//�õ����г��ȣ�����û��ͷ�ڵ�
int Lenth(StudentQueue Q)
{
    Student *p=Q.front->next;

    int num=0;

    for(;p!=NULL;p=p->next,num++);

    return num;
}

//�õ���̶��еı�ţ�ע�⺯����Ҫ����Ϊshort,short�����ͣ��Ǹ��ؼ��֣����������
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

//----------------------------------ʱ�����------------------------------
void Transform(int* hour,int *minute,int *second,int time)
{
    (*hour)=OpenHour+time/3600;
    (*minute)=OpenMin+(time%3600)/60;
    (*second)=OpenSecond+time%60;
}
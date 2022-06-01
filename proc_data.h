#include <stdio.h>

typedef struct {
	int at;//arrival time
	int bt;//burst time
	int p;//pid
	int et;//first execute time
	int ct;//complete time
	int tat;//turnaround time
	int wt;//wait time
	int pri;//priority
	int rem;//remain burst time
	int c;//�Ϸ������� 0 �ƴϸ� -1
}PROC;
typedef struct{
	PROC* procs;
	int num_proc;//number of processes
	int tq;//time quantum
	//for gantt chart&readyQueue sim
	int* g_p;
	int* g_et;
	int* g_bt;
	int gantt_index;
	//result-total values
	int ttat;//turn_around time
	int twt;//total wait time
	int trt;//total response time
}DATA;
//���� ���Ϸκ��� �����͸� �о� ���μ��� �迭�� ä��� �Լ�
int Process_load(FILE* fp,int* num_proc,int* tq, PROC** procs);

//�Էµ� ���μ��� �迭�� ����
PROC* Copy_processes(PROC* procs,int num_proc);

//�Էµ� ���μ��� �迭�κ��� �����층 �Լ��� ���� ������ ���� �迭 ����
DATA* Make_dataIn(PROC* procArr,int num_proc,int tq);
//������ �����迭 ��ü ����(free)
void Destroy_data(DATA* datas);

//wait time, turnaround time �� �����ٸ� ������� ǥ�� ����ϴ� �Լ�
//twt, ttat, trt�� ����ϴ� ��� ����
void Print_table(DATA* datum);
//Gantt chart�� �׸��� �Լ�
void Print_gantt(DATA* datum);

void backspace(int a);        // ���ڸ��� �̻��� ���ڰ� ��µ� ��� backspace�� ����ϴ� �Լ�


void Print_readyQueue(DATA* datum);

int compare_a(const void* p1, const void* p2);//arrival ���� �� ������ ���� compare �Լ�
int compare_p(const void* p1, const void* p2);//pid������ �����ϱ����� compare �Լ�
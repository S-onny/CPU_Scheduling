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
	int c;//완료했으면 0 아니면 -1
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
int Process_load(FILE* fp,int* num_proc,int* tq, PROC** procs);

PROC* Copy_processes(PROC* procs,int num_proc);

DATA* Make_dataIn(PROC* procArr,int num_proc,int tq);
void Destroy_data(DATA* datas);

//wait time, turnaround time 등 스케줄링 결과값을 표로 출력하는 함수
//twt, ttat, trt를 계산하는 기능 포함
void Print_table(DATA* datum);
//Gantt chart를 그리는 함수
void Print_gantt(DATA* datum);

void backspace(int a);        // 두자릿수 이상의 숫자가 출력될 경우 backspace를 출력하는 함수


void Print_readyQueue(DATA* datum);

int compare_a(const void* p1, const void* p2);//arrival 순서 퀵 정렬을 위한 compare 함수
int compare_p(const void* p1, const void* p2);//pid순으로 정렬하기위한 compare 함수
#pragma once
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
//열린 파일로부터 데이터를 읽어 프로세스 배열을 채우는 함수
int Process_load(FILE* fp,int* num_proc,int* tq, PROC** procs);

//입력된 프로세스 배열을 복사
PROC* Copy_processes(PROC* procs, int num_proc);

//입력된 프로세스 배열로부터 스케쥴링 함수에 넣을 데이터 구조 배열 생성
DATA* Make_dataIn(PROC* procArr,int num_proc,int tq);
//데이터 구조배열 전체 제거(free)
void Destroy_data(DATA* datas);

//wait time, turnaround time 등 스케줄링 결과값을 표로 출력하는 함수
//twt, ttat, trt를 계산하는 기능 포함
void Print_table(DATA* datum);
//Gantt chart를 그리는 함수
void Print_gantt(DATA* datum);


int compare_a(const void* p1, const void* p2);//arrival 순서 퀵 정렬을 위한 compare 함수
int compare_p(const void* p1, const void* p2);//pid순으로 정렬하기위한 compare 함수

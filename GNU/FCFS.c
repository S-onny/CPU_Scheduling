#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#include "Print_readyqueue.h"
#pragma warning(disable:4996)
#define swap(x, y, temp) ( (temp)=(x), (x)=(y), (y)=(temp) )        // 배열 요소의 순서를 바꾸기 위한 함수




void FCFS(DATA* data)
{
	int num_proc = data->num_proc;
	PROC* procs = (data->procs);
	int* g_p = (int*)malloc(sizeof(int)*num_proc);//간트차트를 그리기 위한 동적배열들
	int* g_et = (int*)malloc(sizeof(int)*num_proc);//간트차트를 그리기 위한 동적배열들
	int* g_bt = (int*)malloc(sizeof(int)*num_proc);//간트차트를 그리기 위한 동적배열들
	int i, j, tq, e, min, temp, n, m, k;

	double ttat = 0, twt = 0;
	// num_proc = number of processes
	// at = arrival time
	// bt = burst time
	// p = 프로세스 번호 저장
	// et = excution time 프로세스 실행이 시작되는 시간
	// ct = completion time 프로세스 실행이 끝나는 시간
	// tat = turnaround time
	// wt = waiting time
	// ttat = total turnaround time
	// twt = total waiting time
	// pri = priority
	// tq = time quantum
	// e = 마지막에 프로그램 실행 or 종료 


	qsort(procs, num_proc, sizeof(PROC), compare_a);//도착 순서대로 정렬(같을시 프로세스 번호대로)

		


		for (i = 0; i < num_proc; i++) // 도착시간을 실행시간에 저장
		{
			procs[i].et = procs[i].at;
			
		}

		for (i = 1; i < num_proc; i++)     // n번째 도착한 프로세스의 실행시간 = n-1번째 도착한 프로세스의 실행시간(or 도착시간) + burst time
		{
			j = i - 1;
			if (procs[j].et <= procs[i].et && procs[i].et < procs[j].et + procs[j].bt)
			{
				procs[i].et = procs[j].et + procs[j].bt;
			}
			else if (procs[i].et < procs[j].et)       // n번째 프로세스가 n-1번째 프로세스보다 일찍 도착했고, 다른 프로세스가 실행중일때 대기중이였던 경우
			{
				procs[i].et = procs[j].et + procs[j].bt;
			}
		}

		for (i = 0; i < num_proc; i++) // completion time 구하기
		{
			procs[i].ct = procs[i].et + procs[i].bt;
			g_p[i] = procs[i].p;
			g_et[i] = procs[i].et;
			g_bt[i] = procs[i].bt;
		}

		for (i = 0; i < num_proc; i++) // turnaround time, waiting time, total turnaround time, total waiting time 구하기
		{
			procs[i].tat = procs[i].ct - procs[i].at;
			procs[i].wt = procs[i].et - procs[i].at;
			if (procs[i].wt < 0)              // n번째 프로세스가 n-1번째 프로세스의 실행이 끝난 다음에 왔을 경우 wt가 음수로 나오는 오류 고침
				procs[i].wt = 0;
		}

		qsort(procs, num_proc, sizeof(PROC), compare_p);//procs를 다시 pid순으로 정렬
		(data->g_p) = g_p;
		(data->g_et) = g_et;
		(data->g_bt) = g_bt;
		(data->gantt_index) = num_proc-1;


		Print_gantt(data);
		Print_table(data);



		printf("\n\n\n");

		// Ready queue table    

		Print_readyQueue(data);
		// Ready queue table 끝
		printf("\n\n\n");



	return;
}

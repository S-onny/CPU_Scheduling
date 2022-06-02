#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_data.h"

PROC* Process_load(FILE* fp, int* num_proc, int* tq) {
	PROC* procs;
	int ret;
	char discard;
	ret = fscanf(fp, "%d", num_proc);
	if (ret == EOF)
	{
		printf("FILE error:invaild format or empty!");
		return NULL;
	}
	printf("number of processes:%d\n", *num_proc);
	procs = (PROC*)malloc(sizeof(PROC) * (*num_proc));

	for (int i = 0; i < (*num_proc); i++)
	{
		ret = fscanf(fp, " %c%d%d%d%d", &discard, &(procs[i].p), &(procs[i].at), &(procs[i].bt), &(procs[i].pri));
		if (ret == EOF)
		{
			printf("FILE error:not enough processes or invaild format");
			return NULL;
		}
		printf("reading process P%d...\n", procs[i].p);
		printf(" %d %d %d\n", procs[i].at, procs[i].bt, procs[i].pri);

		procs[i].rem = procs[i].bt;//남은시간 burst time으로 초기화
		procs[i].wt = 0;//기타 필요한 수치들 초기화
		procs[i].et = 0 - 1;//최초 실행시간 구분을 위해 -1로 초기화
		procs[i].ct = 0;
		procs[i].tat = 0;
		procs[i].c = -1;
	}
	ret = fscanf(fp, "%d", tq);
	if (ret == EOF)
	{
		printf("FILE error:No Time quantum!");
		return NULL;
	}
	printf("tq:%d\n", *tq);
	return procs;


}

PROC* Copy_processes(PROC* procs, int num_proc) {
	PROC* temp = (PROC*)malloc(sizeof(PROC)*num_proc);
	memcpy(temp, procs, sizeof(PROC) * num_proc);
	return temp;
}
DATA* Make_dataIn(PROC* procArr, int num_proc, int tq) {
	DATA* DataArr = (DATA*)malloc(sizeof(DATA) * 7);
	DATA temp;
	for (int i = 0; i < 7; i++) {
		temp.num_proc = num_proc;
		temp.tq = tq;
		temp.procs = Copy_processes(procArr, num_proc);
		temp.g_p = NULL;
		temp.g_et = NULL;
		temp.g_bt = NULL;
		temp.gantt_index = -1;
		DataArr[i] = temp;
	}
	return DataArr;
}

void Destroy_data(DATA* datas) {
	for (int i = 0; i < 7; i++) {
		printf("Destroying %d's procs\n", i);
		free(datas[i].procs);
		if (datas[i].g_p != NULL) {
			printf("Destroying %d'sGantt\n", i);
			free(datas[i].g_p);
			free(datas[i].g_et);
			free(datas[i].g_bt);
		}

	}
	free(datas);
}
void Print_table(DATA* datum) {
	int np = datum->num_proc;
	datum->twt = 0;
	datum->ttat = 0;
	datum->trt = 0;
	int* rt=(int*)malloc(sizeof(int)*np);
	qsort(datum->procs, np, sizeof(PROC), compare_p);//procs를 다시 pid순으로 정렬

	for (int i = 0; i < np; i++) {//twt ttat 계산
		datum->twt += datum->procs[i].wt;
		datum->ttat += datum->procs[i].tat;
		rt[i] = datum->procs[i].et - datum->procs[i].at;
		datum->trt += rt[i];

	}
	printf("------------------------------------------------------------------------------------------------\n");
	printf(" Processes\tArrival\tBurst\tPriority\tCompletion\tWaiting\tTurnaround\tResponse\n");
	printf("------------------------------------------------------------------------------------------------\n");
	for (int i = 0; i < np; i++)
	{
		printf(" P[%d]\t\t[%d]\t[%d]\t[%d]\t\t[%d]\t\t[%d]\t[%d]\t\t[%d]\n", datum->procs[i].p, datum->procs[i].at, datum->procs[i].bt, datum->procs[i].pri, datum->procs[i].ct, datum->procs[i].wt, datum->procs[i].tat, rt[i]);

	}
	printf("------------------------------------------------------------------------------------------------\n");
	printf("Average waiting time : %.2f\n", (float)datum->twt / np);
	printf("Average turnaround time : %.2f\n", (float)datum->ttat / np);
	printf("Average response time : %.2f\n", (float)datum->trt / np);
}
void Print_gantt(DATA* datum) {

	int gt = datum->gantt_index;
	int n;
	// 간트 차트 그리기
	printf("\n\n");
	printf("Gantt Chart");
	printf("\n");

	for (int i = 1; i <= gt + 1; i++) // 윗줄
	{
		printf(" ----");
		n = i - 1;
		for (int j = 0; j < datum->g_bt[n]; j++)
		{
			printf("-");
		}



		if (i != gt + 1 && datum->g_et[i] > datum->g_et[n] + datum->g_bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 윗줄 "-" 
		{

			printf(" -");
			for (int k = 0; k < datum->g_et[i] - (datum->g_et[n] + datum->g_bt[n]); k++)
			{
				printf("-");
			}
		}
	}





	printf("\n");

	for (int i = 1; i <= gt + 1; i++) // 가운데 줄
	{
		n = i - 1;

		printf("| P%d ", datum->g_p[n]);

		for (int j = 0; j < datum->g_bt[n]; j++)
		{
			printf(" ");
		}


		if (i != gt + 1 && datum->g_et[i] > datum->g_et[n] + datum->g_bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 가운데 줄의 공백
		{
			printf("| ");
			for (int k = 0; k < (datum->g_et[i] - (datum->g_et[n] + datum->g_bt[n])); k++)
			{
				printf(" ");
			}
		}
	}



	printf("|");
	printf("\n");

	for (int i = 1; i <= gt + 1; i++) // 아랫줄
	{
		printf(" ----");

		for (int j = 0; j < datum->g_bt[i - 1]; j++)
		{
			printf("-");
		}

		n = i - 1;
		if (i != gt + 1 && datum->g_et[i] > datum->g_et[n] + datum->g_bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 아랫줄 "-"
		{
			printf(" -");
			for (int k = 0; k < (datum->g_et[i] - (datum->g_et[n] + datum->g_bt[n])); k++)
			{
				printf("-");
			}
		}
	}


	printf("\n");

	int a, b, c, d, e, f;
	for (int i = 1; i <= gt + 1; i++) // 아랫줄 밑 실행시간
	{
		printf("%d    ", datum->g_et[i - 1]);
		for (int j = 0; j < datum->g_bt[i - 1]; j++)
		{
			printf(" ");
		}

		a = datum->g_et[i - 1];                                  // 자릿수만큼 공백 빼기
		b = 0;
		while (a != 0)
		{
			a = a / 10;
			++b;
		}

		for (c = 0; c < b - 1; c++)
		{
			printf("\b");
		}

		n = i - 1;                              // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간 밑의 실행시간 사이의 공백
		if (i != gt + 1 && datum->g_et[i] > datum->g_et[n] + datum->g_bt[n])
		{
			printf("%d ", datum->g_et[n] + datum->g_bt[n]);
			for (int k = 0; k < (datum->g_et[i] - (datum->g_et[n] + datum->g_bt[n])); k++)
			{
				printf(" ");
			}

			d = datum->g_et[n] + datum->g_bt[n];                              // 자릿수만큼 공백 빼기
			e = 0;
			while (d != 0)
			{
				d = d / 10;
				++e;
			}

			for (f = 0; f < e - 1; f++)
			{
				printf("\b");
			}

		}

	}


	printf("%d", datum->g_et[gt] + datum->g_bt[gt]);
	printf("\n");
}

int compare_a(const void* p1, const void* p2) {//arrival 순서 퀵 정렬을 위한 compare 함수
	int proc_cmp;
	proc_cmp = ((PROC*)p1)->at - ((PROC*)p2)->at;
	if (proc_cmp == 0)
		return ((PROC*)p1)->p - ((PROC*)p2)->p;
	return proc_cmp;
}

int compare_p(const void* p1, const void* p2) {//pid순으로 정렬하기위한 compare 함수
	return ((PROC*)p1)->p - ((PROC*)p2)->p;
}

void push(int *front, int *rear, int max, int value, int queue[])
{
	*rear = (*rear + 1) % max;
	queue[*rear] = value;
}

int pop(int *front, int *rear, int max, int queue[])
{
	if (*rear == *front)
		return -1;
	*front = (*front + 1) % max;

	return queue[*front];
}

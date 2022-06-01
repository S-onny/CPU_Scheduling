#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_data.h"

int Process_load(FILE* fp, int* num_proc, int* tq, PROC** procs) {
	PROC temp;
	int ret;
	ret = fscanf(fp, "%d", num_proc);
	if (ret == EOF)
	{
		printf("FILE error:invaild format or empty!");
		return 0;
	}
	printf("number of processes:%d\n",*num_proc);
	*procs = (PROC*)realloc(*procs, sizeof(PROC)*(*num_proc));
	char discard;
	for (int i = 0; i<(*num_proc); i++)
	{
		ret = fscanf(fp, " %c%d%d%d%d",&discard, &(temp.p), &(temp.at), &(temp.bt), &(temp.pri));
		if (ret == EOF)
		{
			printf("FILE error:not enough processes or invaild format");
			return 0;
		}
		printf("reading process P%d...\n", temp.p);
		printf(" %d %d %d\n", temp.at, temp.bt, temp.pri);
		(*procs)[i] = temp;
		(*procs)[i].rem = (*procs)[i].bt;//남은시간 burst time으로 초기화
		(*procs)[i].wt = 0;//기타 필요한 수치들 초기화
		(*procs)[i].et = 0 - 1;//최초 실행시간 구분을 위해 -1로 초기화
		(*procs)[i].ct = 0;
		(*procs)[i].tat = 0;
		(*procs)[i].c = -1;
	}
	ret = fscanf(fp, "%d\n", tq);
	if (ret == EOF)
	{
		printf("FILE error:No Time quantum!");
		return 0;
	}
	printf("tq:%d",*tq);
	return 1;


}

PROC* Copy_processes(PROC* procs, int num_proc) {
	PROC* temp = (PROC*)malloc(sizeof(PROC)*num_proc);
	temp=(PROC*)memcpy(temp, procs, sizeof(PROC)*num_proc);
	return temp;
}
DATA* Make_dataIn(PROC* procArr, int num_proc,int tq) {
	DATA* DataArr = (DATA*)malloc(sizeof(DATA) * 7);
	for (int i = 0; i<7; i++) {
		DataArr[i].num_proc = num_proc;
		DataArr[i].tq = tq;
		DataArr[i].procs = Copy_processes(procArr, num_proc);
		DataArr[i].g_p = NULL;
		DataArr[i].g_et = NULL;
		DataArr[i].g_bt = NULL;
		DataArr[i].gantt_index = 0;

	}
	return DataArr;
}

void Destroy_data(DATA* datas) {
	for (int i = 0; i<7; i++) {
		if (datas[i].procs != NULL) free(datas[i].procs);
		if (datas[i].g_p != NULL) {
			free(datas[i].g_p);
			free(datas[i].g_et);
			free(datas[i].g_bt);
		}
		
	}
	free(datas);
}
void Print_table(DATA* datum) {
	int np = datum->num_proc;
	qsort(datum->procs, np, sizeof(PROC), compare_p);//procs를 다시 pid순으로 정렬
	
	for (int i = 0; i < np; i++) {//twt ttat 계산
		datum->twt += datum->procs[i].wt;
		datum->ttat += datum->procs[i].tat;
	}
	printf("-------------------------------------------------------------------\n");
	printf(" Processes\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
	printf("-------------------------------------------------------------------\n");
	for (int i = 0; i < np; i++)
	{
		printf(" Process[%d]\t[%d]\t[%d]\t[%d]\t\t[%d]\t[%d]\n", datum->procs[i].p, datum->procs[i].at, datum->procs[i].bt, datum->procs[i].ct, datum->procs[i].wt, datum->procs[i].tat);

	}
	printf("-------------------------------------------------------------------\n");
	printf("Average waiting time : %.2f\n", datum->twt / (float)np);
	printf("Average turnaround time : %.2f\n", datum->ttat / (float)np);
}
void Print_gantt(DATA* datum) {

	int gt = datum->gantt_index;
	int n;
	// 간트 차트 그리기
	printf("\n\n");
	printf("Gantt Chart %d %d", datum->gantt_index, datum->g_p[datum->gantt_index]);
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

	for (int i = 1; i <= gt+ 1; i++) // 아랫줄
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
void backspace(int a)        // 두자릿수 이상의 숫자가 출력될 경우 backspace를 출력하는 함수
{
    int x, y, z;
    x = a;
    y = 0;

    while (x != 0)
    {
        x = x / 10;
        y++;
    }

    for (z = 0; z < y - 1; z++)
    {
        printf("\b");
    }
}


void Print_readyQueue(DATA* datum){
	         // Ready queue table    

        printf("Ready queue table\n");
		
        printf(" ------- ------------- ------------ \n");
        printf("| Time  | Ready queue | Remaintime |\n");
        printf(" ------- ------------- ------------ \n");
		int i,j,a,c,d,e;
		int gi=datum->gantt_index;
		int* p=datum->g_p;
		int* et=datum->g_et;
		int* bt=datum->g_bt;
		int ct[(datum->gantt_index)+1];
		int pat[datum->num_proc];
		int pct[datum->num_proc];
		int pp[datum->num_proc];
		int prem[datum->num_proc];
		for (i=0;i<=(gi);i++){
			ct[i]=bt[i]+et[i];
		}
		for (i=0;i<datum->num_proc;i++){
			pat[i]=(datum->procs)[i].at;
			pct[i]=(datum->procs)[i].ct;
			pp[i]=(datum->procs)[i].p;
			prem[i]=(datum->procs)[i].bt;
		}

        for (a = 0; a <= ct[gi]; a++)             // 마지막 프로세스가 실행 종료될때까지의 시간 a
        {
            c = -1, e=-1;
            d = 0;
			for(j=0;j<=gi;j++){// 시간 a 일때 실행중인프로세스 pid찾기
					if (et[j] <= a && a <= ct[j]){
						c=p[j];
						e=j;
					}
				}
			printf("|  %d    ", a);
			backspace(a);
            for (i = 0; i < (datum->num_proc); i++)                  // 시간 a일때 대기중인 프로세스들을 출력하는 알고리즘
            {
				if(pp[i] == c) {
					prem[i] --;
					continue;
				}
                if (a >= pat[i] && a < pct[i])       // 시간 a 일때 도착+ 종료안됨+대기중이였던 프로세스들의 개수
                {
                    d++;
					if (d>1){
						printf("|       ");
					}
					printf("|     P%d      ", pp[i]);
                    backspace(p[e]);
					
                    printf("|     %d      ", prem[i]);
                    backspace(prem[i]);
                    printf("|\n");
                }
				
            }

            if (d == 0)                 // 대기중인 프로세스가 없을때
            {
                printf("|             |            |\n");
			}
			printf(" ------- ------------- ------------ \n");
        }
        // Ready queue table 끝
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

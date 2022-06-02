#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#include "Print_readyqueue.h"
#pragma warning(disable:4996)
#define swap(x, y, temp) ( (temp)=(x), (x)=(y), (y)=(temp) )        // 배열 요소의 순서를 바꾸기 위한 함수




void SJF(DATA* data)
{

	int num_proc = data->num_proc;
	PROC* procs = (data->procs);
	int* g_p = (int*)malloc(sizeof(int)*num_proc);//간트차트를 그리기 위한 동적배열들
	int* g_et = (int*)malloc(sizeof(int)*num_proc);//간트차트를 그리기 위한 동적배열들
	int* g_bt = (int*)malloc(sizeof(int)*num_proc);//간트차트를 그리기 위한 동적배열들

	int  i, j, tq, e, min, temp, n, m, k;

	double ttat = 0, twt = 0;
	int a, b, c, d, f, g, h; // 기타 변수들
	int count = 0;
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




		for (i = 0; i < num_proc - 1; i++)              // 같은 시간에 도착한 프로세스들을 bt에 대해서 정리
		{
			count = 0;
			for (j = i + 1; j < num_proc; j++)
			{
				if (procs[i].at == procs[j].at)                    // 같은 시간에 도착한 프로세스들의 개수
				{
					count++;
				}
			}


			if (count != 0)
			{
				c = i + count;
				for (a = i; a < c; a++)         // i에서부터 i + count까지의 프로세스들을 bt에 대해서 정리
				{                               // 이때 count는 같은 시간에 도착한 프로세스의 개수이다.
					min = a;
					for (b = a + 1; b <= c; b++)
					{


						if (procs[min].bt > procs[b].bt)
						{
							min = b;
						}
					}

					if (a != min)
					{
						swap(procs[a].at, procs[min].at, temp);
						swap(procs[a].p, procs[min].p, temp);
						swap(procs[a].bt, procs[min].bt, temp);
						swap(procs[a].et, procs[min].et, temp);
					}
				}
			}
			i = i + count;             // 정리가 끝난 프로세스들의 다음부터 다시 정리 시작
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




		for (i = 0; i < num_proc - 1; i++)              // 대기중인 프로세스들을 bt에 대해서 정리하기
		{
			count = 0;
			for (j = i + 1; j < num_proc; j++)
			{
				if (procs[i].et <= procs[j].at && procs[j].at < procs[i].et + procs[i].bt)         // i 번째 프로세스가 실행중이여서 대기중인 프로세스들의 개수 세기
				{
					count++;
				}
			}


			if (count != 0)                 // 대기중인 프로세스의 개수가 0이 아닐때, 그 프로세스들을 bt에 대해서 정리
			{
				c = i + count;
				d = i;


				b = 0; // 아래 세가지 조건 중 해당하는 것이 있는지 알아보기 위한 변수
				if (d == 0)     // 조건1. 가장 첫번째 프로세스
				{
					b++;
				}

				if (procs[d].et == procs[d+1].et)     // 조건2. 같은 시간에 도착한 프로세스가 있을 경우, 그 중 첫번째 프로세스
				{
					b++;
				}

				if (procs[d+1].et >= procs[d].et + procs[d].bt)     // 조건3. 바로 앞 프로세스가 종료된 후에 도착한 프로세스
				{
					b++;
				}

				if (b != 0)             // 위 세가지 조건 중 하나라도 해당되면 그 프로세스 제외
				{
					d = d + 1;
				}


				for (a = d; a < c; a++)       // 대기중인 프로세스들을 bt에 대해서 정리
				{
					min = a;
					for (b = a + 1; b <= c; b++)
					{
						if (procs[min].bt > procs[b].bt)
						{
							min = b;
						}
					}

					if (a != min)
					{
						swap(procs[a].at, procs[min].at, temp);
						swap(procs[a].p, procs[min].p, temp);
						swap(procs[a].bt, procs[min].bt, temp);
						swap(procs[a].et, procs[min].et, temp);
					}
				}
			}
			i = i + count;               // 정리가 끝난 프로세스들의 다음부터 다시 정리 시작
		}


		for (i = 1; i < num_proc; i++)     // bt에 대해서 정리한 프로세스들의 et를 다시 정리
		{
			j = i - 1;
			if (procs[j].et <= procs[i].at && procs[i].at < procs[j].et + procs[j].bt)
			{
				procs[i].et = procs[j].et + procs[j].bt;
			}
			else if (procs[i].at < procs[j].et)
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
			if (procs[i].wt < 0)              // n번째 프로세스가 n-1번째 프로세스의 실행이 끝난 다음에 왔을 경우 wt가 음수로 나오는것을 0으로 표시
				procs[i].wt = 0;
		}
		qsort(procs, num_proc, sizeof(PROC), compare_p);//procs를 다시 pid순으로 정렬
		(data->g_p) = g_p;
		(data->g_et) = g_et;
		(data->g_bt) = g_bt;
		(data->gantt_index) = num_proc-1;

		

		// 간트 차트 그리기
		Print_gantt(data);
		// 간트 차트 끝
		Print_table(data);
	
		printf("\n\n\n");

		// Ready queue table    

		Print_readyQueue(data);
		// Ready queue table 끝
		printf("\n\n\n");

	return ;
}

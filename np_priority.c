#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#include "Print_readyqueue.h"
#define INT_MAX	2147483647//int의 최댓값 define

void np_priority(DATA* data)
{
	int num_proc = data->num_proc;
	PROC* procs = (data->procs);
	int n, gantt_index, time;
	double ttat = 0, twt = 0;
	// ijkn: 반복문을 위한 변수, gantt_index:간트차트를 그리기 위한 배열들의 index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// tq = time quantum
	// time: 프로세싱 시각
	// E:무한루프 탈출
	int* g_p = (int*)malloc(sizeof(int));//간트차트를 그리기 위한 동적배열들
	int* g_et = (int*)malloc(sizeof(int));//간트차트를 그리기 위한 동적배열들
	int* g_bt = (int*)malloc(sizeof(int));//간트차트를 그리기 위한 동적배열들
	int min_pri = INT_MAX;//실행가능한 프로세스 중 가장 짧은 남은시간
	int min_pri_index = -1;// 실행가능한 프로세스중 가장 remain time이 짧은 프로세스의 index

	int index = -1;


		qsort(procs, num_proc, sizeof(PROC), compare_a);//도착 순서대로 정렬(같을시 프로세스 번호대로)
		printf("\n\n");

		time = 0;
		gantt_index = 0;
		int N = num_proc;//N:실행이 끝나지 않은 프로세스 수. 입력된 프로세스수를 복사하여 초기화
		while (N > 0) { //종료조건: line136 

			if (index == -1) {
				min_pri = INT_MAX;//최솟값을 찾기 위해 무한 대신 int가 가질 수 있는 최곳값으로 초기화
				min_pri_index = -1;// -1로 초기화. 현재 상태가 idle인지 구분하기 위함.
				for (int i = 0; i < N; i++) {//priority 가장 높은 거 찾기
					if (procs[i].at > time) break;
					if (procs[i].pri < min_pri) {
						min_pri_index = i;
						min_pri = procs[i].pri;
					}
				}
				if (min_pri_index == -1) {//현재 실행가능한 프로세스 없음(idle)
										  /*//이부분은 다른형태의 간트차트를 그리기 위한 예비 코드임
										  if(gantt_index==0&&g_bt[j]==0){
										  g_p[gantt_index]=0;
										  g_et[gantt_index]=time;
										  }
										  else{
										  if (g_p[j] != 0) {
										  if (sizeof(g_p)/(sizeof(int)) < gantt_index+2)
										  g_p=(int*)realloc(g_p,sizeof(g_p)*2);
										  g_et=(int*)realloc(g_et,sizeof(g_et)*2);
										  g_bt=(int*)realloc(g_bt,sizeof(g_bt)*2);
										  gantt_index++;
										  g_p[gantt_index]=0;
										  g_et[gantt_index]=time;
										  }
										  }
										  (g_bt[gantt_index])++;
										  */
					time++;//시간흐름
					continue;
				}
				g_p[gantt_index] = procs[min_pri_index].p;
				g_bt[gantt_index] = 0;
				g_et[gantt_index] = time;

				g_p = (int*)realloc(g_p, (2 + gantt_index) * sizeof(int));
				g_et = (int*)realloc(g_et, (2 + gantt_index) * sizeof(int));
				g_bt = (int*)realloc(g_bt, (2 + gantt_index) * sizeof(int));
				index = 0;
			}

			for (int i = 0; i < N; i++) {//실행이 끝나지 않은 프로세스중 실행되지 않은 프로세스들의 대기시간 증가
				if (procs[i].at > time) break;
				if (i == min_pri_index)
					continue;
				(procs[i].wt)++;
			}
			(procs[min_pri_index].rem)--;//실행중인 프로세스의 remain time 감소
			(g_bt[gantt_index])++;

			if (procs[min_pri_index].et == -1) procs[min_pri_index].et = time;//프로세스가 최초로 실행된 시간을 et에 저장
			if (procs[min_pri_index].rem == 0)
			{	//프로세스가 실행종료된 경우: 가장 마지막 index의 프로세스와 현재 프로세스 swap.
				procs[min_pri_index].ct = time + 1;
				procs[min_pri_index].tat = procs[min_pri_index].ct - procs[min_pri_index].at;
				PROC temp = procs[N - 1];
				procs[N - 1] = procs[min_pri_index];
				procs[min_pri_index] = temp;
				N = N - 1;//실행가능한 프로세스 수 감소
				if (N == 0) {//모든 프로세스가 실행종료된 경우 반복문 탈출
					time++;
					break;
				}
				gantt_index++;
				index = -1;
				qsort(procs, N, sizeof(PROC), compare_a);//실행가능한 프로세스들을 다시 arrival순서로 정렬.
			}

			time++;//시간이 흐름
		}
		//프로세싱 종료

		qsort(procs, num_proc, sizeof(PROC), compare_p);//procs를 다시 pid순으로 정렬

		(data->g_p) = g_p;
		(data->g_et) = g_et;
		(data->g_bt) = g_bt;
		(data->gantt_index) = gantt_index;

		Print_table(data);

		// 간트 차트 그리기
		Print_gantt(data);
		
		// 간트 차트 끝
	printf("\n\n\n");
	Print_readyQueue(data);

		


}

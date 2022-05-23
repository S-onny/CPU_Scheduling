#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"

int compare_a(const void* p1, const void* p2);
int compare_p(const void* p1, const void* p2);
void push(int* front, int* rear, int max, int value, int queue[]) {

	*rear = (*rear + 1) % max;
	queue[*rear] = value;
}
int pop(int* front, int* rear, int max, int queue[]) {
	if (*rear == *front)
		return -1;
	*front = (*front + 1) % max;

	return queue[*front];
}

void RR(PROC* procs,int num_proc ,int tq)
{

	int n, gantt_index, time;
	double ttat = 0, twt = 0;
	// ijkn: 반복문을 위한 변수, gantt_index:간트차트를 그리기 위한 배열들의 index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// time: 프로세싱 시각
	// E:무한루프 탈출
	int front = -1;
	int rear = -1;
	int* queue = (int*)malloc((num_proc+1) * sizeof(int)); //대기큐
	int* g_p = (int*)malloc( sizeof(int));//간트차트를 그리기 위한 동적배열들
	int* g_et = (int*)malloc( sizeof(int));//간트차트를 그리기 위한 동적배열들
	int* g_bt = (int*)malloc(sizeof(int));//간트차트를 그리기 위한 동적배열들
	int index = 0;		//Pop여부에 대한 인덱스
	int queue_index = -1;	//현재 CPU점유중인 프로세스 인덱스
	int at_index = 0;	//도착한 프로세스 인덱스

	qsort(procs, num_proc, sizeof(PROC), compare_a); //도착 순서대로 정렬(같을시 프로세스 번호대로)


		time = 0;
		gantt_index = 0;
		int N = num_proc;//N:실행이 끝나지 않은 프로세스 수. 입력된 프로세스수를 복사하여 초기화

		if (at_index < num_proc && 0 == procs[at_index].at) {	//처음 도착한 프로세스 푸쉬
			push(&front, &rear, num_proc + 1, at_index, queue);
			at_index++;

		}

		while (N > 0) { //종료조건: line136 

			if (index == 0) {	//인덱스가 0이면 pop하고 초기화
			
				queue_index = pop(&front, &rear, num_proc + 1, queue);
				
				if (queue_index == -1) {//현재 실행가능한 프로세스 없음(idle)
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
				if (procs[queue_index].p == g_p[gantt_index - 1]) {	//전 프로세스랑 현 프로세스 동일하면 간트인덱스 유지
					gantt_index--;
				}
				else {
					g_p[gantt_index] = procs[queue_index].p;
					g_bt[gantt_index] = 0;
					g_et[gantt_index] = time;
				}
				
				g_p = (int*)realloc(g_p, (2 + gantt_index) * sizeof(int));
				g_et = (int*)realloc(g_et, (2 + gantt_index) * sizeof(int));
				g_bt = (int*)realloc(g_bt, (2 + gantt_index) * sizeof(int));
				index = 0;
			}

			for (int i = 0; i < num_proc; i++) {	//실행이 끝나지 않은 프로세스중 실행되지 않은 프로세스들의 대기시간 증가
				if (i != queue_index && procs[i].at <= time && (procs[i].c == -1))
					(procs[i].wt)++;
			}
			(procs[queue_index].rem)--;//실행중인 프로세스의 remain time 감소
			(g_bt[gantt_index])++;

			if (at_index < num_proc && time + 1 == procs[at_index].at) {	//도착한 프로세스 푸쉬
				push(&front, &rear, num_proc + 1, at_index, queue);
				at_index++;

			}

			if (procs[queue_index].et == -1) procs[queue_index].et = time;//프로세스가 최초로 실행된 시간을 et에 저장
			if (procs[queue_index].rem == 0)	//실행완료되어 프로세스 바꿔주기
			{
				procs[queue_index].ct = time + 1;
				procs[queue_index].tat = procs[queue_index].ct - procs[queue_index].at;
				procs[queue_index].c = 0;
				N = N - 1;//실행가능한 프로세스 수 감소
				if (N == 0) {//모든 프로세스가 실행종료된 경우 반복문 탈출
					time++;
					break;
				}
				gantt_index++;
				index = 0;


			}
			else if (tq == index + 1)	//time-quantom에 의해 프로세스 바꿔주기
			{

				push(&front, &rear, num_proc + 1, queue_index, queue);

				gantt_index++;
				index = 0;

			}
			else
				index++;

			time++;//시간이 흐름

		}
		//프로세싱 종료

		qsort(procs, num_proc, sizeof(PROC), compare_p);//procs를 다시 pid순으로 정렬

		for (int i = 0; i < num_proc; i++) {//twt ttat 계산
			twt += procs[i].wt;
			ttat += procs[i].tat;
		}

		//print table
		printf("-------------------------------------------------------------------\n");
		printf(" Processes\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
		printf("-------------------------------------------------------------------\n");
		for (int i = 0; i < num_proc; i++)
		{
			printf(" Process[%d]\t[%d]\t[%d]\t[%d]\t\t[%d]\t[%d]\n", procs[i].p, procs[i].at, procs[i].bt, procs[i].ct, procs[i].wt, procs[i].tat);

		}
		printf("-------------------------------------------------------------------\n");
		printf("Average waiting time : %.2f\n", twt / num_proc);
		printf("Average turnaround time : %.2f\n", ttat / num_proc);


		// 간트 차트 그리기
		printf("\n\n");
		printf("Gantt Chart %d %d", gantt_index, g_p[gantt_index]);
		printf("\n");

		for (int i = 1; i <= gantt_index + 1; i++) // 윗줄
		{
			printf(" ----");
			n = i - 1;
			for (int j = 0; j < g_bt[n]; j++)
			{
				printf("-");
			}



			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 윗줄 "-" 
			{

				printf(" -");
				for (int k = 0; k < g_et[i] - (g_et[n] + g_bt[n]); k++)
				{
					printf("-");
				}
			}
		}





		printf("\n");

		for (int i = 1; i <= gantt_index + 1; i++) // 가운데 줄
		{
			n = i - 1;

			printf("| P%d ", g_p[n]);

			for (int j = 0; j < g_bt[n]; j++)
			{
				printf(" ");
			}


			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 가운데 줄의 공백
			{
				printf("| ");
				for (int k = 0; k < (g_et[i] - (g_et[n] + g_bt[n])); k++)
				{
					printf(" ");
				}
			}
		}



		printf("|");
		printf("\n");

		for (int i = 1; i <= gantt_index + 1; i++) // 아랫줄
		{
			printf(" ----");

			for (int j = 0; j < g_bt[i - 1]; j++)
			{
				printf("-");
			}

			n = i - 1;
			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 아랫줄 "-"
			{
				printf(" -");
				for (int k = 0; k < (g_et[i] - (g_et[n] + g_bt[n])); k++)
				{
					printf("-");
				}
			}
		}


		printf("\n");

		int a, b, c, d, e, f;
		for (int i = 1; i <= gantt_index + 1; i++) // 아랫줄 밑 실행시간
		{
			printf("%d    ", g_et[i - 1]);
			for (int j = 0; j < g_bt[i - 1]; j++)
			{
				printf(" ");
			}

			a = g_et[i - 1];                                  // 자릿수만큼 공백 빼기
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
			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])
			{
				printf("%d ", g_et[n] + g_bt[n]);
				for (int k = 0; k < (g_et[i] - (g_et[n] + g_bt[n])); k++)
				{
					printf(" ");
				}

				d = g_et[n] + g_bt[n];                              // 자릿수만큼 공백 빼기
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


		printf("%d", g_et[gantt_index] + g_bt[gantt_index]);
		printf("\n");
		// 간트 차트 끝
		
	free(g_p);
	free(g_et);
	free(g_bt);
	free(queue);

}
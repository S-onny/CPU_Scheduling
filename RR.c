#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"

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

void RR(DATA* data)
{
	int num_proc = data->num_proc;
	PROC* procs = data->procs;
	int tq=(data->tq);
	
	int gantt_index, time;
	double ttat = 0, twt = 0;
	// ijkn: ๋ฐ๋ณต๋ฌธ? ?? ๋ณ?, gantt_index:๊ฐ?ธ์ฐจ?ธ๋? ๊ทธ๋ฆฌ๊ธ??? ๋ฐฐ์ด๋ค์ index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// time: ?๋ก?ธ์??๊ฐ
	// E:๋ฌดํ๋ฃจํ ?์ถ
	int front = -1;
	int rear = -1;
	int* queue = (int*)malloc((num_proc+1) * sizeof(int)); //?๊ธฐํ
	int* g_p = (int*)malloc( sizeof(int));//๊ฐ?ธ์ฐจ?ธ๋? ๊ทธ๋ฆฌ๊ธ??? ??๋ฐฐ์ด๋?
	int* g_et = (int*)malloc( sizeof(int));//๊ฐ?ธ์ฐจ?ธ๋? ๊ทธ๋ฆฌ๊ธ??? ??๋ฐฐ์ด๋?
	int* g_bt = (int*)malloc(sizeof(int));//๊ฐ?ธ์ฐจ?ธ๋? ๊ทธ๋ฆฌ๊ธ??? ??๋ฐฐ์ด๋?
	int index = 0;		//Pop?ฌ๋?? ?? ?ธ๋ฑ์?
	int queue_index = -1;	//???CPU?? ์ค???๋ก?ธ์??ธ๋ฑ์?
	int at_index = 0;	//?์ฐฉํ ?๋ก?ธ์??ธ๋ฑ์?

	qsort(procs, num_proc, sizeof(PROC), compare_a); //?์ฐ????๋ก ???๊ฐ?? ?๋ก?ธ์?๋ฒ?ธ๋๋ก)

		time = 0;
		gantt_index = 0;
		int N = num_proc;//N:?คํ????์ง ?? ?๋ก?ธ์??. ??ฅ๋ ?๋ก?ธ์ค์๋ฅ?๋ณต์ฌํ??์ด๊ธฐํ

		if (at_index < num_proc && 0 == procs[at_index].at) {	//์ฒ? ?์ฐฉํ ?๋ก?ธ์??ธ์?
			push(&front, &rear, num_proc + 1, at_index, queue);
			at_index++;

		}

		while (N > 0) { //์ข๋ฃ์กฐ๊ฑด: line136 
			
			if (index == 0) {	//?ธ๋ฑ์ค๊? 0?ด๋ฉด pop?๊ณ  ์ด๊ธฐํ
			
				queue_index = pop(&front, &rear, num_proc + 1, queue);
				
				if (queue_index == -1) {//????คํ๊ฐ?ฅํ ?๋ก?ธ์???(idle)
										  /*//?ด๋?๋ถ? ?ค๋ฅธ??? ๊ฐ?ธ์ฐจ?ธ๋? ๊ทธ๋ฆฌ๊ธ??? ?๋น ์ฝ??
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
					time++;//?๊ฐ?๋ฆ
					continue;
				}
				if (procs[queue_index].p == g_p[gantt_index - 1]) {	//? ?๋ก?ธ์ค๋ ? ?๋ก?ธ์???ผํ๋ฉ?๊ฐ?ธ์ธ๋ฑ์?? ์ง
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

			for (int i = 0; i < num_proc; i++) {	//?คํ????์ง ?? ?๋ก?ธ์ค์? ?คํ?์ง ?? ?๋ก?ธ์ค๋ค์ ?๊ธฐ์๊ฐ ์ฆ๊ฐ
				if (i != queue_index && procs[i].at <= time && (procs[i].c == -1))
					(procs[i].wt)++;
			}
			(procs[queue_index].rem)--;//?คํ์ค???๋ก?ธ์ค์ remain time ๊ฐ?
			(g_bt[gantt_index])++;

			if (at_index < num_proc && time + 1 >= procs[at_index].at) {	//?์ฐฉํ ?๋ก?ธ์??ธ์?
				push(&front, &rear, num_proc + 1, at_index, queue);
				at_index++;

			}

			if (procs[queue_index].et == -1) procs[queue_index].et = time;//?๋ก?ธ์ค๊? ์ต์ด๋ก ?คํ? ?๊ฐ? et? ???
			if (procs[queue_index].rem == 0)	//?คํ?๋ฃ????๋ก?ธ์?๋ฐ๊ฟ์ฃผ๊ธฐ
			{
				procs[queue_index].ct = time + 1;
				procs[queue_index].tat = procs[queue_index].ct - procs[queue_index].at;
				procs[queue_index].c = 0;
				N = N - 1;//?คํ๊ฐ?ฅํ ?๋ก?ธ์?? ๊ฐ?
				if (N == 0) {//๋ชจ๋  ?๋ก?ธ์ค๊? ?คํ์ข๋ฃ? ๊ฒฝ์?๋ฐ๋ณต๋ฌธ ?์ถ
					time++;
					break;
				}
				gantt_index++;
				index = 0;


			}
			else if (tq == index + 1)	//time-quantom? ????๋ก?ธ์?๋ฐ๊ฟ์ฃผ๊ธฐ
			{

				push(&front, &rear, num_proc + 1, queue_index, queue);

				gantt_index++;
				index = 0;

			}
			else
				index++;

			time++;//?๊ฐ???๋ฆ

		}
		//?๋ก?ธ์?์ข๋ฃ

		qsort(procs, num_proc, sizeof(PROC), compare_p);//procs๋ฅ??ค์ pid??ผ๋? ???


		(data->g_p) = g_p;
		(data->g_et) = g_et;
		(data->g_bt) = g_bt;
		(data->gantt_index) = gantt_index;
		free(queue);
	

		Print_table(data);

		// ๊ฐ??์ฐจํ?๊ทธ๋ฆฌ๊ธ?
		Print_gantt(data);
		// ๊ฐ??์ฐจํ??
		

	
	return;
}

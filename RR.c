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
	// ijkn: 반복문?� ?�?� 변?�, gantt_index:간?�차?��? 그리�??�?� 배열들의 index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// time: ?�로?��??�각
	// E:무한루프 ?�출
	int front = -1;
	int rear = -1;
	int* queue = (int*)malloc((num_proc+1) * sizeof(int)); //?�기큐
	int* g_p = (int*)malloc( sizeof(int));//간?�차?��? 그리�??�?� ?�?�배열�?
	int* g_et = (int*)malloc( sizeof(int));//간?�차?��? 그리�??�?� ?�?�배열�?
	int* g_bt = (int*)malloc(sizeof(int));//간?�차?��? 그리�??�?� ?�?�배열�?
	int index = 0;		//Pop?��??� ?�?� ?�덱�?
	int queue_index = -1;	//?�??CPU?�?�중???�로?��??�덱�?
	int at_index = 0;	//?�착한 ?�로?��??�덱�?

	qsort(procs, num_proc, sizeof(PROC), compare_a); //?��??�?�?�로 ?�??같?�?� ?�로?��?번?�대로)

		time = 0;
		gantt_index = 0;
		int N = num_proc;//N:?�행???�?�지 ?�?� ?�로?��??�. ?�?�된 ?�로?�스수�?복사하??초기화

		if (at_index < num_proc && 0 == procs[at_index].at) {	//처?� ?�착한 ?�로?��??��?
			push(&front, &rear, num_proc + 1, at_index, queue);
			at_index++;

		}

		while (N > 0) { //종료조건: line136 
			
			if (index == 0) {	//?�덱스�? 0?�면 pop?�고 초기화
			
				queue_index = pop(&front, &rear, num_proc + 1, queue);
				
				if (queue_index == -1) {//?�???�행가?�한 ?�로?��??�?�(idle)
										  /*//?��?분?� ?�른?�?�?� 간?�차?��? 그리�??�?� ?�비 코?�?�
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
					time++;//?�간?�름
					continue;
				}
				if (procs[queue_index].p == g_p[gantt_index - 1]) {	//?� ?�로?�스랑 ?� ?�로?��??�?�하�?간?�인덱�??�지
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

			for (int i = 0; i < num_proc; i++) {	//?�행???�?�지 ?�?� ?�로?�스�? ?�행?�지 ?�?� ?�로?�스들의 ?�기시간 증가
				if (i != queue_index && procs[i].at <= time && (procs[i].c == -1))
					(procs[i].wt)++;
			}
			(procs[queue_index].rem)--;//?�행중???�로?�스의 remain time 감?�
			(g_bt[gantt_index])++;

			if (at_index < num_proc && time + 1 >= procs[at_index].at) {	//?�착한 ?�로?��??��?
				push(&front, &rear, num_proc + 1, at_index, queue);
				at_index++;

			}

			if (procs[queue_index].et == -1) procs[queue_index].et = time;//?�로?�스�? 최초로 ?�행?� ?�간?� et?� ?�??
			if (procs[queue_index].rem == 0)	//?�행?�료?�???�로?��?바꿔주기
			{
				procs[queue_index].ct = time + 1;
				procs[queue_index].tat = procs[queue_index].ct - procs[queue_index].at;
				procs[queue_index].c = 0;
				N = N - 1;//?�행가?�한 ?�로?��??� 감?�
				if (N == 0) {//모든 ?�로?�스�? ?�행종료?� 경�?반복문 ?�출
					time++;
					break;
				}
				gantt_index++;
				index = 0;


			}
			else if (tq == index + 1)	//time-quantom?� ?�???�로?��?바꿔주기
			{

				push(&front, &rear, num_proc + 1, queue_index, queue);

				gantt_index++;
				index = 0;

			}
			else
				index++;

			time++;//?�간???�름

		}
		//?�로?��?종료

		qsort(procs, num_proc, sizeof(PROC), compare_p);//procs�??�시 pid?�?��? ?�??


		(data->g_p) = g_p;
		(data->g_et) = g_et;
		(data->g_bt) = g_bt;
		(data->gantt_index) = gantt_index;
		free(queue);
	

		Print_table(data);

		// 간??차�?그리�?
		Print_gantt(data);
		// 간??차�??�
		

	
	return;
}

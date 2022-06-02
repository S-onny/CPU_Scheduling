#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#include "Print_readyqueue.h"

void RR(DATA* data)
{
	int num_proc = data->num_proc;
	PROC* procs = data->procs;
	int tq = (data->tq);

	int gantt_index, time;
	double ttat = 0, twt = 0;
	// ijkn: for itterative, gantt_index:for gantt chart˜ index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// time: time goes on tictok++...
	int front = -1;
	int rear = -1;
	int* queue = (int*)malloc((num_proc + 1) * sizeof(int)); //ready queue alloc
	int* g_p = (int*)malloc(sizeof(int));//--------------------
	int* g_et = (int*)malloc(sizeof(int));//arrays for gantt
	int* g_bt = (int*)malloc(sizeof(int));//-------------------
	int index = 0;		//for Pop
	int queue_index = -1;	//
	int at_index = 0;	//

	qsort(procs, num_proc, sizeof(PROC), compare_a); //sort by arrival. coincidence: sort by pid.

	time = 0;
	gantt_index = 0;
	int N = num_proc;//N is # of processes which not yet completed.

	
	if (at_index < num_proc) {
		for (int i = at_index; i < num_proc; i++) {
			if (0 == procs[i].at) {	//도착한 프로세스 푸쉬
				
				push(&front, &rear, num_proc + 1, i, queue);
				at_index++;
			}
		}
	}

	while (N > 0) {

		if (index == 0) {	//time to pop process

			queue_index = pop(&front, &rear, num_proc + 1, queue);

			if (queue_index == -1) {

				if (at_index < num_proc) {
					for (int i = at_index; i < num_proc; i++) {
						if (time + 1 == procs[i].at) {	//도착한 프로세스 푸쉬
							push(&front, &rear, num_proc + 1, i, queue);
							at_index++;
						}
					}
				}
				time++;//time goes on. next cycle.
				continue;
			}
			if (procs[queue_index].p == g_p[gantt_index - 1]) {	//find out if this process was being execution
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

		for (int i = 0; i < num_proc; i++) {	//arrived, not executed and not completed guys will wait for now
			if (i != queue_index && procs[i].at <= time && (procs[i].c == -1))
				(procs[i].wt)++;
		}
		(procs[queue_index].rem)--;//processing process's remain time has decreased
		(g_bt[gantt_index])++;

		if (at_index < num_proc) {
			for (int i = at_index; i < num_proc; i++) {
				if (time + 1 == procs[i].at) {	//도착한 프로세스 푸쉬
					push(&front, &rear, num_proc + 1, i, queue);
					at_index++;
				}
			}
		}

		if (procs[queue_index].et == -1) procs[queue_index].et = time;//Now is it's First pop o'clock.
		if (procs[queue_index].rem == 0)	//It's done!
		{
			procs[queue_index].ct = time + 1;
			procs[queue_index].tat = procs[queue_index].ct - procs[queue_index].at;
			procs[queue_index].c = 0;
			N = N - 1;//
			if (N == 0) {//no more executables.
				time++;
				break;
			}
			gantt_index++;
			index = 0;


		}
		else if (data->tq == index + 1)	//time-quantom over. change gantt
		{

			push(&front, &rear, num_proc + 1, queue_index, queue);

			gantt_index++;
			index = 0;

		}
		else
			index++;

		time++;//time goes on

	}
	//end of processing

	qsort(procs, num_proc, sizeof(PROC), compare_p);//procsë¥??¤ì‹œ pid?œ?¼ë? ?•??


	(data->g_p) = g_p;
	(data->g_et) = g_et;
	(data->g_bt) = g_bt;
	(data->gantt_index) = gantt_index;
	free(queue);


	

	// gantt chart
	Print_gantt(data);
	Print_table(data);
	printf("\n\n\n");
	Print_readyQueue(data);
	// end of func


	return;
}

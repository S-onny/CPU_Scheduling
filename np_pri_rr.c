#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#define INT_MAX 2147483647

void np_pri_rr(DATA * data)
{
	int num_proc = data->num_proc;
	PROC* procs = data->procs;
	int tq = (data->tq);
	int n, gantt_index, time;
	double ttat = 0, twt = 0;
	int front = -1, rear = -1;
	int *queue = (int *)malloc((num_proc + 1) * sizeof(int));
	int *g_p = (int *)malloc(sizeof(int));
	int *g_et = (int *)malloc(sizeof(int));
	int *g_bt = (int *)malloc(sizeof(int));
	int min_pri = INT_MAX;
	int min_pri_index = -1;
	int index = -1, pop_index = 0, at_index = 0;
	int queue_index = -1;

	qsort(procs, num_proc, sizeof(PROC), compare_a);

	time = 0;
	gantt_index = 0;
	int N = num_proc;
	
	if (at_index < num_proc && 0 == procs[at_index].at)
	{
		push(&front, &rear, num_proc + 1, at_index, queue);
		at_index++;
	}

	while (N > 0)
	{

		if (index == -1)
		{
			min_pri = INT_MAX;
			min_pri_index = -1;
			for (int i = 0; i < N; i++)
			{
				if (procs[i].at > time)
					break;
				if (procs[i].pri < min_pri)
				{
					min_pri_index = i;
					min_pri = procs[i].pri;
				}
			}
			if (pop_index == 0)
			{
				queue_index = pop(&front, &rear, num_proc + 1, queue);
			}
			if (min_pri_index == -1)
			{
				time++;
				continue;
			}
			if (procs[queue_index].p == g_p[gantt_index - 1])
			{
				gantt_index--;
			}
			else
			{
				g_p[gantt_index] = procs[min_pri_index].p;
				g_bt[gantt_index] = 0;
				g_et[gantt_index] = time;
			}
			g_p = (int *)realloc(g_p, (2 + gantt_index) * sizeof(int));
			g_et = (int *)realloc(g_et, (2 + gantt_index) * sizeof(int));
			g_bt = (int *)realloc(g_bt, (2 + gantt_index) * sizeof(int));
			index = 0;
			pop_index = 0;
		}

		for (int i = 0; i < N; i++)
		{
			if (procs[i].at > time)
				break;
			if (i == min_pri_index)
				continue;
			(procs[i].wt)++;
		}
		(procs[min_pri_index].rem)--;
		(g_bt[gantt_index])++;

		if (procs[min_pri_index].et == -1)
			procs[min_pri_index].et = time;
		if (procs[min_pri_index].rem == 0)
		{
			procs[min_pri_index].ct = time + 1;
			procs[min_pri_index].tat = procs[min_pri_index].ct - procs[min_pri_index].at;
			PROC temp = procs[N - 1];
			procs[N - 1] = procs[min_pri_index];
			procs[min_pri_index] = temp;
			N = N - 1;
			if (N == 0)
			{
				time++;
				break;
			}
			gantt_index++;
			index = -1;
			pop_index = 0;
			qsort(procs, N, sizeof(PROC), compare_a);
		}
		else if (tq == index + 1)
		{
			push(&front, &rear, num_proc + 1, queue_index, queue);
			gantt_index++;
			index = 0;
		}
		else
			index++;
		time++;
	}

	qsort(procs, num_proc, sizeof(PROC), compare_p);//sort as pid


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
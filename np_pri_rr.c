#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#define INT_MAX 2147483647

int compare_a(const void *p1, const void *p2);
int compare_p(const void *p1, const void *p2);

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

void np_pri_rr(PROC *procs, int num_proc, int tq)
{
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

	qsort(procs, num_proc, sizeof(PROC), compare_p);

	for (int i = 0; i < num_proc; i++)
	{
		twt += procs[i].wt;
		ttat += procs[i].tat;
	}

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

	// Gantt Chart
	printf("\n\n");
	printf("Gantt Chart %d %d", gantt_index, g_p[gantt_index]);
	printf("\n");

	for (int i = 1; i <= gantt_index + 1; i++)
	{
		printf(" ----");
		n = i - 1;
		for (int j = 0; j < g_bt[n]; j++)
		{
			printf("-");
		}

		if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])
		{

			printf(" -");
			for (int k = 0; k < g_et[i] - (g_et[n] + g_bt[n]); k++)
			{
				printf("-");
			}
		}
	}

	printf("\n");

	for (int i = 1; i <= gantt_index + 1; i++)
	{
		n = i - 1;
		printf("| P%d ", g_p[n]);

		for (int j = 0; j < g_bt[n]; j++)
		{
			printf(" ");
		}

		if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])
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

	for (int i = 1; i <= gantt_index + 1; i++)
	{
		printf(" ----");
		for (int j = 0; j < g_bt[i - 1]; j++)
		{
			printf("-");
		}

		n = i - 1;
		if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])
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
	for (int i = 1; i <= gantt_index + 1; i++)
	{
		printf("%d    ", g_et[i - 1]);
		for (int j = 0; j < g_bt[i - 1]; j++)
		{
			printf(" ");
		}

		a = g_et[i - 1];
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

		n = i - 1;
		if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])
		{
			printf("%d ", g_et[n] + g_bt[n]);
			for (int k = 0; k < (g_et[i] - (g_et[n] + g_bt[n])); k++)
			{
				printf(" ");
			}

			d = g_et[n] + g_bt[n];
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

	free(g_p);
	free(g_et);
	free(g_bt);
}
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
	// ijkn: �ݺ����� ���� ����, gantt_index:��Ʈ��Ʈ�� �׸��� ���� �迭���� index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// time: ���μ��� �ð�
	// E:���ѷ��� Ż��
	int front = -1;
	int rear = -1;
	int* queue = (int*)malloc((num_proc+1) * sizeof(int)); //���ť
	int* g_p = (int*)malloc( sizeof(int));//��Ʈ��Ʈ�� �׸��� ���� �����迭��
	int* g_et = (int*)malloc( sizeof(int));//��Ʈ��Ʈ�� �׸��� ���� �����迭��
	int* g_bt = (int*)malloc(sizeof(int));//��Ʈ��Ʈ�� �׸��� ���� �����迭��
	int index = 0;		//Pop���ο� ���� �ε���
	int queue_index = -1;	//���� CPU�������� ���μ��� �ε���
	int at_index = 0;	//������ ���μ��� �ε���

	qsort(procs, num_proc, sizeof(PROC), compare_a); //���� ������� ����(������ ���μ��� ��ȣ���)


		time = 0;
		gantt_index = 0;
		int N = num_proc;//N:������ ������ ���� ���μ��� ��. �Էµ� ���μ������� �����Ͽ� �ʱ�ȭ

		if (at_index < num_proc && 0 == procs[at_index].at) {	//ó�� ������ ���μ��� Ǫ��
			push(&front, &rear, num_proc + 1, at_index, queue);
			at_index++;

		}

		while (N > 0) { //��������: line136 

			if (index == 0) {	//�ε����� 0�̸� pop�ϰ� �ʱ�ȭ
			
				queue_index = pop(&front, &rear, num_proc + 1, queue);
				
				if (queue_index == -1) {//���� ���డ���� ���μ��� ����(idle)
										  /*//�̺κ��� �ٸ������� ��Ʈ��Ʈ�� �׸��� ���� ���� �ڵ���
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
					time++;//�ð��帧
					continue;
				}
				if (procs[queue_index].p == g_p[gantt_index - 1]) {	//�� ���μ����� �� ���μ��� �����ϸ� ��Ʈ�ε��� ����
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

			for (int i = 0; i < num_proc; i++) {	//������ ������ ���� ���μ����� ������� ���� ���μ������� ���ð� ����
				if (i != queue_index && procs[i].at <= time && (procs[i].c == -1))
					(procs[i].wt)++;
			}
			(procs[queue_index].rem)--;//�������� ���μ����� remain time ����
			(g_bt[gantt_index])++;

			if (at_index < num_proc && time + 1 == procs[at_index].at) {	//������ ���μ��� Ǫ��
				push(&front, &rear, num_proc + 1, at_index, queue);
				at_index++;

			}

			if (procs[queue_index].et == -1) procs[queue_index].et = time;//���μ����� ���ʷ� ����� �ð��� et�� ����
			if (procs[queue_index].rem == 0)	//����Ϸ�Ǿ� ���μ��� �ٲ��ֱ�
			{
				procs[queue_index].ct = time + 1;
				procs[queue_index].tat = procs[queue_index].ct - procs[queue_index].at;
				procs[queue_index].c = 0;
				N = N - 1;//���డ���� ���μ��� �� ����
				if (N == 0) {//��� ���μ����� ��������� ��� �ݺ��� Ż��
					time++;
					break;
				}
				gantt_index++;
				index = 0;


			}
			else if (tq == index + 1)	//time-quantom�� ���� ���μ��� �ٲ��ֱ�
			{

				push(&front, &rear, num_proc + 1, queue_index, queue);

				gantt_index++;
				index = 0;

			}
			else
				index++;

			time++;//�ð��� �帧

		}
		//���μ��� ����

		qsort(procs, num_proc, sizeof(PROC), compare_p);//procs�� �ٽ� pid������ ����

		for (int i = 0; i < num_proc; i++) {//twt ttat ���
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


		// ��Ʈ ��Ʈ �׸���
		printf("\n\n");
		printf("Gantt Chart %d %d", gantt_index, g_p[gantt_index]);
		printf("\n");

		for (int i = 1; i <= gantt_index + 1; i++) // ����
		{
			printf(" ----");
			n = i - 1;
			for (int j = 0; j < g_bt[n]; j++)
			{
				printf("-");
			}



			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])          // ���μ��� ���̿� �� �ð��� ������쿡 ��µǴ� ��Ʈ��Ʈ�� �� ������ ���� "-" 
			{

				printf(" -");
				for (int k = 0; k < g_et[i] - (g_et[n] + g_bt[n]); k++)
				{
					printf("-");
				}
			}
		}





		printf("\n");

		for (int i = 1; i <= gantt_index + 1; i++) // ��� ��
		{
			n = i - 1;

			printf("| P%d ", g_p[n]);

			for (int j = 0; j < g_bt[n]; j++)
			{
				printf(" ");
			}


			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])          // ���μ��� ���̿� �� �ð��� ������쿡 ��µǴ� ��Ʈ��Ʈ�� �� ������ ��� ���� ����
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

		for (int i = 1; i <= gantt_index + 1; i++) // �Ʒ���
		{
			printf(" ----");

			for (int j = 0; j < g_bt[i - 1]; j++)
			{
				printf("-");
			}

			n = i - 1;
			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])          // ���μ��� ���̿� �� �ð��� ������쿡 ��µǴ� ��Ʈ��Ʈ�� �� ������ �Ʒ��� "-"
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
		for (int i = 1; i <= gantt_index + 1; i++) // �Ʒ��� �� ����ð�
		{
			printf("%d    ", g_et[i - 1]);
			for (int j = 0; j < g_bt[i - 1]; j++)
			{
				printf(" ");
			}

			a = g_et[i - 1];                                  // �ڸ�����ŭ ���� ����
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

			n = i - 1;                              // ���μ��� ���̿� �� �ð��� ������쿡 ��µǴ� ��Ʈ��Ʈ�� �� ���� ���� ����ð� ������ ����
			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])
			{
				printf("%d ", g_et[n] + g_bt[n]);
				for (int k = 0; k < (g_et[i] - (g_et[n] + g_bt[n])); k++)
				{
					printf(" ");
				}

				d = g_et[n] + g_bt[n];                              // �ڸ�����ŭ ���� ����
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
		// ��Ʈ ��Ʈ ��
		
	free(g_p);
	free(g_et);
	free(g_bt);
	free(queue);

}
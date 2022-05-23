#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#define INT_MAX	2147483647//int�� �ִ� define

int compare_a(const void* p1, const void* p2);
int compare_p(const void* p1, const void* p2);
void np_priority(PROC* procs, int num_proc)
{
	int n, gantt_index, time;
	double ttat = 0, twt = 0;
	// ijkn: �ݺ����� ���� ����, gantt_index:��Ʈ��Ʈ�� �׸��� ���� �迭���� index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// tq = time quantum
	// time: ���μ��� �ð�
	// E:���ѷ��� Ż��
	int* g_p = (int*)malloc(sizeof(int));//��Ʈ��Ʈ�� �׸��� ���� �����迭��
	int* g_et = (int*)malloc(sizeof(int));//��Ʈ��Ʈ�� �׸��� ���� �����迭��
	int* g_bt = (int*)malloc(sizeof(int));//��Ʈ��Ʈ�� �׸��� ���� �����迭��
	int min_pri = INT_MAX;//���డ���� ���μ��� �� ���� ª�� �����ð�
	int min_pri_index = -1;// ���డ���� ���μ����� ���� remain time�� ª�� ���μ����� index

	int index = -1;


		qsort(procs, num_proc, sizeof(PROC), compare_a);//���� ������� ����(������ ���μ��� ��ȣ���)
		printf("\n\n");

		time = 0;
		gantt_index = 0;
		int N = num_proc;//N:������ ������ ���� ���μ��� ��. �Էµ� ���μ������� �����Ͽ� �ʱ�ȭ
		while (N > 0) { //��������: line136 

			if (index == -1) {
				min_pri = INT_MAX;//�ּڰ��� ã�� ���� ���� ��� int�� ���� �� �ִ� �ְ������� �ʱ�ȭ
				min_pri_index = -1;// -1�� �ʱ�ȭ. ���� ���°� idle���� �����ϱ� ����.
				for (int i = 0; i < N; i++) {//priority ���� ���� �� ã��
					if (procs[i].at > time) break;
					if (procs[i].pri < min_pri) {
						min_pri_index = i;
						min_pri = procs[i].pri;
					}
				}
				if (min_pri_index == -1) {//���� ���డ���� ���μ��� ����(idle)
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
				g_p[gantt_index] = procs[min_pri_index].p;
				g_bt[gantt_index] = 0;
				g_et[gantt_index] = time;

				g_p = (int*)realloc(g_p, (2 + gantt_index) * sizeof(int));
				g_et = (int*)realloc(g_et, (2 + gantt_index) * sizeof(int));
				g_bt = (int*)realloc(g_bt, (2 + gantt_index) * sizeof(int));
				index = 0;
			}

			for (int i = 0; i < N; i++) {//������ ������ ���� ���μ����� ������� ���� ���μ������� ���ð� ����
				if (procs[i].at > time) break;
				if (i == min_pri_index)
					continue;
				(procs[i].wt)++;
			}
			(procs[min_pri_index].rem)--;//�������� ���μ����� remain time ����
			(g_bt[gantt_index])++;

			if (procs[min_pri_index].et == -1) procs[min_pri_index].et = time;//���μ����� ���ʷ� ����� �ð��� et�� ����
			if (procs[min_pri_index].rem == 0)
			{	//���μ����� ��������� ���: ���� ������ index�� ���μ����� ���� ���μ��� swap.
				procs[min_pri_index].ct = time + 1;
				procs[min_pri_index].tat = procs[min_pri_index].ct - procs[min_pri_index].at;
				PROC temp = procs[N - 1];
				procs[N - 1] = procs[min_pri_index];
				procs[min_pri_index] = temp;
				N = N - 1;//���డ���� ���μ��� �� ����
				if (N == 0) {//��� ���μ����� ��������� ��� �ݺ��� Ż��
					time++;
					break;
				}
				gantt_index++;
				index = -1;
				qsort(procs, N, sizeof(PROC), compare_a);//���డ���� ���μ������� �ٽ� arrival������ ����.
			}

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

}
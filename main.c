#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"

void FCFS(DATA* data);
void RR(DATA* procs);
void pre_pri(DATA* data);
void SRTF(DATA* data);
void pre_pri(DATA* data);
void SJF(DATA* data);
void np_pri_rr(DATA *data);
//본인함수 넣어주세요


int main(void) {

	while (1) {
		char s1 = 0;
		char f = 0; //파일 입출력
		int num_proc = 1;
		int tq = 0;
		int check[7] = { 0, };
		PROC* procs = NULL;//프로세스 구조체 배열
		printf("Please Select Your Input.\n");
		printf("1.File Input\n2.CLI Input\n\n");
		while (!(f >= '1' && f <= '2')) {
			f = getch();
		}
		if (f == '2') {
			printf("Enter the number of process: ");
			scanf("%d", &num_proc);
			printf("\n\n");
			procs = (PROC*)malloc(sizeof(PROC) * num_proc);
			printf("Enter the processes\n");
			printf("Processes\tArrival Time\tBurst time\tPriority\n");
			for (int i = 0; i < num_proc; i++) // at, bt 입력받고, 프로세스구조체 배열에 pid와 함께 저장
			{
				printf("Process[%d]:\t\t", i + 1);
				scanf("%d %d %d", &(procs[i].at), &(procs[i].bt), &(procs[i].pri));
				procs[i].p = i + 1;
			}
			printf("Enter the Time-Quantom: ");
			scanf("%d", &tq);
			for (int i = 0; i < num_proc; i++) {
				procs[i].rem = procs[i].bt;//남은시간 burst time으로 초기화
				procs[i].wt = 0;//기타 필요한 수치들 초기화
				procs[i].et = 0 - 1;//최초 실행시간 구분을 위해 -1로 초기화
				procs[i].ct = 0;
				procs[i].tat = 0;
				procs[i].c = -1;
			}
		}
		else if (f == '1') {
			FILE* fp;
			int ret;

			if ((fp = fopen("input.txt", "rt")) == NULL)
			{
				fprintf(stderr, "error: opening file failed! \n");
				return 0;
			}

			procs = Process_load(fp, &num_proc, &tq);
			if (procs == NULL) return 0;
			fclose(fp);
		}


		DATA* Datas = Make_dataIn(procs, num_proc, tq);



		while (1) {
			char s = 0;
			s1 = 0;
			char s2 = 0;
			int del;

			printf("\n\nChoose the algorithm\n");
			printf("0.Edit Process\n");
			printf("1.FCFS\n");
			printf("2.SJF\n");
			printf("3.SRTF\n");
			printf("4.RR\n");
			printf("5.Non-Preemptive Priority\n");
			printf("6.Preemptive Priority\n");
			printf("7.Non-Preemptive Priority with RR\n");
			while (!(s >= '0' && s <= '7')) {
				s = getch();
			}
			if (s == '0') {
				printf("\nEdit Process Selected!\n");
				printf("1.Delete Process\n");
				printf("2.Add Process\n");
				while (!(s2 >= '1' && s <= '2')) {
					s2 = getch();
				}
				if (s2 == '1') {
					num_proc--;
					printf("\nEnter the process number that you would like to delete\n");
					scanf("%d", &del);
					Destroy_data(Datas);
					for (int i = del - 1; i < num_proc; i++) {
						procs[i] = procs[i + 1];
					}
					printf("%d\n", procs[num_proc-1].p);
					for (int i = 0; i < num_proc; i++) {
						procs[i].rem = procs[i].bt;//남은시간 burst time으로 초기화
						procs[i].wt = 0;//기타 필요한 수치들 초기화
						procs[i].et = 0 - 1;//최초 실행시간 구분을 위해 -1로 초기화
						procs[i].ct = 0;
						procs[i].tat = 0;
						procs[i].c = -1;
					}
			
					Datas= Make_dataIn(procs, num_proc, tq);
					
					for (int i = 0; i < 7; i++) {
						check[i] = 0;
					}
					printf("complete\n\n");
				}
				else if (s2 == '2') {
					num_proc++;
					procs = (PROC*)realloc(procs, sizeof(PROC)*num_proc);
					printf("\nEnter the process\n");
					printf("Processes\tArrival Time\tBurst time\tPriority\n\t\t");
					scanf("%d %d %d", &(procs[num_proc-1].at), &(procs[num_proc - 1].bt), &(procs[num_proc - 1].pri));
					procs[num_proc-1].p = num_proc;
					for (int i = 0; i < num_proc; i++) {
						procs[i].rem = procs[i].bt;//남은시간 burst time으로 초기화
						procs[i].wt = 0;//기타 필요한 수치들 초기화
						procs[i].et = 0 - 1;//최초 실행시간 구분을 위해 -1로 초기화
						procs[i].ct = 0;
						procs[i].tat = 0;
						procs[i].c = -1;
					}
					Datas = Make_dataIn(procs, num_proc, tq);
					for (int i = 0; i < 7; i++) {
						check[i] = 0;
					}
					printf("complete\n\n");
					
				}
				continue;
			}
			else if (s == '1') {
				printf("\nFCFS Selected!\n");

				if (check[s - '1'] == 0) {
					FCFS(&Datas[0]);
					check[s - '1'] = 1;
				
				}
				else {
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '2') {
				printf("\nSJF Selected!\n");
				if (check[s - '1'] == 0) {
					SJF(&Datas[1]);
					check[s - '1'] = 1;
				}
				else {
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '3') {
				printf("\nSRTF Selected!\n");
				if (check[s - '1'] == 0) {
					SRTF(&Datas[2]);
					check[s - '1'] = 1;
				}
				else {
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '4') {
				printf("\nRR Selected!\n");
				if (check[s - '1'] == 0) {
					RR(&Datas[3]);
					check[s - '1'] = 1;
				}
				else
				{
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '5') {
				printf("\nNON-Preemptive Priority Selected!\n");
				if (check[s - '1'] == 0) {
					np_priority(&Datas[4]);
					check[s - '1'] = 1;
				}
				else {
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '6') {
				printf("\nPreemptive Priority Selected!\n");
				if (check[s - '1'] == 0) {
					pre_pri(&Datas[5]);
					check[s - '1'] = 1;
				}
				else {
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '7') {
				printf("\nNon-Preemptive Priority with RR Selected!\n");
				if (check[s - '1'] == 0) {
					np_pri_rr(&Datas[6]);
					check[s - '1'] = 1;
				}
				else {
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}



			printf("1.Try with different algorithm\n");
			printf("2.Try with new process\n");
			printf("3.End\n");
			while (!(s1 >= '1' && s1 <= '3')) {
				s1 = getch();
			}
			if (s1 == '1') {
				printf("1 Selected\n\n");
			}
			else if (s1 == '2') {
				printf("2 Selected\n\n");
				break;
			}
			else if (s1 == '3') {
				break;
			}

		}
		Destroy_data(Datas);
		free(procs);
		if (s1 == '3')
			break;
	}
	return 0;
}

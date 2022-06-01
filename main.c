#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"

void RR(PROC* procs, int num_#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"

void RR(DATA* procs, int tq);
void pre_pri(DATA* data);
void SRTF(DATA* data);
//본인함수 넣어주세요


int main(void) {
	
	while (1) {
		char s1 = 0;
		int num_proc = 0;
		int tq = 0;
		int check[7] = { 0, };
		PROC* procs = (PROC*)malloc(sizeof(PROC));//프로세스 구조체 배열



		printf("Enter the number of process: ");
		scanf("%d", &num_proc);
		printf("\n\n");
		procs = (PROC*)realloc(procs, sizeof(PROC) * num_proc);
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

		DATA* Datas = Make_dataIn(procs, num_proc);

		

		while (1) {
			char s = 0;
			s1 = 0;

			printf("\n\nChoose the algorithm\n");
			printf("1.FCFS\n");
			printf("2.SJF\n");
			printf("3.SRTF\n");
			printf("4.RR\n");
			printf("5.Non-Preemptive Priority\n");
			printf("6.Preemptive Priority\n");
			printf("7.Non-Preemptive Priority with RR\n");
			while (!(s >= '1' && s <= '7')) {
				s = getch();
			}
			if (s == '1') {
				printf("FCFS Selected!\n");

				if (check[s - '1'] == 0) {
										check[s - '1'] = 1;
										//본인함수 넣어주세요
				}
				else {
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '2') {
				printf("SJF Selected!\n");
				if (check[s - '1'] == 0) {
					//본인함수 넣어주세요
					check[s - '1'] = 1;
				}
				else {
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '3') {
				printf("SRTF Selected!\n");
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
				printf("RR Selected!\n");
				if (check[s - '1'] == 0) {
					RR(&Datas[3], tq);
					check[s - '1'] = 1;
				}
				else
				{
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '5') {
				printf("NON-Preemptive Priority Selected!\n");
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
				printf("Preemptive Priority Selected!\n");
				if (check[s - '1'] == 0) {
					//본인함수 넣어주세요
					check[s - '1'] = 1;
				}
				else {
					Print_table(&Datas[s - '1']);
					Print_gantt(&Datas[s - '1']);
				}
			}
			else if (s == '7') {
				printf("Non-Preemptive Priority with RR Selected!\n");
				if (check[s - '1'] == 0) {
					//본인함수 넣어주세요
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
		printf("파괴");
		Destroy_data(Datas);
		printf("파괴완료");
		free(procs);
		if (s1 == '3')
			break;
	}
	return 0;
}


#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#include "Print_readyqueue.h"

//본인함수 넣어주세요
#include "FCFS.h"
#include "RR.h"
#include "pre_pri.h"
#include "SRTF.h"
#include "np_priority.h"
#include "SJF.h"
#include "np_pri_rr.h"
int main(void) {
	void(*funcptr[7])(DATA*);
	funcptr[0]=FCFS;
	funcptr[1]=SJF;
	funcptr[2]=SRTF;
	funcptr[3]=RR;
	funcptr[4]=np_priority;
	funcptr[5]=pre_pri;
	funcptr[6]=np_pri_rr;
	

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
			f = getchar();
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

			printf("\n\nChoose the function\n");
			printf("0.Edit Process\n");
			printf("1.FCFS\n");
			printf("2.SJF\n");
			printf("3.SRTF\n");
			printf("4.RR\n");
			printf("5.Non-Preemptive Priority\n");
			printf("6.Preemptive Priority\n");
			printf("7.Non-Preemptive Priority with RR\n");
			printf("8.RUN ALL scheduling Algorithms\n");
			printf("9.Rank cheduling Algorithms(run all of them first)\n");
		
			
			while (!(s >= '0' && s <= '9')) {
				s = getchar();
			}
			if (s == '0') {
				printf("\nEdit Process Selected!\n");
				printf("1.Delete Process\n");
				printf("2.Add Process\n");
				while (!(s2 >= '1' && s <= '2')) {
					s2 = getchar();
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
				printf("\n\n\n");
				printf("\nFCFS Selected!\n");

				if (check[s - '1'] == 0) {
					FCFS(&Datas[0]);
					check[s - '1'] = 1;
				
				}
				else {
					Print_gantt(&Datas[s - '1']);
					Print_table(&Datas[s - '1']);
					Print_readyQueue(&Datas[s-'1']);
				}
			}
			else if (s == '2') {
				printf("\n\n\n");
				printf("\nSJF Selected!\n");
				if (check[s - '1'] == 0) {
					SJF(&Datas[1]);
					check[s - '1'] = 1;
				}
				else {
					Print_gantt(&Datas[s - '1']);
					Print_table(&Datas[s - '1']);
					Print_readyQueue(&Datas[s-'1']);
				}
			}
			else if (s == '3') {
				printf("\n\n\n");
				printf("\nSRTF Selected!\n");
				if (check[s - '1'] == 0) {
					SRTF(&Datas[2]);
					check[s - '1'] = 1;
				}
				else {
					Print_gantt(&Datas[s - '1']);
					Print_table(&Datas[s - '1']);
					Print_readyQueue(&Datas[s-'1']);
				}
			}
			else if (s == '4') {
				printf("\n\n\n");
				printf("\nRR Selected!\n");
				if (check[s - '1'] == 0) {
					RR(&Datas[3]);
					check[s - '1'] = 1;
				}
				else
				{
					Print_gantt(&Datas[s - '1']);
					Print_table(&Datas[s - '1']);
					Print_readyQueue(&Datas[s-'1']);
				}
			}
			else if (s == '5') {
				printf("\n\n\n");
				printf("\nNON-Preemptive Priority Selected!\n");
				if (check[s - '1'] == 0) {
					np_priority(&Datas[4]);
					check[s - '1'] = 1;
				}
				else {
					Print_gantt(&Datas[s - '1']);
					Print_table(&Datas[s - '1']);
					Print_readyQueue(&Datas[s-'1']);
				}
			}
			else if (s == '6') {
				printf("\n\n\n");
				printf("\nPreemptive Priority Selected!\n");
				if (check[s - '1'] == 0) {
					pre_pri(&Datas[5]);
					check[s - '1'] = 1;
				}
				else {
					Print_gantt(&Datas[s - '1']);
					Print_table(&Datas[s - '1']);
					Print_readyQueue(&Datas[s-'1']);
				}
			}
			else if (s == '7') {
				printf("\n\n\n");
				printf("\nNon-Preemptive Priority with RR Selected!\n");
				if (check[s - '1'] == 0) {
					np_pri_rr(&Datas[6]);
					check[s - '1'] = 1;
				}
				else {
					Print_gantt(&Datas[s - '1']);
					Print_table(&Datas[s - '1']);
					Print_readyQueue(&Datas[s-'1']);
				}
			}
			else if(s=='8'){
				printf("\nRUN ALL!\n");
				for(int i = 0 ; i < 7 ; i++){
					printf("\n\n");
					switch(i){
						case(1):
							printf("<<<<<SJF>>>>>");
							break;
						case(2):
							printf("<<<<<SRTF>>>>>");
							break;
						case(3):
							printf("<<<<<RR>>>>>");
							break;
						case(4):
							printf("<<<<<Non-Preemptive Priority>>>>>");
							break;
						case(5):
							printf("<<<<<Preemptive Priority>>>>>");
							break;
						case(6):
							printf("<<<<<Non-Preemptive Priority with RR>>>>>");
							break;
						default:
							printf("<<<<<FCFS>>>>>");
					}
					if (check[i] == 0) {
						funcptr[i](&Datas[i]);
						check[i] = 1;
					}
					else {
						Print_gantt(&Datas[i]);
						Print_table(&Datas[i]);
						Print_readyQueue(&Datas[i]);
						
					}
				}
			}
			else if(s=='9'){
				printf("\nRanking Time!\n");
				al_rank(check,Datas);
			}



			printf("1.Try other functions on this process set\n");
			printf("2.Try with new process\n");
			printf("3.End\n");
			while (!(s1 >= '1' && s1 <= '3')) {
				s1 = getchar();
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

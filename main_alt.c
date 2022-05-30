#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#include "scheduling.h"


void RR(PROC* procs, int num_proc, int tq);

int main(int argc, char **argv) {
	FILE *fp;
	int num_proc = 0;
	int tq = 0;
	if (argc != 2)
	{
		printf("no file input!", argv[0]);
		return 0;
	}
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		fprintf( stderr, "error: opening file %s failed! \n", argv[1]);
		return 0;
	}
	int num_proc = 1;
	int tq = 0;
	int ret;
	PROC* procs = (PROC*)malloc(sizeof(PROC)*num_proc);//프로세스 구조체 배열
	
	ret=Process_load(fp,&num_proc,&tq,procs);
	if (!ret) return 0;
	 //FCFS
	//[function here]
	 //SJF
	//[function here]
	 //SRTF
	//[function here]
	 //RR
	//[function here]
	 //NON-Preemptive Priority
	//[function here]
	 //Preemptive Priority
	//[function here]
	 //Non-Preemptive Priority with RR
	//[function here]
	

	
	
	while (1) 
	{
		//입력옵션 선택 코드(미완). 과제 조건과 맞지 않아 일단 주석처리
		/*
		int file_or_manual=-1;
		while(file_or_manual!=0 || file_or_manual!=1)
		{
			printf("Processes input-Choose Your Option!\n\tfrom file [0], manual input[1]:");
			scanf("%d", &file_or_manual);
			printf("\n");
			if(file_or_manual!=0 || file_or_manual!=1)
				while (getchar() != '\n');////flush input buffer
		}
		char str[1024];
		FILE *fp;
		int exit=-1;
		while((!file_or_manual)&&(exit!=1 || exit!=2))
		{
			printf("Option: Processes Input by File\n");
			printf("Warning: only files in same directory with this EXE file work!\n");
			printf("enter filename(ex: process.txt) or Q to manual input: ");
			scanf("%s", str);
			while (getchar() != '\n');//flush input buffer
			if(0==strcmp(str,"Q"))
			{
				file_or_manual=1;
			}
			if((fp=fopen(str,"rt"))==NULL)
			{
				printf("file open error: %s\n", str);
				printf("retry[0] manual input[1]:");
				while((!file_or_manual)&&(exit!=1 || exit!=2))
				{
					//scanf("%d",)
				}
				
			}
			else
			{
				
			}
		}
		*/

		//////이전 코드들. 다른 코드 작성을 위해 주석 처리해뒀습니다.
		/*
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

		while (1) {
			char s = 0;
			s1 = 0;
			PROC* cpy_procs = (PROC*)malloc(sizeof(PROC) * num_proc);
			for (int i = 0; i < num_proc; i++) {
				cpy_procs[i] = procs[i];
			}
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
			}
			else if (s == '2') {
				printf("SJF Selected!\n");
			}
			else if (s == '3') {
				printf("SRTF Selected!\n");
			}
			else if (s == '4') {
				printf("RR Selected!\n");
				RR(cpy_procs, num_proc, tq);
			}
			else if (s == '5') {
				printf("NON-Preemptive Priority Selected!\n");
				np_priority(cpy_procs, num_proc);
			}
			else if (s == '6') {
				printf("Preemptive Selected!\n");
			}
			else if (s == '7') {
				printf("Non-Preemptive Priority with RR Selected!\n");
			}

			free(cpy_procs);

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
		free(procs);
		if (s1 == '3')
			break;
		*/
		
		
	}
	return 0;
	free(procs);
}
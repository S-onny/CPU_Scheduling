#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
void backspace(int a)        // 두자릿수 이상의 숫자가 출력될 경우 backspace를 출력하는 함수
{
    int x, y, z;
    x = a;
    y = 0;

    while (x != 0)
    {
        x = x / 10;
        y++;
    }

    for (z = 0; z < y - 1; z++)
    {
        printf("\b");
    }
}


void Print_readyQueue(DATA* datum){
	         // Ready queue table    

        printf("Ready queue table\n");
		
        printf(" ------- ------------- ------------ \n");
        printf("| Time  | Ready queue | Remaintime |\n");
        printf(" ------- ------------- ------------ \n");
		int i,j,a,c,d,e;
		int gi=datum->gantt_index;
		int num_proc = datum->num_proc;
		int* p=datum->g_p;
		int* et=datum->g_et;
		int* bt=datum->g_bt;
	int *ct= (int*)malloc(sizeof(int) * (gi+1));
	int *pat= (int*)malloc(sizeof(int) * num_proc);
	int *pct= (int*)malloc(sizeof(int) * num_proc);
	int *pp= (int*)malloc(sizeof(int) * num_proc); 
	int *prem= (int*)malloc(sizeof(int) * num_proc);
		for (i=0;i<=(gi);i++){
			ct[i]=bt[i]+et[i];
		}
		for (i=0;i<datum->num_proc;i++){
			pat[i]=(datum->procs)[i].at;
			pct[i]=(datum->procs)[i].ct;
			pp[i]=(datum->procs)[i].p;
			prem[i]=(datum->procs)[i].bt;
		}

        for (a = 0; a <= ct[gi]; a++)             // 마지막 프로세스가 실행 종료될때까지의 시간 a
        {
            c = -1, e=-1;
            d = 0;
			for(j=0;j<=gi;j++){// 시간 a 일때 실행중인프로세스 pid찾기
					if (et[j] <= a && a <= ct[j]){
						c=p[j];
						e=j;
					}
				}
			printf("|  %d    ", a);
			backspace(a);
            for (i = 0; i < (datum->num_proc); i++)                  // 시간 a일때 대기중인 프로세스들을 출력하는 알고리즘
            {
				if(pp[i] == c) {
					prem[i] --;
					continue;
				}
                if (a >= pat[i] && a < pct[i])       // 시간 a 일때 도착+ 종료안됨+대기중이였던 프로세스들의 개수
                {
                    d++;
					if (d>1){
						printf("|       ");
					}
					printf("|     P%d      ", pp[i]);
                    backspace(p[e]);
					
                    printf("|     %d      ", prem[i]);
                    backspace(prem[i]);
                    printf("|\n");
                }
				
            }

            if (d == 0)                 // 대기중인 프로세스가 없을때
            {
                printf("|             |            |\n");
			}
			printf(" ------- ------------- ------------ \n");
        }
        // Ready queue table 끝
	free(ct);
	free(pat);
	free(pct);
	free(pp);
	free(prem);
}

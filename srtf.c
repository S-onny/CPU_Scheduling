#include <stdio.h>
#include <stdlib.h>
#define INT_MAX	2147483647
typedef struct {
	int at;
	int bt;
	int p;
	int et;
	int ct;
	int tat;
	int wt;
	int pri;
	int rem;
}PROC;
int compare_a(const void* p1, const void* p2) {//arrival 순서 퀵 정렬을 위한 compare 함수
	int proc_cmp;
	proc_cmp=((PROC*)p1)->at - ((PROC*)p2)->at;
	if (proc_cmp == 0) 
		return ((PROC*)p1)->p - ((PROC*)p2)->p;
	return proc_cmp;
}
int compare_p(const void* p1, const void* p2){//pid순으로 정렬하기위한 compare 함수
	return ((PROC*)p1)->p - ((PROC*)p2)->p;
}
int main()
{
    int num_proc = 0, i, j,k,n, gantt_index, tq, E,time;
    //int* at, * bt, * p, * et, * ct, * tat, * wt, * pri;
    double ttat = 0, twt = 0;
	
    // num_proc = number of processes
    // at = arrival time
    // bt = burst time
    // p = 프로세스 번호 저장
    // et = excution time 프로세스 실행이 시작되는 시간
    // ct = completion time 프로세스 실행이 끝나는 시간
    // tat = turnaround time
    // wt = waiting time
    // ttat = total turnaround time
    // twt = total waiting time
    // pri = priority
    // tq = time quantum
    // e = 마지막에 프로그램 실행 or 종료 
	
	//sort for arrival 
	

		printf("Enter the number of process: ");
        scanf("%d", &num_proc);
        printf("\n\n");
		PROC* procs= malloc(sizeof(PROC)*num_proc);
		int* p=malloc(sizeof(int)*num_proc);
		int* et=malloc(sizeof(int)*num_proc);
		int* bt=malloc(sizeof(int)*num_proc);

        printf("Enter the processes\n");
        printf("Processes\tArrival Time\tBurst time\n");
        for (i = 0; i < num_proc; i++) // at, bt 입력받고, 프로세스 번호 저장
        {
            printf("Process[%d]:\t\t", i + 1);
            scanf("%d %d", &(procs[i].at), &(procs[i].bt));
			procs[i].p = i + 1;
        }
		qsort(procs,num_proc,sizeof(PROC),compare_a);//도착 순서대로 정렬(같을시 프로세스 번호대로)
        printf("\n\n");
		for(i=0; i<num_proc;i++){
			procs[i].rem=procs[i].bt;
			procs[i].wt=0;
			procs[i].et=0-1;
			procs[i].ct=0-1;
			procs[i].tat=0-1;
		}
		time=0;
		gantt_index=0;
		int N=num_proc;
		while(N>0){
			int min_rem=INT_MAX;
			int min_rem_index=-1;
			for(i=0;i<N;i++){
				if(procs[i].at>time) break;
				if(procs[i].rem<min_rem){
					min_rem_index=i;
					min_rem=procs[i].rem;
				}
			}
			if(min_rem_index== -1){
				/*
				if(gantt_index==0&&bt[j]==0){
					p[gantt_index]=0;
					et[gantt_index]=time;
				}
				else{
					if (p[j] != 0) {
						if (sizeof(p)/(sizeof(int)) < gantt_index+2)
							p=(int*)realloc(p,sizeof(p)*2);
							et=(int*)realloc(et,sizeof(et)*2);
							bt=(int*)realloc(bt,sizeof(bt)*2);
						gantt_index++;
						p[gantt_index]=0;
						et[gantt_index]=time;
					}
				}
				(bt[gantt_index])++;
				*/
				time++;
				continue;
			}
			for(i=0;i<N;i++){
				if(procs[i].at>time) break;
				if(i == min_rem_index)
					continue;
				(procs[i].wt)++;
			}
			(procs[min_rem_index].rem)--;
			if(gantt_index==0&&bt[gantt_index]==0){
				p[gantt_index]=procs[min_rem_index].p;
				et[gantt_index]=time;
			}
			else{
				if (p[gantt_index] != procs[min_rem_index].p) {
					if (sizeof(p)/(sizeof(int)) < gantt_index+2)
						p=(int*)realloc(p,sizeof(p)*2);
						et=(int*)realloc(et,sizeof(et)*2);
						bt=(int*)realloc(bt,sizeof(bt)*2);
					gantt_index++;
					p[gantt_index]=procs[min_rem_index].p;
					et[gantt_index]=time;
				}
			}
				(bt[gantt_index])++;
			if(procs[min_rem_index].et == -1) procs[min_rem_index].et=time;
			if(procs[min_rem_index].rem == 0){
				procs[min_rem_index].ct=time+1;
				procs[min_rem_index].tat=procs[min_rem_index].ct - procs[min_rem_index].at;
				PROC temp = procs[N-1];
				procs[N-1]=procs[min_rem_index];
				procs[min_rem_index]=temp;
				N=N-1;
				if(N==0) {
					time++;
					break;
					}
				qsort(procs,N,sizeof(PROC),compare_a);
			}
			time++;
		}
		qsort(procs,num_proc,sizeof(PROC),compare_p);
		for(i=0;i<num_proc;i++){
			twt += procs[i].wt;
			ttat += procs[i].tat;
		}
	    printf("-------------------------------------------------------------------\n");
        printf(" Processes\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
        printf("-------------------------------------------------------------------\n");
        for (i = 0; i < num_proc; i++)
        {
            printf(" Process[%d]\t[%d]\t[%d]\t[%d]\t\t[%d]\t[%d]\n", procs[i].p, procs[i].at, procs[i].bt, procs[i].ct, procs[i].wt, procs[i].tat);

        }
        printf("-------------------------------------------------------------------\n");
        printf("Average waiting time : %.2f\n", twt / num_proc);
        printf("Average turnaround time : %.2f\n", ttat / num_proc);
		
		// 간트 차트 그리기
        printf("\n\n");
        printf("Gantt Chart");
        printf("\n");


        for (i = 1; i <= gantt_index+1; i++) // 윗줄
        {
            printf(" ----");

            for (j = 0; j < bt[i - 1]; j++)
            {
                printf("-");
            }

            n = i - 1;
            if (et[i] > et[n]+bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 윗줄 "-" 
            {
                printf(" -");
                for (k = 0; k < et[i] - (et[n]+bt[n]); k++)
                {
                    printf("-");    
                }
            }
        }





        printf("\n");

        for (i = 1; i <= gantt_index+1; i++) // 가운데 줄
        {

            printf("| P%d ", p[i - 1]);

            for (j = 0; j < bt[i - 1]; j++)
            {
                printf(" ");
            }

            n = i - 1;
            if (et[i] > et[n]+bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 가운데 줄의 공백
            {
                printf("| ");
                for (k = 0; k < (et[i] - (et[n]+bt[n])); k++)
                {
                    printf(" ");
                }
            }
        }



        printf("|");
        printf("\n");

        for (i = 1; i <= gantt_index+1; i++) // 아랫줄
        {
            printf(" ----");

            for (j = 0; j < bt[i - 1]; j++)
            {
                printf("-");
            }

            n = i - 1;
            if (et[i] > et[n]+bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 아랫줄 "-"
            {
                printf(" -");
                for (k = 0; k < (et[i] - (et[n]+bt[n])); k++)
                {
                    printf("-");
                }
            }
        }


        printf("\n");

        int a, b, c, d, e, f;
        for (i = 1; i <= gantt_index+1; i++) // 아랫줄 밑 실행시간
        {
            printf("%d    ", et[i - 1]);
            for (j = 0; j < bt[i - 1]; j++)
            {
                printf(" ");
            }

            a = et[i - 1];                                  // 자릿수만큼 공백 빼기
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

            n = i - 1;                              // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간 밑의 실행시간 사이의 공백
            if (et[i] > et[n]+bt[n])
            {
                printf("%d ", et[n]+bt[n]);
                for (k = 0; k < (et[i] - (et[n]+bt[n])); k++)
                {
                    printf(" ");
                }

                d = et[n]+bt[n];                              // 자릿수만큼 공백 빼기
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



        printf("%d", et[gantt_index]+bt[gantt_index]);
        printf("\n");
        // 간트 차트 끝
	free(p);
	free(et);
	free(bt);
	free(procs);
	
}
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
    int num_proc = 0, i, j, tq, e,time;
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
		j=0;
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
				printf("time:%d, IDLE\n",time);
				/*
				if(j==0&&bt[j]==0){
					p[j]=0;
					et[j]=time;
				}
				else{
					if (p[j] != 0) {
						if (sizeof(p)/(sizeof(int)) < j+2)
							p=(int*)realloc(p,sizeof(p)*2);
							et=(int*)realloc(et,sizeof(et)*2);
							bt=(int*)realloc(bt,sizeof(bt)*2);
						j++;
						p[j]=0;
						et[j]=time;
					}
				}
				(bt[j])++;
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
			printf("time:%d, process%d\n",time,procs[min_rem_index].p);
			if(j==0&&bt[j]==0){
				p[j]=procs[min_rem_index].p;
				et[j]=time;
			}
			else{
				if (p[j] != procs[min_rem_index].p) {
					if (sizeof(p)/(sizeof(int)) < j+2)
						p=(int*)realloc(p,sizeof(p)*2);
						et=(int*)realloc(et,sizeof(et)*2);
						bt=(int*)realloc(bt,sizeof(bt)*2);
					j++;
					p[j]=procs[min_rem_index].p;
					et[j]=time;
				}
			}
				(bt[j])++;
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
		for (i=0;i<j+1;i++){
			printf("%d: p%d start:%d burst:%d\n",i,p[i],et[i],bt[i]);
		}
	
	free(p);
	free(et);
	free(bt);
	free(procs);
	
}
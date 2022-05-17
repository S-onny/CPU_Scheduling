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

#include <stdio.h>
#include <stdlib.h>

int num_proc = 0, tq, e, min, n, m, k, ttat, twt;
int *at, *bt, *p, *et, *ct, *tat, *wt, *pri;

int main()
{
    printf("Enter the number of Processes : ");
    scanf("%d", &num_proc);

    at = (int *)malloc(sizeof(int) * num_proc);
    bt = (int *)malloc(sizeof(int) * num_proc);
    p = (int *)malloc(sizeof(int) * num_proc);
    pri = (int *)malloc(sizeof(int) * num_proc);

    et = (int *)malloc(sizeof(int) * num_proc);
    ct = (int *)malloc(sizeof(int) * num_proc);
    tat = (int *)malloc(sizeof(int) * num_proc);
    wt = (int *)malloc(sizeof(int) * num_proc);

    printf("\t\t\tProcesses\n");
    printf("Processes\tArrivalTime BurstTime Priority\n");

    int i = 0, j = 0;
    for (i = 0; i < num_proc; i++)
    {
        p[i] = i;
        printf("process[%d]:\t\t", i + 1);
        scanf("%d%d%d", &at[i], &bt[i], &pri[i]);
    }


    printf("\nInput Time Qunatum : ");
    scanf("%d", &tq);

    


    return 0;
}
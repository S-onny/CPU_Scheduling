#include <stdio.h>
#include <stdlib.h>

int main()
{
    int num_proc, i, j, tq;
    int* at, * bt, * p, * et, * ct, * tat, * wt, * pri;
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

    printf("Enter the number of process: ");
    scanf("%d", &num_proc);
    printf("\n\n");
    at = malloc(sizeof(int) * num_proc); // arrival time 저장공간
    bt = malloc(sizeof(int) * num_proc); // brust time 저장공간
    p = malloc(sizeof(int) * num_proc); // 프로세스 번호 저장공간
    et = malloc(sizeof(int) * num_proc); // 실행시간 저장공간
    ct = malloc(sizeof(int) * num_proc); // completion time 저장공간
    tat = malloc(sizeof(int) * num_proc); // turnaround time 저장공간
    wt = malloc(sizeof(int) * num_proc); // waiting time 저장공간

    printf("Enter the processes\n");
    printf("Processes\tArrival Time\tBurst time\n");
    for (i = 0; i < num_proc; i++) // at, bt 입력받고, 프로세스 번호 저장
    {
        printf("Process[%d]:\t\t", i + 1);
        scanf("%d %d", &at[i], &bt[i]);
        p[i] = i + 1;
    }
    printf("\n\n");

    int min, temp;
    for (i = 0; i < num_proc - 1; i++) // at 배열의 순서를 정리하고, p와 bt의 순서도 at와 동일하게 정리
    {
        min = i;
        for (j = i + 1; j < num_proc; j++)
        {
            if (at[min] > at[j])
            {

                temp = at[min];
                at[min] = at[j];
                at[j] = temp;

                temp = p[min];
                p[min] = p[j];
                p[j] = temp;

                temp = bt[min];
                bt[min] = bt[j];
                bt[j] = temp;

                min = j;
            }
        }
    }

    for (i = 0; i < num_proc; i++) // 도착시간을 실행시간에 저장
    {
        et[i] = at[i];
    }

    for (i = 1; i < num_proc; i++) // n번째 도착한 프로세스의 실행시간 = n-1번째 도착한 프로세스의 실행시간(or 도착시간) + burst time
    {
        j = i - 1;
        if (et[j] < et[i] < et[j] + bt[j])
        {
            et[i] = et[j] + bt[j];
        }
    }

    for (i = 0; i < num_proc; i++) // completion time 구하기
    {
        ct[i] = et[i] + bt[i];
    }

    for (i = 0; i < num_proc; i++) // turnaround time, waiting time, total turnaround time, total waiting time 구하기
    {
        tat[i] = ct[i] - at[i];
        wt[i] = et[i] - at[i];
        ttat += tat[i];
        twt += wt[i];
    }

    printf("==================================================================\n");
    printf("Processes\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
    printf("------------------------------------------------------------------\n");
    for (i = 0; i < num_proc; i++)
    {
        printf("Process[%d]\t[%d]\t[%d]\t[%d]\t\t[%d]\t[%d]\n", p[i], at[i], bt[i], ct[i], wt[i], tat[i]);

    }
    printf("Average waiting time : %.2f\n", twt / num_proc);
    printf("Average turnaround time : %.2f\n", ttat / num_proc);



    // 간트 차트 그리기
    printf("\n\n\n\n");
    printf("Gantt Chart");
    printf("\n");

    for (i = 0; i < num_proc; i++) // 맨 윗줄
    {
        printf(" ----");
        
        for (j = 0; j < bt[i]; j++)
        {
            printf("-");
        }
    }
    printf("\n");

    for (i = 0; i < num_proc; i++) // 가운뎃줄
    {
        printf("| P%d ", p[i]);
        
        for (j = 0; j < bt[i]; j++)
        {
            printf(" ");
        }
    }
    printf("|");
    printf("\n");

    for (i = 0; i < num_proc; i++) // 아랫줄
    {
        printf(" ----");
        
        for (j = 0; j < bt[i]; j++)
        {
            printf("-");
        }
    }
    printf("\n");

    for (i = 0; i < num_proc; i++) // 아랫줄 밑 실행시간
    {
        printf("%d    ", et[i]);
        for (j = 0; j < bt[i]; j++)
        {
            printf(" ");
        }
    }
    printf("\b");
    printf("%d", ct[num_proc-1]);
    printf("\n");
    // 간트 차트 끝


    free(at);
    free(bt);
    free(p);
    free(et);
    free(ct);
    free(tat);
    free(wt);

    return 0;
}

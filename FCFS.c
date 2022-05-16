#include <stdio.h>
#include <stdlib.h>


int main()
{
    int num_proc = 0, i, j, tq, e, min, temp, n, m, k;
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
    // e = 마지막에 프로그램 실행 or 종료 

    at = malloc(sizeof(int) * num_proc); // arrival time 저장공간
    bt = malloc(sizeof(int) * num_proc); // brust time 저장공간
    p = malloc(sizeof(int) * num_proc); // 프로세스 번호 저장공간
    et = malloc(sizeof(int) * num_proc); // 실행시간 저장공간
    ct = malloc(sizeof(int) * num_proc); // completion time 저장공간
    tat = malloc(sizeof(int) * num_proc); // turnaround time 저장공간
    wt = malloc(sizeof(int) * num_proc); // waiting time 저장공간


    while (1)
    {


        printf("Enter the number of process: ");
        scanf("%d", &num_proc);
        printf("\n\n");


        printf("Enter the processes\n");
        printf("Processes\tArrival Time\tBurst time\n");
        for (i = 0; i < num_proc; i++) // at, bt 입력받고, 프로세스 번호 저장
        {
            printf("Process[%d]:\t\t", i + 1);
            scanf("%d %d", &at[i], &bt[i]);
            p[i] = i + 1;
        }
        printf("\n\n");


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

        for (i = 1; i < num_proc; i++)     // n번째 도착한 프로세스의 실행시간 = n-1번째 도착한 프로세스의 실행시간(or 도착시간) + burst time
        {
            j = i - 1;
            if (et[j] <= et[i] && et[i] < et[j] + bt[j])
            {
                et[i] = et[j] + bt[j];
            }
            else if (et[i] < et[j])       // n번째 프로세스가 n-1번째 프로세스보다 일찍 도착했고, 다른 프로세스가 실행중일때 대기중이였던 경우
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
            if (wt[i] < 0)              // n번째 프로세스가 n-1번째 프로세스의 실행이 끝난 다음에 왔을 경우 wt가 음수로 나오는 오류 고침
                wt[i] = 0;
            ttat += tat[i];
            twt += wt[i];
        }

        printf("-------------------------------------------------------------------\n");
        printf(" Processes\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
        printf("-------------------------------------------------------------------\n");
        for (i = 0; i < num_proc; i++)
        {
            printf(" Process[%d]\t[%d]\t[%d]\t[%d]\t\t[%d]\t[%d]\n", p[i], at[i], bt[i], ct[i], wt[i], tat[i]);

        }
        printf("-------------------------------------------------------------------\n");
        printf("Average waiting time : %.2f\n", twt / num_proc);
        printf("Average turnaround time : %.2f\n", ttat / num_proc);



        // 간트 차트 그리기
        printf("\n\n");
        printf("Gantt Chart");
        printf("\n");


        for (i = 1; i <= num_proc; i++) // 윗줄
        {
            printf(" ----");

            for (j = 0; j < bt[i - 1]; j++)
            {
                printf("-");
            }

            n = i - 1;
            if (at[i] > ct[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 윗줄 "-" 
            {
                printf(" -");
                for (k = 0; k < (at[i] - ct[n]); k++)
                {
                    printf("-");       // 일부러 -- 두개 출력했습니다. 너무 짧으면 밑에 실행시간 출력될 공간 부족할까봐
                }
            }
        }





        printf("\n");

        for (i = 1; i <= num_proc; i++) // 가운데 줄
        {

            printf("| P%d ", p[i - 1]);

            for (j = 0; j < bt[i - 1]; j++)
            {
                printf(" ");
            }

            n = i - 1;
            if (at[i] > ct[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 가운데 줄의 공백
            {
                printf("| ");
                for (k = 0; k < (at[i] - ct[n]); k++)
                {
                    printf(" ");
                }
            }
        }



        printf("|");
        printf("\n");

        for (i = 1; i <= num_proc; i++) // 아랫줄
        {
            printf(" ----");

            for (j = 0; j < bt[i - 1]; j++)
            {
                printf("-");
            }

            n = i - 1;
            if (at[i] > ct[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 아랫줄 "-"
            {
                printf(" -");
                for (k = 0; k < (at[i] - ct[n]); k++)
                {
                    printf("-");
                }
            }
        }


        printf("\n");

        int a, b, c, d, e, f;
        for (i = 1; i <= num_proc; i++) // 아랫줄 밑 실행시간
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
            if (at[i] > ct[n])
            {
                printf("%d ", ct[n]);
                for (k = 0; k < (at[i] - ct[n]); k++)
                {
                    printf(" ");
                }

                d = ct[n];                              // 자릿수만큼 공백 빼기
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



        printf("%d", ct[num_proc - 1]);
        printf("\n");
        // 간트 차트 끝

        printf("\n\n");
        printf("What to do next?\n");
        printf("1. Continue\n");
        printf("2. Exit\n");
        printf("\n");
        printf("Do: ");
        scanf("%d", &e);

        if (e == 1)
        {
            printf("===================================================================\n");
            printf("\n\n\n\n");
        }
        else if (e == 2)
        {
            break;
        }

    }

    free(at);
    free(bt);
    free(p);
    free(et);
    free(ct);
    free(tat);
    free(wt);

    system("pause");
    return 0;
}

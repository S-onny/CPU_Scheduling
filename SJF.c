#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)
#define swap(x, y, temp) ( (temp)=(x), (x)=(y), (y)=(temp) )        // 배열 요소의 순서를 바꾸기 위한 함수

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


int main()
{
    int num_proc = 0, i, j, tq, e, min, temp, n, m, k;
    int* at, * bt, * p, * et, * ct, * tat, * wt, * pri;
    double ttat = 0, twt = 0;
    int a, b, c, d, f, g, h; // 기타 변수들
    int count = 0;
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


        for (i = 0; i < num_proc - 1; i++) // at의 순서를 정리하고, p와 bt의 순서도 at와 동일하게 정리
        {
            min = i;
            for (j = i + 1; j < num_proc; j++)
            {
                if (at[min] > at[j])
                {
                    min = j;
                }
            }

            if (i != min)
            {
                swap(at[i], at[min], temp);
                swap(p[i], p[min], temp);
                swap(bt[i], bt[min], temp);
            }
        }


        for (i = 0; i < num_proc; i++) // 도착시간을 실행시간에 저장
        {
            et[i] = at[i];
        }




        for (i = 0; i < num_proc - 1; i++)              // 같은 시간에 도착한 프로세스들을 bt에 대해서 정리
        {
            count = 0;
            for (j = i + 1; j < num_proc; j++)
            {
                if (at[i] == at[j])                    // 같은 시간에 도착한 프로세스들의 개수
                {
                    count++;
                }
            }


            if (count != 0)
            {
                c = i + count;
                for (a = i; a < c; a++)         // i에서부터 i + count까지의 프로세스들을 bt에 대해서 정리
                {                               // 이때 count는 같은 시간에 도착한 프로세스의 개수이다.
                    min = a;
                    for (b = a + 1; b <= c; b++)
                    {


                        if (bt[min] > bt[b])
                        {
                            min = b;
                        }
                    }

                    if (a != min)
                    {
                        swap(at[a], at[min], temp);
                        swap(p[a], p[min], temp);
                        swap(bt[a], bt[min], temp);
                        swap(et[a], et[min], temp);
                    }
                }
            }
            i = i + count;             // 정리가 끝난 프로세스들의 다음부터 다시 정리 시작
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




        for (i = 0; i < num_proc - 1; i++)              // 대기중인 프로세스들을 bt에 대해서 정리하기
        {
            count = 0;
            for (j = i + 1; j < num_proc; j++)
            {
                if (et[i] <= at[j] && at[j] < et[i] + bt[i])         // i 번째 프로세스가 실행중이여서 대기중인 프로세스들의 개수 세기
                {
                    count++;
                }
            }


            if (count != 0)                 // 대기중인 프로세스의 개수가 0이 아닐때, 그 프로세스들을 bt에 대해서 정리
            {
                c = i + count;
                d = i;


                b = 0; // 아래 세가지 조건 중 해당하는 것이 있는지 알아보기 위한 변수
                if (d == 0)     // 조건1. 가장 첫번째 프로세스
                {
                    b++;
                }

                if (et[d] == et[d + 1])     // 조건2. 같은 시간에 도착한 프로세스가 있을 경우, 그 중 첫번째 프로세스
                {
                    b++;
                }

                if (et[d + 1] >= et[d] + bt[d])     // 조건3. 바로 앞 프로세스가 종료된 후에 도착한 프로세스
                {
                    b++;
                }

                if (b != 0)             // 위 세가지 조건 중 하나라도 해당되면 그 프로세스 제외
                {
                    d = d + 1;
                }


                for (a = d; a < c; a++)       // 대기중인 프로세스들을 bt에 대해서 정리
                {
                    min = a;
                    for (b = a + 1; b <= c; b++)
                    {
                        if (bt[min] > bt[b])
                        {
                            min = b;
                        }
                    }

                    if (a != min)
                    {
                        swap(at[a], at[min], temp);
                        swap(p[a], p[min], temp);
                        swap(bt[a], bt[min], temp);
                        swap(et[a], et[min], temp);
                    }
                }
            }
            i = i + count;               // 정리가 끝난 프로세스들의 다음부터 다시 정리 시작
        }


        for (i = 1; i < num_proc; i++)     // bt에 대해서 정리한 프로세스들의 et를 다시 정리
        {
            j = i - 1;
            if (et[j] <= at[i] && at[i] < et[j] + bt[j])
            {
                et[i] = et[j] + bt[j];
            }
            else if (at[i] < et[j])
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
            if (wt[i] < 0)              // n번째 프로세스가 n-1번째 프로세스의 실행이 끝난 다음에 왔을 경우 wt가 음수로 나오는것을 0으로 표시
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
        printf("\n\n\n");
        printf("Gantt Chart");
        printf("\n");


        for (i = 1; i <= num_proc; i++) // 윗줄
        {
            printf(" ----");

            for (j = 0; j < bt[i - 1]; j++)
            {
                printf("-");
            }

            if (at[i] > ct[i - 1])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 윗줄 "-" 
            {
                printf(" -");
                for (k = 0; k < (at[i] - ct[i - 1]); k++)
                {
                    printf("-");
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

            backspace(p[i - 1]);        // 프로세스 번호의 자릿수만큼 빼기

            if (at[i] > ct[i - 1])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 가운데 줄의 공백
            {
                printf("| ");
                for (k = 0; k < (at[i] - ct[i - 1]); k++)
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

            if (at[i] > ct[i - 1])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 아랫줄 "-"
            {
                printf(" -");
                for (k = 0; k < (at[i] - ct[i - 1]); k++)
                {
                    printf("-");
                }
            }
        }


        printf("\n");


        for (i = 1; i <= num_proc; i++) // 아랫줄 밑 실행시간
        {
            printf("%d    ", et[i - 1]);
            for (j = 0; j < bt[i - 1]; j++)
            {
                printf(" ");
            }

            backspace(et[i - 1]);

            if (at[i] > ct[i - 1])                  // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간 밑의 실행시간 사이의 공백
            {
                printf("%d ", ct[i - 1]);
                for (k = 0; k < (at[i] - ct[i - 1]); k++)
                {
                    printf(" ");
                }

                backspace(ct[i - 1]);

            }

        }

        printf("%d", ct[num_proc - 1]);
        printf("\n");
        // 간트 차트 끝

        printf("\n\n\n");







        // Ready queue table    

        printf("Ready queue table\n");
        printf(" ------- ------------- ------------ \n");
        printf("| Time  | Ready queue | Burst time |\n");
        printf(" ------- ------------- ------------ \n");

        for (a = 0; a <= ct[num_proc - 1]; a++)             // 마지막 프로세스가 실행 종료될때까지의 시간 a
        {
            c = 0;
            d = 0;
            for (i = 0; i < num_proc; i++)                  // 시간 a일때 대기중인 프로세스들을 출력하는 알고리즘
            {
                if (et[i] <= a && a < et[i] + bt[i])         // 시간 a 일때 실행중인 i번째 프로세스
                {
                    c = i + 1;
                }
                else if (a >= at[i] && a < et[i])        // 시간 a 일때 도착은 했지만 대기중이였던 프로세스들의 개수
                {
                    d++;
                }
            }

            if (d == 0)                 // 대기중인 프로세스가 없을때
            {
                printf("|  %d    ", a);
                backspace(a);
                printf("|             |            |\n");
                printf(" ------- ------------- ------------ \n");
            }

            if (d != 0)             // 대기중인 프로세스가 있을때
            {
                for (e = c; e < c + d; e++)
                {
                    if (e == c)
                    {
                        printf("|  %d    ", a);
                        backspace(a);
                    }
                    else if (e != c)
                    {
                        printf("|       ");
                    }
                    printf("|     P%d      ", p[e]);
                    backspace(p[e]);
                    printf("|     %d      |\n", bt[e]);
                    backspace(bt[e]);
                }
                printf(" ------- ------------- ------------ \n");
            }
        }
        // Ready queue table 끝


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
        else
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

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#define INT_MAX	2147483647//int의 최댓값 define
typedef struct {
	int at;//arrival time
	int bt;//burst time
	int p;//pid
	int et;//first execute time
	int ct;//complete time
	int tat;//turnaround time
	int wt;//wait time
	int pri;//priority
	int rem;//remain burst time
}PROC;//구조체 선언 
int compare_a(const void* p1, const void* p2) {//arrival 순서 퀵 정렬을 위한 compare 함수
	int proc_cmp;
	proc_cmp = ((PROC*)p1)->at - ((PROC*)p2)->at;
	if (proc_cmp == 0)
		return ((PROC*)p1)->p - ((PROC*)p2)->p;
	return proc_cmp;
}
int compare_p(const void* p1, const void* p2) {//pid순으로 정렬하기위한 compare 함수
	return ((PROC*)p1)->p - ((PROC*)p2)->p;
}
int main()
{
	int num_proc = 0, i, j, k, n, gantt_index, E, tq, time;
	double ttat = 0, twt = 0;
	// ijkn: 반복문을 위한 변수, gantt_index:간트차트를 그리기 위한 배열들의 index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// tq = time quantum
	// time: 프로세싱 시각
	// E:무한루프 탈출
	PROC* procs = (PROC*)malloc(sizeof(PROC));//프로세스 구조체 배열
	int* g_p = (int*)malloc(sizeof(int));//간트차트를 그리기 위한 동적배열들
	int* g_et = (int*)malloc(sizeof(int));//간트차트를 그리기 위한 동적배열들
	int* g_bt = (int*)malloc(sizeof(int));//간트차트를 그리기 위한 동적배열들
	int min_pri = INT_MAX;//실행가능한 프로세스 중 가장 짧은 남은시간
	int min_pri_index = -1;// 실행가능한 프로세스중 가장 remain time이 짧은 프로세스의 index

	while (1)//무한반복:탈출
	{
		int index = -1;
		printf("Enter the number of process: ");
		scanf("%d", &num_proc);
		printf("\n\n");
		procs = (PROC*)realloc(procs, sizeof(PROC)*num_proc);//각 동적배열들 크기 재지정
		g_p = (int*)realloc(g_p, sizeof(int));
		g_et = (int*)realloc(g_et, sizeof(int));
		g_bt = (int*)realloc(g_bt, sizeof(int));

		printf("Enter the processes\n");
		printf("Processes\tArrival Time\tBurst time\tPriority\n");
		for (i = 0; i < num_proc; i++) // at, bt 입력받고, 프로세스구조체 배열에 pid와 함께 저장
		{
			printf("Process[%d]:\t\t", i + 1);
			scanf("%d %d %d", &(procs[i].at), &(procs[i].bt), &(procs[i].pri));
			procs[i].p = i + 1;
		}


		qsort(procs, num_proc, sizeof(PROC), compare_a);//도착 순서대로 정렬(같을시 프로세스 번호대로)
		printf("\n\n");


		for (i = 0; i<num_proc; i++) {
			procs[i].rem = procs[i].bt;//남은시간 burst time으로 초기화
			procs[i].wt = 0;//기타 필요한 수치들 초기화
			procs[i].et = 0 - 1;//최초 실행시간 구분을 위해 -1로 초기화
			procs[i].ct = 0;
			procs[i].tat = 0;
		}


		time = 0;
		gantt_index = 0;
		int N = num_proc;//N:실행이 끝나지 않은 프로세스 수. 입력된 프로세스수를 복사하여 초기화
		while (N>0) { //종료조건: line136 
		
			if (index == -1) {
				min_pri = INT_MAX;//최솟값을 찾기 위해 무한 대신 int가 가질 수 있는 최곳값으로 초기화
				min_pri_index = -1;// -1로 초기화. 현재 상태가 idle인지 구분하기 위함.
				for (i = 0; i < N; i++) {//priority 가장 높은 거 찾기
					if (procs[i].at > time) break;
					if (procs[i].pri < min_pri) {
						min_pri_index = i;
						min_pri = procs[i].pri;
					}
				}
				if (min_pri_index == -1) {//현재 실행가능한 프로세스 없음(idle)
										  /*//이부분은 다른형태의 간트차트를 그리기 위한 예비 코드임
										  if(gantt_index==0&&g_bt[j]==0){
										  g_p[gantt_index]=0;
										  g_et[gantt_index]=time;
										  }
										  else{
										  if (g_p[j] != 0) {
										  if (sizeof(g_p)/(sizeof(int)) < gantt_index+2)
										  g_p=(int*)realloc(g_p,sizeof(g_p)*2);
										  g_et=(int*)realloc(g_et,sizeof(g_et)*2);
										  g_bt=(int*)realloc(g_bt,sizeof(g_bt)*2);
										  gantt_index++;
										  g_p[gantt_index]=0;
										  g_et[gantt_index]=time;
										  }
										  }
										  (g_bt[gantt_index])++;
										  */
					time++;//시간흐름
					continue;
				}
				g_p[gantt_index] = procs[min_pri_index].p;
				g_bt[gantt_index] = 0;
				g_et[gantt_index] = time;
				
				g_p = (int*)realloc(g_p, (2+gantt_index)*sizeof(int));
				g_et = (int*)realloc(g_et, (2 + gantt_index)*sizeof(int));
				g_bt = (int*)realloc(g_bt, (2 + gantt_index)*sizeof(int));
				printf("저장%d %d %d %d\n", gantt_index, g_p[gantt_index], g_bt[gantt_index], g_et[gantt_index]);
				index = 0;
			}

			for (i = 0; i<N; i++) {//실행이 끝나지 않은 프로세스중 실행되지 않은 프로세스들의 대기시간 증가
				if (procs[i].at>time) break;
				if (i == min_pri_index)
					continue;
				(procs[i].wt)++;
			}
			(procs[min_pri_index].rem)--;//실행중인 프로세스의 remain time 감소
			(g_bt[gantt_index])++;

			if (procs[min_pri_index].et == -1) procs[min_pri_index].et = time;//프로세스가 최초로 실행된 시간을 et에 저장
			if (procs[min_pri_index].rem == 0)
			{	//프로세스가 실행종료된 경우: 가장 마지막 index의 프로세스와 현재 프로세스 swap.
				procs[min_pri_index].ct = time + 1;
				procs[min_pri_index].tat = procs[min_pri_index].ct - procs[min_pri_index].at;
				PROC temp = procs[N - 1];
				procs[N - 1] = procs[min_pri_index];
				procs[min_pri_index] = temp;
				N = N - 1;//실행가능한 프로세스 수 감소
				if (N == 0) {//모든 프로세스가 실행종료된 경우 반복문 탈출
					time++;
					break;
				}
				gantt_index++;
				index = -1;
				qsort(procs, N, sizeof(PROC), compare_a);//실행가능한 프로세스들을 다시 arrival순서로 정렬.
			}

			time++;//시간이 흐름
		}
		//프로세싱 종료

		qsort(procs, num_proc, sizeof(PROC), compare_p);//procs를 다시 pid순으로 정렬

		for (i = 0; i<num_proc; i++) {//twt ttat 계산
			twt += procs[i].wt;
			ttat += procs[i].tat;
		}

		//print table
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
		printf("Gantt Chart %d %d",gantt_index, g_p[gantt_index]);
		printf("\n");
		
		for (i = 1; i <=gantt_index + 1; i++) // 윗줄
		{
			printf(" ----");
			n = i - 1;
			for (j = 0; j < g_bt[n]; j++)
			{
				printf("-");
			}
			
			
			
			if (i!= gantt_index + 1 &&g_et[i] > g_et[n] + g_bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 윗줄 "-" 
			{
				
				printf(" -");
				for (k = 0; k < g_et[i] - (g_et[n] + g_bt[n]); k++)
				{
					printf("-");
				}
			}
		}





		printf("\n");

		for (i = 1; i <= gantt_index + 1; i++) // 가운데 줄
		{
			n = i - 1;

			printf("| P%d ", g_p[n]);

			for (j = 0; j < g_bt[n]; j++)
			{
				printf(" ");
			}

			
			if (i != gantt_index + 1&&g_et[i] > g_et[n] + g_bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 가운데 줄의 공백
			{
				printf("| ");
				for (k = 0; k < (g_et[i] - (g_et[n] + g_bt[n])); k++)
				{
					printf(" ");
				}
			}
		}



		printf("|");
		printf("\n");

		for (i = 1; i <= gantt_index + 1; i++) // 아랫줄
		{
			printf(" ----");

			for (j = 0; j < g_bt[i - 1]; j++)
			{
				printf("-");
			}

			n = i - 1;
			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])          // 프로세스 사이에 빈 시간이 있을경우에 출력되는 간트차트의 빈 공간의 아랫줄 "-"
			{
				printf(" -");
				for (k = 0; k < (g_et[i] - (g_et[n] + g_bt[n])); k++)
				{
					printf("-");
				}
			}
		}


		printf("\n");

		int a, b, c, d, e, f;
		for (i = 1; i <= gantt_index + 1; i++) // 아랫줄 밑 실행시간
		{
			printf("%d    ", g_et[i - 1]);
			for (j = 0; j < g_bt[i - 1]; j++)
			{
				printf(" ");
			}

			a = g_et[i - 1];                                  // 자릿수만큼 공백 빼기
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
			if (i != gantt_index + 1 && g_et[i] > g_et[n] + g_bt[n])
			{
				printf("%d ", g_et[n] + g_bt[n]);
				for (k = 0; k < (g_et[i] - (g_et[n] + g_bt[n])); k++)
				{
					printf(" ");
				}

				d = g_et[n] + g_bt[n];                              // 자릿수만큼 공백 빼기
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


		printf("%d", g_et[gantt_index] + g_bt[gantt_index]);
		printf("\n");
		// 간트 차트 끝

		//재실행?
		E = 0;//E초기화
		while (E != 1 && E != 2) {
			printf("Re do? : y:1/n:2 \n");
			scanf("%d", &E);
			getchar();
		}
		if (E == 2) break;
	}


	free(g_p);
	free(g_et);
	free(g_bt);
	free(procs);

}

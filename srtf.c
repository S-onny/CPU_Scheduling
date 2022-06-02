#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proc_data.h"
#define INT_MAX	2147483647//int의 최댓값 define
#define swap(x, y, temp) ( (temp)=(x), (x)=(y), (y)=(temp) ) 	// 배열 요소의 순서를 바꾸기 위한 함수

int compare_a(const void* p1, const void* p2);
int compare_p(const void* p1, const void* p2);
void SRTF(DATA* data)
{
	int num_proc=(data->num_proc);
	PROC* procs=(data->procs);
    int i,j,k,n, gantt_index,E, tq,time;
    double ttat = 0, twt = 0;
	// ijkn: 반복문을 위한 변수, gantt_index:간트차트를 그리기 위한 배열들의 index
    // num_proc = number of processes
    // ttat = total turnaround time
    // twt = total waiting time
    // tq = time quantum
	// time: 프로세싱 시각
	// E:무한루프 탈출
	int* g_p=(int*)malloc(sizeof(int)*num_proc);//간트차트를 그리기 위한 동적배열들
	int* g_et=(int*)malloc(sizeof(int)*num_proc);//간트차트를 그리기 위한 동적배열들
	int* g_bt=(int*)malloc(sizeof(int)*num_proc);//간트차트를 그리기 위한 동적배열들
	int gantt_capacity=num_proc;
	memset(g_bt,0,sizeof(int)*(gantt_capacity));
	int min_rem=INT_MAX;//실행가능한 프로세스 중 가장 짧은 남은시간
	int min_rem_index=-1;// 실행가능한 프로세스중 가장 remain time이 짧은 프로세스의 index

        qsort(procs, num_proc, sizeof(PROC), compare_a);//도착 순서대로 정렬(같을시 프로세스 번호대로)
		time=0;
		gantt_index=0;
		int N=num_proc;//N:실행이 끝나지 않은 프로세스 수. 입력된 프로세스수를 복사하여 초기화
		while(N>0){ //종료조건: line93
			min_rem=INT_MAX;//최솟값을 찾기 위해 무한 대신 int가 가질 수 있는 최곳값으로 초기화
			min_rem_index=-1;// -1로 초기화. 현재 상태가 idle인지 구분하기 위함.
			for(i=0;i<num_proc;i++){//실행이 끝나지 않은 프로세스들중 가장 남음시간이 짧은 프로세스의 index 찾기
				if(procs[i].at>time) break;//아직 도착하지 않은 프로세스는 연산하지 않음
				if(procs[i].c == 0) continue;//종료된 프로세스는 무시
				if(procs[i].rem<min_rem){
					min_rem_index=i;
					min_rem=procs[i].rem;
				}
			}
			if(min_rem_index== -1){//현재 실행가능한 프로세스 없음(idle)
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

			(procs[min_rem_index].rem)--;//실행중인 프로세스의 remain time 감소
			if(gantt_index==0&&g_bt[gantt_index]==0){//간트차트를 위한 동적 배열 채우기.첫번째로 실행되는 프로세스의 경우:
				g_p[gantt_index]=procs[min_rem_index].p;
				g_et[gantt_index]=time;
			}
			else{//첫번째로 실행되는 것이 아닌 프로세스의 경우
				if (g_p[gantt_index] != procs[min_rem_index].p) {//이전에 실행된 프로세스와 다른 프로세스일 경우: 다음 index에 저장
					if (gantt_capacity < gantt_index+2){//동적배열들의 할당크기가 부족할 경우 재할당
						gantt_capacity *=2;
						g_p=(int*)realloc(g_p,sizeof(int)*gantt_capacity);
						g_et=(int*)realloc(g_et,sizeof(int)*gantt_capacity);
						g_bt=(int*)realloc(g_bt,sizeof(int)*gantt_capacity);
						memset(&(g_bt[gantt_index+1]),0,sizeof(int)*(gantt_capacity-1-gantt_index));
					}
					gantt_index++;
					g_p[gantt_index]=procs[min_rem_index].p;
					g_et[gantt_index]=time;
				}
			}
				(g_bt[gantt_index])++;
			if(procs[min_rem_index].et == -1) procs[min_rem_index].et=time;//프로세스가 최초로 실행된 시간을 et에 저장
			if(procs[min_rem_index].rem == 0)
			{	//프로세스가 실행종료된 경우:
				procs[min_rem_index].ct=time+1;
				procs[min_rem_index].c=0;//is_completed!
				procs[min_rem_index].tat=procs[min_rem_index].ct - procs[min_rem_index].at;//turnaroundtime=completetime-arivaltime
				procs[min_rem_index].wt=procs[min_rem_index].tat - procs[min_rem_index].bt;//waitingtime=turnaroundtime-bursttime
				//no need to swap now!
				//PROC temp = procs[N-1];
				//procs[N-1]=procs[min_rem_index];
				//procs[min_rem_index]=temp;
				//swap(procs[N-1],procs[min_rem_index],temp);
				N=N-1;//실행가능한 프로세스 수 감소
				
				if(N==0) {//모든 프로세스가 실행종료된 경우 반복문 탈출
					time++;
					break;
					}
			}
			
			time++;//시간이 흐름
		}
		//프로세싱 종료
		
		//data에 프로세스 후 변경내용 저장
		qsort(procs,num_proc,sizeof(PROC),compare_p);//procs를 다시 pid순으로 정렬
		(data->g_p)=g_p;
		(data->g_et)=g_et;
		(data->g_bt)=g_bt;
		(data->gantt_index)=gantt_index;
}
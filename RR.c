#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"

int compare_a(const void* p1, const void* p2);
int compare_p(const void* p1, const void* p2);
void push(int* front, int* rear, int max, int value, int queue[]) {

	*rear = (*rear + 1) % max;
	queue[*rear] = value;
}
int pop(int* front, int* rear, int max, int queue[]) {
	if (*rear == *front)
		return -1;
	*front = (*front + 1) % max;

	return queue[*front];
}

void RR(DATA* data)
{
	int num_proc=(data->num_proc);
	PROC* procs=(data->procs);
	int tq =(data->tq);

	int n, gantt_index, time;
	double ttat = 0, twt = 0;
	// ijkn: ¹İº¹¹®À» À§ÇÑ º¯¼ö, gantt_index:°£Æ®Â÷Æ®¸¦ ±×¸®±â À§ÇÑ ¹è¿­µéÀÇ index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// time: ÇÁ·Î¼¼½Ì ½Ã°¢
	// E:¹«ÇÑ·çÇÁ Å»Ãâ
	int front = -1;
	int rear = -1;
	int* queue = (int*)malloc((num_proc+1) * sizeof(int)); //´ë±âÅ¥
	int* g_p = (int*)malloc( sizeof(int));//°£Æ®Â÷Æ®¸¦ ±×¸®±â À§ÇÑ µ¿Àû¹è¿­µé
	int* g_et = (int*)malloc( sizeof(int));//°£Æ®Â÷Æ®¸¦ ±×¸®±â À§ÇÑ µ¿Àû¹è¿­µé
	int* g_bt = (int*)malloc(sizeof(int));//°£Æ®Â÷Æ®¸¦ ±×¸®±â À§ÇÑ µ¿Àû¹è¿­µé
	int index = 0;		//Pop¿©ºÎ¿¡ ´ëÇÑ ÀÎµ¦½º
	int queue_index = -1;	//ÇöÀç CPUÁ¡À¯ÁßÀÎ ÇÁ·Î¼¼½º ÀÎµ¦½º
	int at_index = 0;	//µµÂøÇÑ ÇÁ·Î¼¼½º ÀÎµ¦½º

	qsort(procs, num_proc, sizeof(PROC), compare_a); //µµÂø ¼ø¼­´ë·Î Á¤·Ä(°°À»½Ã ÇÁ·Î¼¼½º ¹øÈ£´ë·Î)


		time = 0;
		gantt_index = 0;
		int N = num_proc;//N:½ÇÇàÀÌ ³¡³ªÁö ¾ÊÀº ÇÁ·Î¼¼½º ¼ö. ÀÔ·ÂµÈ ÇÁ·Î¼¼½º¼ö¸¦ º¹»çÇÏ¿© ÃÊ±âÈ­

		if (at_index < num_proc && 0 == procs[at_index].at) {	//Ã³À½ µµÂøÇÑ ÇÁ·Î¼¼½º Çª½¬
			push(&front, &rear, num_proc + 1, at_index, queue);
			at_index++;

		}

		while (N > 0) { //Á¾·áÁ¶°Ç: line136 

			if (index == 0) {	//ÀÎµ¦½º°¡ 0ÀÌ¸é popÇÏ°í ÃÊ±âÈ­
			
				queue_index = pop(&front, &rear, num_proc + 1, queue);
				
				if (queue_index == -1) {//ÇöÀç ½ÇÇà°¡´ÉÇÑ ÇÁ·Î¼¼½º ¾øÀ½(idle)
										  /*//ÀÌºÎºĞÀº ´Ù¸¥ÇüÅÂÀÇ °£Æ®Â÷Æ®¸¦ ±×¸®±â À§ÇÑ ¿¹ºñ ÄÚµåÀÓ
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
					time++;//½Ã°£Èå¸§
					continue;
				}
				if (procs[queue_index].p == g_p[gantt_index - 1]) {	//Àü ÇÁ·Î¼¼½º¶û Çö ÇÁ·Î¼¼½º µ¿ÀÏÇÏ¸é °£Æ®ÀÎµ¦½º À¯Áö
					gantt_index--;
				}
				else {
					g_p[gantt_index] = procs[queue_index].p;
					g_bt[gantt_index] = 0;
					g_et[gantt_index] = time;
				}
				
				g_p = (int*)realloc(g_p, (2 + gantt_index) * sizeof(int));
				g_et = (int*)realloc(g_et, (2 + gantt_index) * sizeof(int));
				g_bt = (int*)realloc(g_bt, (2 + gantt_index) * sizeof(int));
				index = 0;
			}

			for (int i = 0; i < num_proc; i++) {	//½ÇÇàÀÌ ³¡³ªÁö ¾ÊÀº ÇÁ·Î¼¼½ºÁß ½ÇÇàµÇÁö ¾ÊÀº ÇÁ·Î¼¼½ºµéÀÇ ´ë±â½Ã°£ Áõ°¡
				if (i != queue_index && procs[i].at <= time && (procs[i].c == -1))
					(procs[i].wt)++;
			}
			(procs[queue_index].rem)--;//½ÇÇàÁßÀÎ ÇÁ·Î¼¼½ºÀÇ remain time °¨¼Ò
			(g_bt[gantt_index])++;

			if (at_index < num_proc && time + 1 >= procs[at_index].at) {	//??i¡Æ¨Ïi?©« ??e¢®©«?¢¬i???¢¬i¢¶?
				push(&front, &rear, num_proc + 1, at_index, queue);
				at_index++;

			}

			if (procs[queue_index].et == -1) procs[queue_index].et = time;//ÇÁ·Î¼¼½º°¡ ÃÖÃÊ·Î ½ÇÇàµÈ ½Ã°£À» et¿¡ ÀúÀå
			if (procs[queue_index].rem == 0)	//½ÇÇà¿Ï·áµÇ¾î ÇÁ·Î¼¼½º ¹Ù²ãÁÖ±â
			{
				procs[queue_index].ct = time + 1;
				procs[queue_index].tat = procs[queue_index].ct - procs[queue_index].at;
				procs[queue_index].c = 0;
				N = N - 1;//½ÇÇà°¡´ÉÇÑ ÇÁ·Î¼¼½º ¼ö °¨¼Ò
				if (N == 0) {//¸ğµç ÇÁ·Î¼¼½º°¡ ½ÇÇàÁ¾·áµÈ °æ¿ì ¹İº¹¹® Å»Ãâ
					time++;
					break;
				}
				gantt_index++;
				index = 0;


			}
			else if (tq == index + 1)	//time-quantom¿¡ ÀÇÇØ ÇÁ·Î¼¼½º ¹Ù²ãÁÖ±â
			{

				push(&front, &rear, num_proc + 1, queue_index, queue);

				gantt_index++;
				index = 0;

			}
			else
				index++;

			time++;//?©«e¡Æ????e|?

		}
		//??e¢®©«?¢¬i??i¡Ë¡¦e¡Ì¨«

		qsort(procs, num_proc, sizeof(PROC), compare_p);//procse¡Í??¢´i?©« pid?©«?¨ùe? ????


		(data->g_p) = g_p;
		(data->g_et) = g_et;
		(data->g_bt) = g_bt;
		(data->gantt_index) = gantt_index;
		free(queue);
	

		Print_table(data);

		// e¡Æ???i¡Æ¡§i??e¡¤¢¬e|¡şe¢¬?
		Print_gantt(data);
		// e¡Æ???i¡Æ¡§i???

}
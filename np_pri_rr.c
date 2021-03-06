#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"
#include "Print_readyqueue.h"
#define INT_MAX 2147483647

void np_pri_rr(DATA *data)
{
	int num_proc = data->num_proc;
	PROC* procs = data->procs;
	int tq = (data->tq);

	int gantt_index, time;
	double ttat = 0, twt = 0;
	// ijkn: ë°˜ë³µë¬¸?„ ?„?œ ë³€?˜, gantt_index:ê°„?¸ì°¨?¸ë? ê·¸ë¦¬ê¸??„?œ ë°°ì—´ë“¤ì˜ index
	// num_proc = number of processes
	// ttat = total turnaround time
	// twt = total waiting time
	// time: ?„ë¡œ?¸ì‹??œê°
	// E:ë¬´í•œë£¨í”„ ?ˆì¶œ
	int front = -1;
	int rear = -1;
	int* queue = (int*)malloc((num_proc + 1) * sizeof(int)); //?€ê¸°í
	int* pri_queue= (int*)malloc((num_proc + 1) * sizeof(int));
	int* g_p = (int*)malloc(sizeof(int));//ê°„?¸ì°¨?¸ë? ê·¸ë¦¬ê¸??„?œ ?™?ë°°ì—´ë“?
	int* g_et = (int*)malloc(sizeof(int));//ê°„?¸ì°¨?¸ë? ê·¸ë¦¬ê¸??„?œ ?™?ë°°ì—´ë“?
	int* g_bt = (int*)malloc(sizeof(int));//ê°„?¸ì°¨?¸ë? ê·¸ë¦¬ê¸??„?œ ?™?ë°°ì—´ë“?
	int index = 0;		//Pop?¬ë?? ?€?œ ?¸ë±ìŠ?
	int queue_index = -1;	//?„??CPU?? ì¤‘???„ë¡œ?¸ìŠ??¸ë±ìŠ?
	int at_index = 0;	//?„ì°©í•œ ?„ë¡œ?¸ìŠ??¸ë±ìŠ?

	qsort(procs, num_proc, sizeof(PROC), compare_a); //?„ì°??œ?œ?€ë¡œ ?•??ê°™?„?œ ?„ë¡œ?¸ìŠ?ë²ˆ?¸ëŒ€ë¡œ)

	time = 0;
	gantt_index = 0;
	int N = num_proc;//N:?¤í–‰????˜ì§€ ?Š?€ ?„ë¡œ?¸ìŠ??˜. ?…?¥ëœ ?„ë¡œ?¸ìŠ¤ìˆ˜ë¥?ë³µì‚¬í•˜??ì´ˆê¸°í™”



	for (int i = 0; i < num_proc; i++) {
		if (0 == procs[i].at) {	//도착한 프로세스 푸쉬
			pri_push(&front, &rear, num_proc + 1, i, procs[i].pri ,queue,pri_queue);

		}
	}


	while (N > 0) { //ì¢…ë£Œì¡°ê±´: line136 

		if (index == 0) {	//?¸ë±ìŠ¤ê? 0?´ë©´ pop?˜ê³  ì´ˆê¸°í™”

			queue_index = pop(&front, &rear, num_proc + 1, queue);

			if (queue_index == -1) {


				for (int i = 0; i < num_proc; i++) {
					if (time + 1 == procs[i].at) {	//도착한 프로세스 푸쉬
						pri_push(&front, &rear, num_proc + 1, i, procs[i].pri, queue, pri_queue);

					}
				}

				time++;//?œê°„?ë¦„
				continue;
			}
			if (procs[queue_index].p == g_p[gantt_index - 1]) {	//?„ ?„ë¡œ?¸ìŠ¤ëž‘ ?„ ?„ë¡œ?¸ìŠ??™?¼í•˜ë©?ê°„?¸ì¸ë±ìŠ?? ì§€
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

		for (int i = 0; i < num_proc; i++) {	//?¤í–‰????˜ì§€ ?Š?€ ?„ë¡œ?¸ìŠ¤ì? ?¤í–‰?˜ì§€ ?Š?€ ?„ë¡œ?¸ìŠ¤ë“¤ì˜ ?€ê¸°ì‹œê°„ ì¦ê°€
			if (i != queue_index && procs[i].at <= time && (procs[i].c == -1))
				(procs[i].wt)++;
		}
		(procs[queue_index].rem)--;//?¤í–‰ì¤‘???„ë¡œ?¸ìŠ¤ì˜ remain time ê°?Œ
		(g_bt[gantt_index])++;


		for (int i = 0; i < num_proc; i++) {
			if (time + 1 == procs[i].at) {	//도착한 프로세스 푸쉬
				pri_push(&front, &rear, num_proc + 1, i, procs[i].pri, queue, pri_queue);

			}
		}


		if (procs[queue_index].et == -1) procs[queue_index].et = time;//?„ë¡œ?¸ìŠ¤ê? ìµœì´ˆë¡œ ?¤í–‰?œ ?œê°„?„ et? ?€??
		if (procs[queue_index].rem == 0)	//?¤í–‰?„ë£Œ?˜???„ë¡œ?¸ìŠ?ë°”ê¿”ì£¼ê¸°
		{
			procs[queue_index].ct = time + 1;
			procs[queue_index].tat = procs[queue_index].ct - procs[queue_index].at;
			procs[queue_index].c = 0;
			N = N - 1;//?¤í–‰ê°€?¥í•œ ?„ë¡œ?¸ìŠ??˜ ê°?Œ
			if (N == 0) {//ëª¨ë“  ?„ë¡œ?¸ìŠ¤ê? ?¤í–‰ì¢…ë£Œ?œ ê²½ìš?ë°˜ë³µë¬¸ ?ˆì¶œ
				time++;
				break;
			}
			gantt_index++;
			index = 0;


		}
		else if (data->tq == index + 1)	//time-quantom? ?˜???„ë¡œ?¸ìŠ?ë°”ê¿”ì£¼ê¸°
		{

			pri_push(&front, &rear, num_proc + 1, queue_index,procs[queue_index].pri, queue,pri_queue);

			gantt_index++;
			index = 0;

		}
		else
			index++;

		time++;//?œê°„???ë¦„

	}
	//?„ë¡œ?¸ì‹?ì¢…ë£Œ

	qsort(procs, num_proc, sizeof(PROC), compare_p);//procsë¥??¤ì‹œ pid?œ?¼ë? ?•??


	(data->g_p) = g_p;
	(data->g_et) = g_et;
	(data->g_bt) = g_bt;
	(data->gantt_index) = gantt_index;
	free(queue);
	free(pri_queue);


	

	// ê°„??ì°¨íŠ?ê·¸ë¦¬ê¸?
	Print_gantt(data);
	Print_table(data);
	printf("\n\n");
	Print_readyQueue(data);
	// ê°„??ì°¨íŠ??



	return;
}

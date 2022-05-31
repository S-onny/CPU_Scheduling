#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"

int Process_load(FILE* fp,int* num_proc, PROC* procs){
	PROC temp;
	int ret;
	ret=fscanf(fp,"%d",num_proc);
	if (ret==EOF)
	{
		printf("FILE error:invaild format or empty!");
		return=0;
	} 

	procs=(PROC*)realloc(procs,sizeof(PROC)*(*num_proc));
	for(int i=0; i<(*num_proc);i++)
	{
		ret=fscanf(fp,"P%d %d %d %d",&(temp.p),&(temp.at),&(temp.bt),&(temp.pri));
		if (ret==EOF)
		{
			printf("FILE error:not enough processes or invaild format");
			return 0;
		}
		printf("reading process P%d...",temp.p);
		procs[i]=temp;
		procs[i].rem = procs[i].bt;//�����ð� burst time���� �ʱ�ȭ
		procs[i].wt = 0;//��Ÿ �ʿ��� ��ġ�� �ʱ�ȭ
		procs[i].et = 0 - 1;//���� ����ð� ������ ���� -1�� �ʱ�ȭ
		procs[i].ct = 0;
		procs[i].tat = 0;
		procs[i].c = -1;
	}
	ret=fscanf(fp,"%d",tq);
	if(ret==EOF)
	{
		printf("FILE error:No Time quantum!");
		return 0;
	}
	return 1;

	
}

PROC* Copy_processes(PROC* procs){
	PROC* temp = (PROC*)malloc(sizeof(procs));
	memcpy(temp,procs,sizeof(procs));
	return temp;
}
DATA* Make_dataIn(PROC* procArr,int num_proc){
	DATA* DataArr=(DATA*)malloc(sizeof(DATA)*7);
	for(int i = 0; i<7; i++){
		DataArr[i].procs=Copy_processes(PROC* procArr);
		DataArr[i].g_p=NULL;
		DataArr[i].g_et=NULL;
		DataArr[i].g_bt=NULL;
		DataArr[i].gantt_index=0;
		
	}
	return DataArr;
}
void Destroy_data(DATA* datas){
	for(int i =0;i<7;i++){
		if (datas[i].procs!=NULL) free(datas[i].procs);
		if(datas[i].g_p!=NULL){
			free(datas[i].g_p);
			free(datas[i].g_et);
			free(datas[i].g_bt);
		}
		free(datas);
	}
}
int compare_a(const void* p1, const void* p2) {//arrival ���� �� ������ ���� compare �Լ�
	int proc_cmp;
	proc_cmp = ((PROC*)p1)->at - ((PROC*)p2)->at;
	if (proc_cmp == 0)
		return ((PROC*)p1)->p - ((PROC*)p2)->p;
	return proc_cmp;
}

int compare_p(const void* p1, const void* p2) {//pid������ �����ϱ����� compare �Լ�
	return ((PROC*)p1)->p - ((PROC*)p2)->p;
}
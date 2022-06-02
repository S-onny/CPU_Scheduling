
#include "proc_data.h"
typedef struct{
	char name[40];
	int twt;
	int ttat;
	int trt;
}RANK;
int compare_w(const void* p1, const void* p2); //twt순으로 정렬하기위한 compare 함수

int compare_t(const void* p1, const void* p2);//ttat순으로 정렬하기위한 compare 함수

int compare_r(const void* p1, const void* p2);//trt순으로 정렬하기위한 compare 함수

void al_rank(int * check,DATA* datas);
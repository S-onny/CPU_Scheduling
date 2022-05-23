#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"

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
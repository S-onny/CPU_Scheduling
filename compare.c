#include <stdio.h>
#include <stdlib.h>
#include "proc_data.h"

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

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
	int c;//�Ϸ������� 0 �ƴϸ� -1
}PROC;

int Process_load(FILE* fp,int* num_proc,int* tq, PROC* procs);